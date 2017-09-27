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

/// ����TreeCtrl���е���Ϣ
void UpdateTreeCtrlSQLiteAllInfo(SQLite3ClientManager&, wxTreeCtrl*);

/// ����TreeCtrl�������ݿ����Ϣ
void UpdateTreeCtrlSQLiteOneDBInfo(SQLite3ClientManager&, wxTreeCtrl*, std::string);

/// ��ʼ��SQLite�ͻ���
void InitSQLiteClientTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*, wxImageList*);

/// ��ӵ������ݿ�ڵ�
void AddSQLiteDBTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*, std::string);

/// �Ƴ��������ݿ�ڵ�
void RmSQLiteDBTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*, std::string);

/// �Ƴ����нڵ�
void RmAllDBTreeCtrl(SQLite3ClientManager&, wxTreeCtrl*);

/// ����Tree�����ݿ�����
void ErgodicTreeAllDBs(wxTreeCtrl*, std::vector<std::string>&);

/// ���wxTreeItemId�����
void GetWxTreeItemIdDeep(wxTreeCtrl*, wxTreeItemId, int&);
int  GetWxTreeItemIdDeep(wxTreeCtrl*, wxTreeItemId);













#endif