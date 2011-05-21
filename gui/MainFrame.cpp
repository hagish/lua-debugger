/*
 * The MIT License - see LICENSE file for details
 */

#include <stdio.h>
#include "MainFrame.h"
#include "DebugPanel.h"
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT_ADD_DEBUG_THREAD)
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT_REMOVE_DEBUG_THREAD)

MainFrame::MainFrame() :
	MainFrameLayout(NULL), currentActiveDebugThread(NULL)
{
	this->Connect(wxID_ANY, wxEVT_MY_EVENT_ADD_DEBUG_THREAD,
			wxCommandEventHandler( MainFrame::OnAddDebugThread ));
	this->Connect(wxID_ANY, wxEVT_MY_EVENT_REMOVE_DEBUG_THREAD,
			wxCommandEventHandler( MainFrame::OnRemoveDebugThread ));
}

MainFrame::~MainFrame()
{
	this->Disconnect(wxID_ANY, wxEVT_MY_EVENT_ADD_DEBUG_THREAD,
			wxCommandEventHandler( MainFrame::OnAddDebugThread ));
	this->Disconnect(wxID_ANY, wxEVT_MY_EVENT_REMOVE_DEBUG_THREAD,
			wxCommandEventHandler( MainFrame::OnRemoveDebugThread ));
}

void MainFrame::OnAddDebugThread(wxCommandEvent &event)
{
	DebugThread *thread = (DebugThread *) event.GetClientData();

	DebugPanel *panel = new DebugPanel(m_notebook);
	threadPanelMapping[thread] = panel;

	m_notebook->AddPage(panel,
			wxString::Format(wxT("0x%x"), (ptrdiff_t) thread));

	panel->assignDebugThread(thread);
	thread->assignDebugPanel(panel);

}

void MainFrame::exit( wxCommandEvent& event )
{
	this->Close();
}

void MainFrame::OnRemoveDebugThread(wxCommandEvent &event)
{
	DebugThread *thread = (DebugThread *) event.GetClientData();
	DebugPanel *panel = threadPanelMapping[thread];
	threadPanelMapping[thread] = NULL;

	// find panel and remove
	for (int i = 0; i < m_notebook->GetPageCount(); i++)
	{
		if (m_notebook->GetPage(i) == panel)
		{
			m_notebook->RemovePage(i);
		}
	}
}

void MainFrame::toolBreakpoint( wxCommandEvent& event )
{
	if (currentActiveDebugThread != NULL)
	{
		DebugPanel *p = threadPanelMapping[currentActiveDebugThread];
		if (p != NULL)
		{
			p->toggleBreakpoint();
		}
	}
}

void MainFrame::toolContinue(wxCommandEvent& event)
{
	if (currentActiveDebugThread != NULL)
	{
		currentActiveDebugThread->postline("run\n");
	}
}

void MainFrame::toolStepIn(wxCommandEvent& event)
{
	if (currentActiveDebugThread != NULL)
	{
		currentActiveDebugThread->postline("step\n");
	}
}

void MainFrame::toolEval(wxCommandEvent& event)
{
	if (currentActiveDebugThread != NULL)
	{
		DebugPanel *p = threadPanelMapping[currentActiveDebugThread];
		if (p != NULL)
		{
			p->evalCurrentSelection();
		}
	}
}

void MainFrame::toolRemove(wxCommandEvent& event)
{
	if (currentActiveDebugThread != NULL)
	{
		DebugPanel *p = threadPanelMapping[currentActiveDebugThread];
		if (p != NULL)
		{
			currentActiveDebugThread->postline("exit");

			threadPanelMapping[currentActiveDebugThread] = NULL;

			// find panel and remove
			for (int i = 0; i < m_notebook->GetPageCount(); i++)
			{
				if (m_notebook->GetPage(i) == p)
				{
					m_notebook->RemovePage(i);
				}
			}
		}
	}
}

void MainFrame::toolStep(wxCommandEvent& event)
{
	if (currentActiveDebugThread != NULL)
	{
		currentActiveDebugThread->postline("over\n");
	}
}

DebugThread *MainFrame::findDebugThreadFromPanel(DebugPanel *panel)
{
	for (std::map<DebugThread *, DebugPanel *>::const_iterator it =
			threadPanelMapping.begin(); it != threadPanelMapping.end(); ++it)
	{
		if (it->second == panel)
		{
			return it->first;
		}
	}

	return NULL;
}

void MainFrame::pageChanged(wxNotebookEvent& event)
{
	int page = event.GetSelection();

	if (page != -1)
	{
		DebugPanel *panel = (DebugPanel *) m_notebook->GetPage(page);
		currentActiveDebugThread = findDebugThreadFromPanel(panel);
	}
}
