#ifndef __WX_APP_HH__
#define __WX_APP_HH__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <wx/wxprec.h>

class WxSQLiteUIFrame;
class wxMyApp :public wxApp{
public:
	virtual bool OnInit();
	WxSQLiteUIFrame* sqlite_ui_frame_;
};




#endif