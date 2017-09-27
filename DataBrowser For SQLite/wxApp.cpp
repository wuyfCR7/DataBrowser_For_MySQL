#include "wxApp.h"
#include "wxSQLiteFrame.h"
/*
#include "wxcontrol.h"
*/

wxDECLARE_APP(wxMyApp);
wxIMPLEMENT_APP(wxMyApp);

bool wxMyApp::OnInit()
{
	sqlite_ui_frame_ = new WxSQLiteUIFrame(NULL, wxID_ANY, wxT("DataBrowser For SQLite"));
	sqlite_ui_frame_->Show(true);
	SetTopWindow(sqlite_ui_frame_);
	return true;
}