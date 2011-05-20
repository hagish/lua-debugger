///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __Layout__
#define __Layout__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include "wx/wxscintilla.h"
#include <wx/textctrl.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameLayout
///////////////////////////////////////////////////////////////////////////////
class MainFrameLayout : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			wxID_MENU_EXIT = 1000,
			wxID_TOOL_REMOVE,
			wxID_TOOL_CONTINUE,
			wxID_TOOL_STEP,
			wxID_TOOL_STEPIN,
			wxID_TOOL_EVAL,
			wxID_TOOL_BREAKPOINT,
		};
		
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxToolBar* m_toolBar;
		wxStatusBar* m_statusBar;
		wxNotebook* m_notebook;
		
		// Virtual event handlers, overide them in your derived class
		virtual void exit( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolContinue( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolStep( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolStepIn( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolEval( wxCommandEvent& event ) { event.Skip(); }
		virtual void toolBreakpoint( wxCommandEvent& event ) { event.Skip(); }
		virtual void pageChanged( wxNotebookEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("lua-debugger v0.1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrameLayout();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DebugPanelLayout
///////////////////////////////////////////////////////////////////////////////
class DebugPanelLayout : public wxPanel 
{
	private:
	
	protected:
		wxScintilla* m_code;
		wxTextCtrl* m_textCtrlOutput;
		wxTextCtrl* m_textCtrlConsole;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnTextEnter( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DebugPanelLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~DebugPanelLayout();
	
};

#endif //__Layout__
