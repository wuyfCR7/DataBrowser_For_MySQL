#ifndef __WX_SQLITE_DATA_HH__
#define __WX_SQLITE_DATA_HH__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <wxCommon.h>
#include <Lite3Client.h>

struct GridData;

extern SQLite3ClientManager SQLITE_CLIENT_MANAGER;
extern std::vector<wxColour> COLOR_LIST;
extern int DEFAULT_ONEPAGE_AMOUNT;

class WxSQLiteData : public wxPanel
{
public:
	wxGrid* sqlite_data_;
	wxButton* btn_next_;
	wxButton* btn_prev_;
	wxButton* btn_first_;
	wxTextCtrl* sql_edit_;
	wxTextCtrl* current_page_;
	wxTextCtrl* total_page_;

public:
	int page_;
	int all_page_;
	SQLite3DataBasePtr db_ptr_;
	std::string tableview_name_;

public:
	WxSQLiteData(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 750, 700 ), long style = wxTAB_TRAVERSAL );

	void OnNextCallBack( wxCommandEvent& );
	void OnPrevCallBack( wxCommandEvent& );
	void OnFirstCallBack( wxCommandEvent& );

public:
	DECLARE_EVENT_TABLE();
};


/// 数据的展示
struct GridData
{
	std::vector<std::vector<std::string>> data_;
	std::vector<std::string> header_;
};
void DataGridPresent(wxGrid*, GridData&);

/// 数据查询回调函数
int QryGridDataCallBack(void*, int, char**, char**);

/// 页面总行数回调函数
int QryTotalLineCallBack(void*, int, char**, char**);











#endif