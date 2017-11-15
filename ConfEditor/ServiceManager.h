#ifndef SERCICE_MANAGER_H_
#define SERCICE_MANAGER_H_

#include <wx/wx.h>
#include <mutex>

class ServiceManager
{
	wxString serviceName;

	SC_HANDLE theService, scm;
	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwBytesNeeded;
	const DWORD DEFAULT_TIMEOUT = 30000;

	bool WaitForState(DWORD waitFor, DWORD startState, DWORD timeout);
	bool StopDependentServices();

public:
	std::mutex m;

	void LoadServiceName();
	static wxString GetServiceName();

	bool OpenManagerAndService();
	bool OpenScmManager();
	bool CloseScmManager();

	bool OpenServiceWithAccess(DWORD dwDesiredAccess); //SERVICE_QUERY_STATUS
	bool CloseService();

	bool QueuryServiceStatus();
	bool StopService();
	bool startService(); // starts with lowercase to resolve name collision with #define
	bool RestartService();

	DWORD GetServiceCurrentState();
};

#endif
