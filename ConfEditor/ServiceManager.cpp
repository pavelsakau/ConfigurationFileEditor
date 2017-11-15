#include "ServiceManager.h"
#include <wx/msw/registry.h>
#include <fstream>

wxString ServiceManager::GetServiceName()
{
	wxRegKey key(wxRegKey::HKLM, "SOFTWARE\\WOW6432Node\\NetLatency\\SwitchMonitor");
	wxString serviceName;
	key.QueryValue("ServiceName", serviceName);

	return serviceName;
}

void ServiceManager::LoadServiceName()
{
	serviceName = GetServiceName();
}

bool ServiceManager::OpenManagerAndService()
{
	if (OpenScmManager()) {
		if (OpenServiceWithAccess(SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP | SERVICE_ENUMERATE_DEPENDENTS)) {
			return true;
		}
	}
	return false;
}

bool ServiceManager::OpenScmManager()
{
	scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
	if (!scm) {
		//wxMessageBox(wxT("OpenSCManager failure"));
		return false;
	}
	return true;
}

bool ServiceManager::CloseScmManager()
{
	if (scm != nullptr) {
		CloseServiceHandle(scm);
		return true;
	}
	return false;
}

bool ServiceManager::OpenServiceWithAccess(DWORD dwDesiredAccess)
{
	if (scm != nullptr) {
		theService = OpenService(scm, serviceName, dwDesiredAccess);
		if(!theService) {
			//wxMessageBox(wxT("OpenService failure"));
			//DWORD lastError = GetLastError();
			//if (lastError == ERROR_ACCESS_DENIED) {
			//	wxMessageBox(wxT("ERROR_ACCESS_DENIED"));
			//} else if (lastError == ERROR_INVALID_HANDLE) {
			//	wxMessageBox(wxT("ERROR_INVALID_HANDLE"));
			//} else if (lastError == ERROR_INVALID_NAME) {
			//	wxMessageBox(wxT("ERROR_INVALID_NAME"));
			//} else if (lastError == ERROR_SERVICE_DOES_NOT_EXIST) {
			//	wxMessageBox(wxT("ERROR_SERVICE_DOES_NOT_EXIST"));
			//}
			//wxMessageBox(wxString::Format("error code: %i", lastError));
			CloseScmManager();
			return false;
		} 
	} else {
		return false;
	}
	return true;
}

bool ServiceManager::CloseService()
{	
	if (theService != nullptr) {
		CloseServiceHandle(theService);
		return true;
	}
	return false;
}

bool ServiceManager::WaitForState(DWORD waitFor, DWORD startState, DWORD timeout)
{
	SERVICE_STATUS_PROCESS ssp;
	DWORD dwStartTime;
	DWORD dwBytesNeeded;
	if (scm != nullptr && theService != nullptr) {
		dwStartTime = GetTickCount();
		ssp.dwCurrentState = startState;
		while (ssp.dwCurrentState != waitFor) 
		{
			if (!QueryServiceStatusEx(theService, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
				//wxMessageBox(wxT("WaitForState QueryServiceStatusEx failure"));
				return false;
			}
			if (ssp.dwCurrentState == waitFor)
				break;
			if (GetTickCount() - dwStartTime > timeout)
			{
				//wxMessageBox(wxT("StopService timeout"));
				return false;
			}
			Sleep(ssp.dwWaitHint);
		}
	} else {
		return false;
	}
	return true;
}

bool ServiceManager::StopDependentServices()
{
	DWORD i;
	DWORD dwBytesNeeded;
	DWORD dwCount;
	LPENUM_SERVICE_STATUS lpDependencies = NULL;
	ENUM_SERVICE_STATUS ess;
	SC_HANDLE hDepService;
	SERVICE_STATUS_PROCESS ssp;
	DWORD dwStartTime;

	if (EnumDependentServices(theService, SERVICE_ACTIVE, lpDependencies, 0, &dwBytesNeeded, &dwCount)) { // Pass a zero-length buffer to get the required buffer size
		return true;
	} else {
		if (GetLastError() != ERROR_MORE_DATA)
			return false;
		lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded); // Allocate a buffer for the dependencies
		if (!lpDependencies)
			return false;
		try {
			if (!EnumDependentServices(theService, SERVICE_ACTIVE, lpDependencies, dwBytesNeeded, &dwBytesNeeded, &dwCount))
				return false;
			for (i = 0; i < dwCount; i++) 
			{
				ess = *(lpDependencies + i);
				hDepService = OpenService(scm, ess.lpServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
				if (!hDepService)
					return false;
				try {
					if (!ControlService(hDepService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS) &ssp))
						return false;
					dwStartTime = GetTickCount();
					ssp.dwCurrentState = SERVICE_RUNNING;
					while (ssp.dwCurrentState != SERVICE_STOPPED) 
					{
						if (!QueryServiceStatusEx(hDepService, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(SERVICE_STATUS_PROCESS),	&dwBytesNeeded))
							return false;
						if (ssp.dwCurrentState == SERVICE_STOPPED)
							break;
						if (GetTickCount() - dwStartTime > DEFAULT_TIMEOUT)
							return false;
						Sleep(ssp.dwWaitHint);
					}
				} catch(...) {
					CloseServiceHandle(hDepService);
					return false;
				}
				CloseServiceHandle(hDepService);
			}
		} catch (...) {
			HeapFree(GetProcessHeap(), 0, lpDependencies);
			return false;
		}
		HeapFree(GetProcessHeap(), 0, lpDependencies);
	}
	return true;
}

bool ServiceManager::StopService()
{
	SERVICE_STATUS_PROCESS ssp;
	if (scm != nullptr && theService != nullptr) {
		if (!StopDependentServices()) {
			return false;
		}
		if (!ControlService(theService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS) &ssp))
		{
			//wxMessageBox(wxT("StopService failure"));
			return false;
		} 
		return WaitForState(SERVICE_STOPPED, SERVICE_RUNNING, DEFAULT_TIMEOUT);
	} else {
		return false;
	}
}

bool ServiceManager::startService()
{
	if (scm != nullptr && theService != nullptr) {
		if (!StartService(theService, 0, nullptr)) {
			//wxMessageBox(wxT("StartService failure"));
			return false;
		}
		return WaitForState(SERVICE_RUNNING, SERVICE_STOPPED, DEFAULT_TIMEOUT);
	} else {
		return false;
	}
}

bool ServiceManager::RestartService()
{
	// handled by main window start and stop methods
}

bool ServiceManager::QueuryServiceStatus()
{
	if (scm != nullptr && theService != nullptr) {
		if (!QueryServiceStatusEx(theService, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssStatus), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded)) {
			//wxMessageBox(wxT("QueryServiceStatusEx failure"));
			return false;
		}
		return true;
	} else {
		return false;
	}
}

DWORD ServiceManager::GetServiceCurrentState()
{
	return ssStatus.dwCurrentState;
}