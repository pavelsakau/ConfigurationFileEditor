#ifndef TOOLBAR_TIMER_H_
#define TOOLBAR_TIMER_H_

#include "wx/wx.h"
#include "Toolbar.h"
#include "ServiceManager.h"

class ToolbarTimer :public wxTimer
{
	Toolbar* toolbar;
	ServiceManager* serviceManager;

	const int TIMER_WAIT = 3000;

public:

	ToolbarTimer(Toolbar* toolbar, ServiceManager* serviceManager);
	void Notify();
	void StartTimer();
};

#endif
