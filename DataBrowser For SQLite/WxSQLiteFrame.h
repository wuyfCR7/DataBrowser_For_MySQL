#ifndef __WX_SQLITE_FRAME_HH__
#define __WX_SQLITE_FRAME_HH__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <wxCommon.h>
#include "WxSQLiteData.h"

class WxSQLiteUIFrame :public wxFrame
{
public:
	WxSQLiteUIFrame(wxWindow*, wxWindowID, const wxString&);
public:
	wxMenuBar* menubar_;
	wxMenu* menu_file_;
	wxMenu* menu_database_;
	wxToolBar* toolbar_;
	wxTreeCtrl* sqlite3_tree_;
	wxNotebook* sqlite_infobook_;
	wxMenu* popup_;

	WxSQLiteData* data_page_;
	wxTextCtrl* sql_edit_;

	std::string sqlite_config_file_;

	wxImageList imagelist_;
public:
	void OnSQLiteTreeRightClick( wxTreeEvent& );

	void OnNewDataBase( wxCommandEvent&  );
	void OnRmOneDataBase( wxCommandEvent& );
	void OnRmAllDataBase( wxCommandEvent& );
	void OnCloseWindow( wxCommandEvent& );
	void OnTableShow( wxCommandEvent&  );
	void OnViewShow( wxCommandEvent& );
	void OnTriggerShow( wxCommandEvent& );
	void OnIndexShow( wxCommandEvent& );

	void OnDataQuery( wxCommandEvent& );
	void OnDataBaseRefresh( wxCommandEvent& );
	void OnDataBaseRefreshAll( wxCommandEvent& );

public:
	DECLARE_EVENT_TABLE();
};

/// 更新TreeCtrl所有的信息
void UpdateTreeCtrlSQLiteAllInfo(SQLite3ClientManager&, wxTreeCtrl*);

/// 更新TreeCtrl单个数据库的信息
void UpdateTreeCtrlSQLiteOneDBInfo(SQLite3ClientManager&, wxTreeCtrl*, std::string);

/// 初始化SQLite客户端
void InitSQLiteClientTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*, wxImageList*);

/// 添加单个数据库节点
void AddSQLiteDBTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*, std::string);

/// 移除单个数据库节点
void RmSQLiteDBTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*, std::string);

/// 移除所有节点
void RmAllDBTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*);

/// 遍历Tree的数据库名称
void ErgodicTreeAllDBs(wxTreeCtrl*, std::vector<std::string>&);

/// 获得wxTreeItemId的深度
void GetWxTreeItemIdDeep(wxTreeCtrl*, wxTreeItemId, int&);
int  GetWxTreeItemIdDeep(wxTreeCtrl*, wxTreeItemId);













#endif