/*
 * The MIT License - see LICENSE file for details
 */

#include <wx/wx.h>

#include "Layout.h"
#include "DebugThread.h"
#include "StringUtils.h"
#include "Server.h"
#include "MainFrame.h"

class MyApp: public wxApp
{
	virtual bool OnInit()
	{
		wxInitAllImageHandlers();

		MainFrame *frame = new MainFrame();
		frame->Show(true);
		SetTopWindow(frame);

		server = new Server(8171, frame);
		server->start();

		return true;
	}

	virtual int OnExit()
	{
		server->stop();
		delete server;
		return 0;
	}

	Server *server;
};

IMPLEMENT_APP(MyApp)
