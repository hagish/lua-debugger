/*
 * The MIT License - see LICENSE file for details
 */

#ifndef DEBUGPANEL_H_
#define DEBUGPANEL_H_
#include "Layout.h"
DECLARE_EVENT_TYPE(wxEVT_MY_EVENT_PRINT_LINE, -1)
DECLARE_EVENT_TYPE(wxEVT_MY_EVENT_NOTIFY_FILE_AND_LINE, -1)
DECLARE_EVENT_TYPE(wxEVT_MY_EVENT_NOTIFY_BREAKPOINT_UPDATE, -1)

class DebugThread;

class DebugPanel: public DebugPanelLayout
{
public:
			DebugPanel(wxWindow* parent, wxWindowID id = wxID_ANY,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxSize(500, 300),
					long style = wxTAB_TRAVERSAL);
	virtual ~DebugPanel();

	void assignDebugThread(DebugThread *thread);
	void toggleBreakpoint();
	void evalCurrentSelection();
protected:
	virtual void OnPrintLine(wxCommandEvent& event);
	virtual void OnNotifyFileAndLine(wxCommandEvent& event);
	virtual void OnNotifyBreakpointUpdate(wxCommandEvent& event);
	virtual void OnTextEnter(wxCommandEvent& event);

private:
	void updateBreakpoints();

	DebugThread *debugThread;
	std::string currentFile;
};

#endif /* DEBUGPANEL_H_ */
