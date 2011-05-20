/*
 * The MIT License - see LICENSE file for details
 */

#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include "Layout.h"
#include "DebugThread.h"

DECLARE_EVENT_TYPE(wxEVT_MY_EVENT_ADD_DEBUG_THREAD, -1)
DECLARE_EVENT_TYPE(wxEVT_MY_EVENT_REMOVE_DEBUG_THREAD, -1)

class MainFrame: public MainFrameLayout
{
public:
	MainFrame();
	virtual ~MainFrame();

protected:
	void addDebugThread(DebugThread *debugThread);
	void removeDebugThread(DebugThread *debugThread);

	virtual void OnAddDebugThread( wxCommandEvent &event );
	virtual void OnRemoveDebugThread( wxCommandEvent &event );

	virtual void toolContinue(wxCommandEvent& event);
	virtual void toolStepIn(wxCommandEvent& event);
	virtual void toolStep(wxCommandEvent& event);
	virtual void toolEval( wxCommandEvent& event );
	virtual void toolRemove( wxCommandEvent& event );
	virtual void toolBreakpoint( wxCommandEvent& event );
	virtual void exit( wxCommandEvent& event );

	virtual void pageChanged(wxNotebookEvent& event);

private:
	DebugThread *findDebugThreadFromPanel(DebugPanel *panel);

	std::map<DebugThread *, DebugPanel *> threadPanelMapping;
	DebugThread *currentActiveDebugThread;
};

#endif /* MAINFRAME_H_ */
