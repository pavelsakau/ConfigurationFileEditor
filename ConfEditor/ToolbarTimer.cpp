#include "ToolbarTimer.h"

ToolbarTimer::ToolbarTimer(Toolbar* toolbar, ServiceManager* serviceManager) : wxTimer(), toolbar(toolbar), serviceManager(serviceManager)
{
}

void ToolbarTimer::Notify()
{
	Stop();
	std::lock_guard<std::mutex> guard(serviceManager->m);
	toolbar->UpdateServerButtons(serviceManager);
	Start(TIMER_WAIT);
}

void ToolbarTimer::StartTimer()
{
	Start(TIMER_WAIT);
}