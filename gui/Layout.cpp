///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui/Layout.h"

///////////////////////////////////////////////////////////////////////////

MainFrameLayout::MainFrameLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menuFile, wxID_MENU_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemExit );
	
	m_menubar->Append( m_menuFile, wxT("File") );
	
	this->SetMenuBar( m_menubar );
	
	m_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolBar->AddTool( wxID_TOOL_REMOVE, wxT("remove"), wxBitmap( wxT("gfx/remove.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("remove current session"), wxEmptyString );
	m_toolBar->AddSeparator();
	m_toolBar->AddTool( wxID_TOOL_CONTINUE, wxT("continue"), wxBitmap( wxT("gfx/continue.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("continue"), wxEmptyString );
	m_toolBar->AddTool( wxID_TOOL_STEP, wxT("step"), wxBitmap( wxT("gfx/step.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("step"), wxEmptyString );
	m_toolBar->AddTool( wxID_TOOL_STEPIN, wxT("stepin"), wxBitmap( wxT("gfx/step_in.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("step in"), wxEmptyString );
	m_toolBar->AddSeparator();
	m_toolBar->AddTool( wxID_TOOL_EVAL, wxT("eval"), wxBitmap( wxT("gfx/eval.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("eval current selection"), wxEmptyString );
	m_toolBar->AddSeparator();
	m_toolBar->AddTool( wxID_TOOL_BREAKPOINT, wxT("breakpoint"), wxBitmap( wxT("gfx/breakpoint.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("toggle breakpoint"), wxEmptyString );
	m_toolBar->Realize();
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 1, 0, 0 );
	
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( gSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameLayout::exit ) );
	this->Connect( wxID_TOOL_REMOVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolRemove ) );
	this->Connect( wxID_TOOL_CONTINUE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolContinue ) );
	this->Connect( wxID_TOOL_STEP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolStep ) );
	this->Connect( wxID_TOOL_STEPIN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolStepIn ) );
	this->Connect( wxID_TOOL_EVAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolEval ) );
	this->Connect( wxID_TOOL_BREAKPOINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolBreakpoint ) );
	m_notebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MainFrameLayout::pageChanged ), NULL, this );
}

MainFrameLayout::~MainFrameLayout()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameLayout::exit ) );
	this->Disconnect( wxID_TOOL_REMOVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolRemove ) );
	this->Disconnect( wxID_TOOL_CONTINUE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolContinue ) );
	this->Disconnect( wxID_TOOL_STEP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolStep ) );
	this->Disconnect( wxID_TOOL_STEPIN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolStepIn ) );
	this->Disconnect( wxID_TOOL_EVAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolEval ) );
	this->Disconnect( wxID_TOOL_BREAKPOINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameLayout::toolBreakpoint ) );
	m_notebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MainFrameLayout::pageChanged ), NULL, this );
}

DebugPanelLayout::DebugPanelLayout( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_code = new wxScintilla(this);
	m_code->SetReadOnly(true);
	bSizer4->Add( m_code, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlOutput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_textCtrlOutput->SetFont( wxFont( 11, 76, 90, 90, false, wxT("Monospace") ) );
	
	bSizer4->Add( m_textCtrlOutput, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrlConsole = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	m_textCtrlConsole->SetFont( wxFont( 11, 76, 90, 90, false, wxT("Monospace") ) );
	
	bSizer4->Add( m_textCtrlConsole, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	// Connect Events
	m_textCtrlConsole->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DebugPanelLayout::OnTextEnter ), NULL, this );
}

DebugPanelLayout::~DebugPanelLayout()
{
	// Disconnect Events
	m_textCtrlConsole->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DebugPanelLayout::OnTextEnter ), NULL, this );
}
