/*
 * The MIT License - see LICENSE file for details
 */

#include "DebugPanel.h"
#include "DebugThread.h"
#include "FileUtils.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT_PRINT_LINE)
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT_NOTIFY_FILE_AND_LINE)
DEFINE_EVENT_TYPE(wxEVT_MY_EVENT_NOTIFY_BREAKPOINT_UPDATE)

DebugPanel::DebugPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
		const wxSize& size, long style) :
	DebugPanelLayout(parent, id, pos, size, style), debugThread(NULL)
{
	this->Connect(wxID_ANY, wxEVT_MY_EVENT_PRINT_LINE,
			wxCommandEventHandler( DebugPanel::OnPrintLine ));
	this->Connect(wxID_ANY, wxEVT_MY_EVENT_NOTIFY_FILE_AND_LINE,
			wxCommandEventHandler( DebugPanel::OnNotifyFileAndLine ));
	this->Connect(wxID_ANY, wxEVT_MY_EVENT_NOTIFY_BREAKPOINT_UPDATE,
			wxCommandEventHandler( DebugPanel::OnNotifyBreakpointUpdate ));
}

DebugPanel::~DebugPanel()
{
	this->Disconnect(wxID_ANY, wxEVT_MY_EVENT_PRINT_LINE,
			wxCommandEventHandler( DebugPanel::OnPrintLine ));
	this->Disconnect(wxID_ANY, wxEVT_MY_EVENT_NOTIFY_FILE_AND_LINE,
			wxCommandEventHandler( DebugPanel::OnNotifyFileAndLine ));
	this->Disconnect(wxID_ANY, wxEVT_MY_EVENT_NOTIFY_BREAKPOINT_UPDATE,
			wxCommandEventHandler( DebugPanel::OnNotifyBreakpointUpdate ));
}

void DebugPanel::evalCurrentSelection()
{
	wxString selection = m_code->GetSelectedText();

	if (selection.Length() > 0)
	{
		assert(debugThread != NULL);

		debugThread->postline(std::string((wxT("eval ") + selection).mb_str()));
	}
}

void DebugPanel::updateBreakpoints()
{
	assert(debugThread != NULL);

	m_code->MarkerDefine(1, wxSCI_MARK_CIRCLE);
	m_code->MarkerDeleteAll(1);

	std::vector<unsigned int> breakpoints = debugThread->listBreakpointOfFile(
			currentFile);
	BOOST_FOREACH(unsigned int line, breakpoints)
				{
					// mark current line
					m_code->MarkerAdd(line - 1, 1);
				}
}

void DebugPanel::OnNotifyBreakpointUpdate(wxCommandEvent& event)
{
	updateBreakpoints();
}

void DebugPanel::OnNotifyFileAndLine(wxCommandEvent& event)
{
	wxString file = event.GetString();
	unsigned int line = (unsigned int) event.GetInt();

	currentFile = std::string(file.mb_str());

	// set current source
	std::string src = FileUtils::getContent(std::string(file.mb_str()));
	m_code->SetReadOnly(false);
	m_code->SetText(wxString(src.c_str(), wxConvUTF8));
	m_code->SetReadOnly(true);

	// scroll to current line
	m_code->EnsureVisible(line - 1);
	m_code->ScrollToLine(line - 1 - 4);

	// line numbers
	int marginWidth = m_code->TextWidth(wxSCI_STYLE_LINENUMBER, _T("99999"));
	m_code->SetMarginWidth(0, marginWidth);

	m_code->SetWrapMode(wxSCI_WRAP_NONE);

	// mark current line
	m_code->MarkerDefine(0, wxSCI_MARK_ARROW);
	m_code->MarkerDeleteAll(0);
	m_code->MarkerAdd(line - 1, 0);

	wxFont font(10, wxTELETYPE, wxFIXED, wxNORMAL);
	m_code->StyleSetFont(wxSCI_STYLE_DEFAULT, font);
	m_code->StyleSetForeground(wxSCI_STYLE_DEFAULT, wxColour(wxT("BLACK")));
	m_code->StyleSetBackground(wxSCI_STYLE_DEFAULT, wxColour(wxT("WHITE")));
	m_code->StyleSetForeground(wxSCI_STYLE_LINENUMBER,
			wxColour(wxT("DARK BLUE")));
	m_code->StyleSetBackground(wxSCI_STYLE_LINENUMBER, wxColour(wxT("WHITE")));
	m_code->StyleSetForeground(wxSCI_STYLE_INDENTGUIDE,
			wxColour(wxT("DARK GREY")));

	// enable lua syntax highlight
	m_code->SetLexerLanguage(wxT("lua"));
	m_code->Colourise(0, wxSCI_INVALID_POSITION);

	updateBreakpoints();
}

void DebugPanel::OnPrintLine(wxCommandEvent& event)
{
	m_textCtrlOutput->AppendText(event.GetString() + wxT("\n"));
}

void DebugPanel::assignDebugThread(DebugThread *thread)
{
	debugThread = thread;
}

void DebugPanel::toggleBreakpoint()
{
	assert(debugThread != NULL);

	unsigned int line = m_code->GetCurrentLine();
	line = line + 1;

	if (debugThread->hasBreakpointAt(currentFile, line))
	{
		debugThread ->postline(
				(boost::format("delb %1% %2%") % currentFile % line).str());
	}
	else
	{
		debugThread ->postline(
				(boost::format("setb %1% %2%") % currentFile % line).str());
	}

	updateBreakpoints();
}

void DebugPanel::OnTextEnter(wxCommandEvent& event)
{
	assert(debugThread != NULL);

	std::string line(m_textCtrlConsole->GetValue().mb_str());
	debugThread->postline(line);
	m_textCtrlConsole->SetValue(wxT(""));
}
