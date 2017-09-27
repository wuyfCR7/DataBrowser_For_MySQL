#include "WxSQLiteFrame.h"
/*-------*/
#include "xpm/header.xpm"
#include "xpm/dbadd.xpm"
#include "xpm/dbrm.xpm"
#include "xpm/dbrmall.xpm"
/*-------*/
#include "xpm/table.xpm"
#include "xpm/view.xpm"
#include "xpm/trigger.xpm"
#include "xpm/index.xpm"
/*-------*/
#include "xpm/tr_db.xpm"
#include "xpm/tr_table.xpm"
#include "xpm/tr_view.xpm"
#include "xpm/tr_trigger.xpm"
#include "xpm/tr_index.xpm"
#include "xpm/user.xpm"

#include <algorithm>
#include <boost/algorithm/string.hpp>

wxIcon tree_header_ico(user_xpm);
wxIcon tree_db_ico(tr_db_xpm);
wxIcon tree_table_ico(tr_table_xpm);
wxIcon tree_view_ico(tr_view_xpm);
wxIcon tree_trigger_ico(tr_trigger_xpm);
wxIcon tree_index_ico(tr_index_xpm);

wxBEGIN_EVENT_TABLE(WxSQLiteUIFrame, wxFrame)
wxEND_EVENT_TABLE()

WxSQLiteUIFrame::WxSQLiteUIFrame(wxWindow* parent__, wxWindowID id__, const wxString& str__) :
	wxFrame(NULL, wxID_ANY, str__, wxPoint(100, 100), wxSize(1167, 708), wxFrameStyle::wxMODAL_STYLE),
	sqlite_config_file_("SQLiteConfig.xml"),
	imagelist_(16, 16)
{
	wxIcon ico_(header_xpm);
	this->SetIcon(ico_);
	this->SetFont(wxFontWrapper::GetBoldWxFont(11, "Times New Roman"));

	// MenuBar
	menubar_ = new wxMenuBar( 0 );
	menu_file_ = new wxMenu();
	wxMenuItem* m_file_newdatabse_;
	m_file_newdatabse_ = new wxMenuItem( menu_file_, wxID_ANY, wxString( wxT("New DataBase") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file_->Append( m_file_newdatabse_ );
	
	wxMenuItem* m_file_droponedb_;
	m_file_droponedb_ = new wxMenuItem( menu_file_, wxID_ANY, wxString( wxT("Drop One DataBase") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file_->Append( m_file_droponedb_ );
	
	wxMenuItem* m_file_dropalldb_;
	m_file_dropalldb_ = new wxMenuItem( menu_file_, wxID_ANY, wxString( wxT("Drop All DataBase") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file_->Append( m_file_dropalldb_ );
	
	wxMenuItem* m_file_exit_;
	m_file_exit_ = new wxMenuItem( menu_file_, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file_->Append( m_file_exit_ );

	menu_database_ = new wxMenu();
	wxMenuItem* m_db_table_;
	m_db_table_ = new wxMenuItem( menu_database_, wxID_ANY, wxString( wxT("Table") ) , wxEmptyString, wxITEM_NORMAL );
	menu_database_->Append( m_db_table_ );

	wxMenuItem* m_db_view_;
	m_db_view_ = new wxMenuItem( menu_database_, wxID_ANY, wxString( wxT("View") ) , wxEmptyString, wxITEM_NORMAL );
	menu_database_->Append( m_db_view_ );
	
	wxMenuItem* m_db_trigger_;
	m_db_trigger_ = new wxMenuItem( menu_database_, wxID_ANY, wxString( wxT("Trigger") ) , wxEmptyString, wxITEM_NORMAL );
	menu_database_->Append( m_db_trigger_ );
	
	wxMenuItem* m_db_index_;
	m_db_index_ = new wxMenuItem( menu_database_, wxID_ANY, wxString( wxT("Index") ) , wxEmptyString, wxITEM_NORMAL );
	menu_database_->Append( m_db_index_ );
	
	menubar_->Append( menu_file_, wxT("File") ); 
	menubar_->Append( menu_database_, wxT("DataBase") ); 
	this->SetMenuBar( menubar_ );
	
	// ToolBar
	toolbar_ = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 

	wxToolBarToolBase* tb_newdatabse_ = toolbar_->AddTool(wxID_ANY, "", wxIcon(dbadd_xpm), "New DataBase");
	wxToolBarToolBase* tb_droponedb_  = toolbar_->AddTool(wxID_ANY, "", wxIcon(dbrm_xpm), "Drop One DataBase");
	wxToolBarToolBase* tb_dropalldb_  = toolbar_->AddTool(wxID_ANY, "", wxIcon(dbrmall_xpm), "Drop All DataBase");

	wxToolBarToolBase* tb_table_ = toolbar_->AddTool(wxID_ANY, "", wxIcon(table_xpm), "Table");
	wxToolBarToolBase* tb_view_  = toolbar_->AddTool(wxID_ANY, "", wxIcon(view_xpm), "View");
	wxToolBarToolBase* tb_trigger_ = toolbar_->AddTool(wxID_ANY, "", wxIcon(trigger_xpm), "Trigger");
	wxToolBarToolBase* tb_index_ = toolbar_->AddTool(wxID_ANY, "", wxIcon(index_xpm), "Index");

	wxToolBarToolBase* tb_exit_ = toolbar_->AddTool(wxID_ANY, "", wxArtProvider::GetBitmap(wxART_QUIT, wxART_TOOLBAR), "Exit");
	toolbar_->Realize(); 

	// Layout
	wxBoxSizer* box_sizer_;
	box_sizer_ = new wxBoxSizer( wxHORIZONTAL );
	
	sqlite3_tree_ = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 300,700 ), wxTR_DEFAULT_STYLE | wxTR_SINGLE );
	box_sizer_->Add( sqlite3_tree_, 0, wxALL, 1 );
	sqlite3_tree_->SetBackgroundColour(wxColour(189, 252, 201));

	imagelist_.Add(tree_header_ico);
	imagelist_.Add(tree_db_ico);
	imagelist_.Add(tree_table_ico);
	imagelist_.Add(tree_view_ico);
	imagelist_.Add(tree_trigger_ico);
	imagelist_.Add(tree_index_ico);
	
	sqlite_infobook_ = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 800,700 ), wxNB_TOP );
	box_sizer_->Add( sqlite_infobook_, 1, wxEXPAND | wxALL, 1 );
	sqlite_infobook_->SetBackgroundColour(wxColor(255, 235, 205));
	data_page_ = new WxSQLiteData(sqlite_infobook_);
	auto edit_property_ = wxTE_MULTILINE | wxTE_LEFT | wxHSCROLL | wxTE_READONLY;
	sql_edit_ = new wxTextCtrl(sqlite_infobook_, wxID_ANY, "", wxDefaultPosition, wxSize(700, 600), edit_property_);
	sql_edit_->SetForegroundColour(wxCOLOR_WHITE);
	sql_edit_->SetBackgroundColour(wxCOLOR_BLACK);

	sqlite_infobook_->AddPage(data_page_, "DATA");
	sqlite_infobook_->AddPage(sql_edit_, "SQL");

	// Popup
	popup_ = new wxMenu();
	wxMenuItem* m_query_;
	m_query_ = new wxMenuItem( popup_, wxID_ANY, wxString( wxT("Query") ) , wxEmptyString, wxITEM_NORMAL );
	popup_->Append( m_query_ );
	
	wxMenuItem* m_refresh_;
	m_refresh_ = new wxMenuItem( popup_, wxID_ANY, wxString( wxT("Refresh") ) , wxEmptyString, wxITEM_NORMAL );
	popup_->Append( m_refresh_ );

	wxMenuItem* m_refreshall_;
	m_refreshall_ = new wxMenuItem( popup_, wxID_ANY, wxString( wxT("RefreshAll") ) , wxEmptyString, wxITEM_NORMAL );
	popup_->Append( m_refreshall_ );
	
	// Connect
	sqlite3_tree_->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( WxSQLiteUIFrame::OnSQLiteTreeRightClick ), NULL, this );

	this->Connect( m_file_newdatabse_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnNewDataBase ) );
	this->Connect( m_file_droponedb_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnRmOneDataBase ) );
	this->Connect( m_file_dropalldb_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnRmAllDataBase ) );
	this->Connect( m_file_exit_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnCloseWindow ) );
	this->Connect( m_db_table_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnTableShow ) );
	this->Connect( m_db_view_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnViewShow ) );
	this->Connect( m_db_trigger_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnTriggerShow ) );
	this->Connect( m_db_index_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnIndexShow ) );

	this->Connect( tb_newdatabse_->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(WxSQLiteUIFrame::OnNewDataBase), NULL, this);
	this->Connect( tb_droponedb_ ->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnRmOneDataBase ) );
	this->Connect( tb_dropalldb_ ->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnRmAllDataBase ) );
	this->Connect( tb_exit_->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnCloseWindow ) );
	this->Connect( tb_table_ ->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnTableShow ) );
	this->Connect( tb_view_ ->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnViewShow ) );
	this->Connect( tb_trigger_ ->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnTriggerShow ) );
	this->Connect( tb_index_ ->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( WxSQLiteUIFrame::OnIndexShow ) );

	wxCloseWrapper::AppCloseCallBack(this, &WxSQLiteUIFrame::OnCloseWindow);

	this->Connect( m_query_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnDataQuery ) );
	this->Connect( m_refresh_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnDataBaseRefresh ) );
	this->Connect( m_refreshall_->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WxSQLiteUIFrame::OnDataBaseRefreshAll ) );

	// SQLite3
	SQLITE_CLIENT_MANAGER.import_xml(sqlite_config_file_.c_str());
	InitSQLiteClientTreeCtrl(SQLITE_CLIENT_MANAGER, sqlite3_tree_, &imagelist_);

	this->SetSizer( box_sizer_ );
	this->Layout();
	this->Centre( wxBOTH );
}

void WxSQLiteUIFrame::OnSQLiteTreeRightClick( wxTreeEvent& event_)
{
	this->PopupMenu(popup_, event_.GetPoint());
}

void WxSQLiteUIFrame::OnNewDataBase( wxCommandEvent&  )
{
	wxFileDialog* open_ = new wxFileDialog(this, "Open SQLite Database file", "", "",
		"SQLite Database files(*.db)|*.db", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (open_->ShowModal() == wxID_OK)
	{
		std::string pathname_ = open_->GetPath();
		std::vector<std::string> tmp_str_;
		boost::split(tmp_str_, pathname_, boost::is_any_of("/\\"), boost::token_compress_on);
		bool result_ = SQLITE_CLIENT_MANAGER.add(tmp_str_.back(), pathname_);
		if(result_)
			AddSQLiteDBTreeCtrl(SQLITE_CLIENT_MANAGER,  sqlite3_tree_, SQLITE_CLIENT_MANAGER.db_names_.back());
	}
	open_->Destroy();
}

void WxSQLiteUIFrame::OnRmOneDataBase( wxCommandEvent& )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	if(sqlite3_tree_->GetRootItem() == sqlite3_tree_->GetItemParent(select_id_))
	{
		std::string dbname_ = sqlite3_tree_->GetItemText(select_id_);
		RmSQLiteDBTreeCtrl(SQLITE_CLIENT_MANAGER, sqlite3_tree_, dbname_);
	}
}

void WxSQLiteUIFrame::OnRmAllDataBase( wxCommandEvent& )
{
	RmAllDBTreeCtrl(SQLITE_CLIENT_MANAGER, sqlite3_tree_);
}

void WxSQLiteUIFrame::OnCloseWindow( wxCommandEvent& )
{
	SQLITE_CLIENT_MANAGER.export_xml(sqlite_config_file_.c_str());
	Destroy();
}

void WxSQLiteUIFrame::OnTableShow( wxCommandEvent&  )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	if(sqlite3_tree_->GetRootItem() == sqlite3_tree_->GetItemParent(select_id_))
	{
		wxString dbname_ = sqlite3_tree_->GetItemText(select_id_);
		auto dbmanager_ = SQLITE_CLIENT_MANAGER.getSQLite3(std::string(dbname_.c_str()));
		if(dbmanager_.get() == NULL)
		{
		}
		else
		{
			GridData grid_data_;
			grid_data_.header_.push_back("Table");
			std::string sql_;
			for(auto& clientdata_:dbmanager_->table_datas_)
			{
				grid_data_.data_.push_back(std::vector<std::string>(1, clientdata_.name_));
				if(clientdata_.sql_.empty())
				{
				}else
					sql_ += clientdata_.sql_ + "\n";
			}
			DataGridPresent(data_page_->sqlite_data_, grid_data_);
			sql_edit_->SetValue(sql_);
		}
	}
}

void WxSQLiteUIFrame::OnViewShow( wxCommandEvent& )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	if(sqlite3_tree_->GetRootItem() == sqlite3_tree_->GetItemParent(select_id_))
	{
		wxString dbname_ = sqlite3_tree_->GetItemText(select_id_);
		auto dbmanager_ = SQLITE_CLIENT_MANAGER.getSQLite3(std::string(dbname_.c_str()));
		if(dbmanager_.get() == NULL)
		{
		}
		else
		{
			GridData grid_data_;
			grid_data_.header_.push_back("View");
			std::string sql_;
			for(auto& clientdata_:dbmanager_->view_datas_)
			{
				grid_data_.data_.push_back(std::vector<std::string>(1, clientdata_.name_));
				if(clientdata_.sql_.empty())
				{
				}else
					sql_ += clientdata_.sql_ + "\n";
			}
			DataGridPresent(data_page_->sqlite_data_, grid_data_);
			sql_edit_->SetValue(sql_);
		}
	}
}

void WxSQLiteUIFrame::OnTriggerShow( wxCommandEvent& )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	if(sqlite3_tree_->GetRootItem() == sqlite3_tree_->GetItemParent(select_id_))
	{
		wxString dbname_ = sqlite3_tree_->GetItemText(select_id_);
		auto dbmanager_ = SQLITE_CLIENT_MANAGER.getSQLite3(std::string(dbname_.c_str()));
		if(dbmanager_.get() == NULL)
		{
		}
		else
		{
			GridData grid_data_;
			grid_data_.header_.push_back("Trigger");
			std::string sql_;
			for(auto& clientdata_:dbmanager_->trigger_datas_)
			{
				grid_data_.data_.push_back(std::vector<std::string>(1, clientdata_.name_));
				if(clientdata_.sql_.empty())
				{
				}else
					sql_ += clientdata_.sql_ + "\n";
			}
			DataGridPresent(data_page_->sqlite_data_, grid_data_);
			sql_edit_->SetValue(sql_);
		}
	}
}

void WxSQLiteUIFrame::OnIndexShow( wxCommandEvent& )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	if(sqlite3_tree_->GetRootItem() == sqlite3_tree_->GetItemParent(select_id_))
	{
		wxString dbname_ = sqlite3_tree_->GetItemText(select_id_);
		auto dbmanager_ = SQLITE_CLIENT_MANAGER.getSQLite3(std::string(dbname_.c_str()));
		if(dbmanager_.get() == NULL)
		{
		}
		else
		{
			GridData grid_data_;
			grid_data_.header_.push_back("Index");
			std::string sql_;
			for(auto& clientdata_:dbmanager_->index_datas_)
			{
				grid_data_.data_.push_back(std::vector<std::string>(1, clientdata_.name_));
				if(clientdata_.sql_.empty())
				{
				}else
					sql_ += clientdata_.sql_ + "\n";
			}
			DataGridPresent(data_page_->sqlite_data_, grid_data_);
			sql_edit_->SetValue(sql_);
		}
	}
}

void WxSQLiteUIFrame::OnDataQuery( wxCommandEvent& )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	int deep_ = GetWxTreeItemIdDeep(sqlite3_tree_, select_id_);
	if(deep_ == 4)
	{
		wxTreeItemId parent_id_ = sqlite3_tree_->GetItemParent(select_id_);
		wxString type_ = sqlite3_tree_->GetItemText(parent_id_);
		if(type_ == "table" || type_ == "view")
		{
			wxTreeItemId db_id_ = sqlite3_tree_->GetItemParent(parent_id_);
			wxString dbname_ = sqlite3_tree_->GetItemText(db_id_);
			auto db_managerptr_ = SQLITE_CLIENT_MANAGER.getSQLite3(std::string(dbname_.c_str()));
			auto& SQLitePtr_ = db_managerptr_->dbptr_;

			std::string tbviname_ = sqlite3_tree_->GetItemText(select_id_);
			char buffer_[512];
			sprintf_s(buffer_, "select * from %s limit 0,%d;", tbviname_.c_str(), DEFAULT_ONEPAGE_AMOUNT);
			std::string sql_ = buffer_;

			// 数据查询
			GridData griddata_;
			SQLite3Error error_ = SQLite3Wrapper::sqlite3_exec_db(SQLitePtr_, sql_, &griddata_, QryGridDataCallBack);
			if(error_.no_ == 0)
			{
				if(griddata_.data_.empty())
				{
					data_page_->page_ = -1;
					data_page_->db_ptr_.reset();
					data_page_->tableview_name_.clear();
					wxString str_ = wxString::Format("%s Data Is Empty", tbviname_.c_str());
					wxLogMessage(str_);
					return;
				}
				
				data_page_->sql_edit_->SetValue(sql_.c_str());
				data_page_->page_   = 1;
				data_page_->db_ptr_ = SQLitePtr_;
				data_page_->tableview_name_ = tbviname_;

				// 查询总行数
				int totalline_ = 0;
				sprintf_s(buffer_, "select count(*) from %s;", tbviname_.c_str());
				sql_ = buffer_;
				SQLite3Wrapper::sqlite3_exec_db(SQLitePtr_, sql_, &totalline_, QryTotalLineCallBack);

				data_page_->current_page_->SetValue("Current:1");
				int all_page_;
				if(totalline_%DEFAULT_ONEPAGE_AMOUNT)
					all_page_ = totalline_/DEFAULT_ONEPAGE_AMOUNT + 1;
				else
					all_page_ = totalline_/DEFAULT_ONEPAGE_AMOUNT;
				data_page_->all_page_ = all_page_;
				data_page_->total_page_->SetValue(wxString::Format("Total:%d", all_page_));

				DataGridPresent(data_page_->sqlite_data_, griddata_);
			}
			else
			{
				wxString str_ = wxString::Format("%d: %s", error_.no_, error_.msg_.c_str());
				wxLogMessage(str_);
				return;
			}
		}
	}
}

void WxSQLiteUIFrame::OnDataBaseRefresh( wxCommandEvent& )
{
	wxTreeItemId select_id_ = sqlite3_tree_->GetSelection();
	if(sqlite3_tree_->GetRootItem() == sqlite3_tree_->GetItemParent(select_id_))
	{
		wxString dbname_ = sqlite3_tree_->GetItemText(select_id_);
		std::string str_dbname_ = std::string(dbname_.c_str());
		auto dbmanager_ = SQLITE_CLIENT_MANAGER.getSQLite3(str_dbname_);
		if(dbmanager_.get() == NULL)
		{
		}
		else
		{
			dbmanager_->query_fields();
			UpdateTreeCtrlSQLiteOneDBInfo(SQLITE_CLIENT_MANAGER, sqlite3_tree_, str_dbname_);
		}
	}
}

void WxSQLiteUIFrame::OnDataBaseRefreshAll( wxCommandEvent& )
{
	if(SQLITE_CLIENT_MANAGER.db_names_.empty())
		return;
	for(auto& dbname_:SQLITE_CLIENT_MANAGER.db_names_)
		SQLITE_CLIENT_MANAGER.query_fields(dbname_);
	UpdateTreeCtrlSQLiteAllInfo(SQLITE_CLIENT_MANAGER, sqlite3_tree_);
}

/// 更新TreeCtrl所有的信息
void UpdateTreeCtrlSQLiteAllInfo(SQLite3ClientManager& sqlite3_client_, wxTreeCtrl* tree_)
{
	tree_->DeleteAllItems();
	wxTreeItemId root_item_ = tree_->AddRoot("SQLite3", 0, 0);
	for(auto& dbname_:sqlite3_client_.db_names_)
		AddSQLiteDBTreeCtrl(sqlite3_client_, tree_, dbname_);
}

/// 更新TreeCtrl单个数据库的信息
void UpdateTreeCtrlSQLiteOneDBInfo(SQLite3ClientManager& sqlite3_client_, wxTreeCtrl* tree_, std::string dbname_)
{
	std::vector<std::string> dbnames_;
	ErgodicTreeAllDBs(tree_, dbnames_);
	// dbname_必须在tree_内
	bool condition1_ = std::find(dbnames_.begin(), dbnames_.end(), dbname_) != dbnames_.end();
	// dbname_必须在sqlite3_client_内的db_names_内
	auto& dbnames_list_ = sqlite3_client_.db_names_;
	bool condition2_ = std::find(dbnames_list_.begin(), dbnames_list_.end(), dbname_) != dbnames_list_.end();
	if(condition1_ && condition2_)
	{
		// 删除
		wxTreeItemId root_ = tree_->GetRootItem();
		if(tree_->ItemHasChildren(root_))
		{
			wxTreeItemIdValue cookie_;
			wxTreeItemId id_(NULL);
			for (wxTreeItemId child_ = tree_->GetFirstChild(root_, cookie_); child_ != id_; child_ = tree_->GetNextChild(root_, cookie_))
			{
				wxString tmpstr_ = tree_->GetItemText(child_);
				if(tmpstr_.c_str() == dbname_)
				{
					tree_->DeleteChildren(child_);
					tree_->Delete(child_);
					break;
				}
			}

			// 添加
			AddSQLiteDBTreeCtrl(sqlite3_client_, tree_, dbname_);
		}
	}
}

/// 初始化SQLite客户端
void InitSQLiteClientTreeCtrl(SQLite3ClientManager& sqlite3_client_, wxTreeCtrl* tree_, wxImageList* imagelist_)
{
	tree_->SetImageList(imagelist_);
	UpdateTreeCtrlSQLiteAllInfo(sqlite3_client_, tree_);
}

/// 添加单个数据库节点
void AddSQLiteDBTreeCtrl(SQLite3ClientManager& sqlite3_client_, wxTreeCtrl* tree_, std::string dbname_)
{
	std::vector<std::string> dbnames_;
	ErgodicTreeAllDBs(tree_, dbnames_);
	// dbname_必须不在tree_内
	bool condition1_ = std::find(dbnames_.begin(), dbnames_.end(), dbname_) == dbnames_.end();
	// dbname_必须在sqlite3_client_内的db_names_内
	auto& dbnames_list_ = sqlite3_client_.db_names_;
	bool condition2_ = std::find(dbnames_list_.begin(), dbnames_list_.end(), dbname_) != dbnames_list_.end();
	if(condition1_ && condition2_)
	{
		wxTreeItemId root_    = tree_->GetRootItem();
		wxTreeItemId dbnode_  = tree_->AppendItem(root_, dbname_  , 1, 1);
		wxTreeItemId table_   = tree_->AppendItem(dbnode_, "table"  , 2, 2);
		wxTreeItemId view_    = tree_->AppendItem(dbnode_, "view"   , 3, 3);
		wxTreeItemId trigger_ = tree_->AppendItem(dbnode_, "trigger", 4, 4);
		wxTreeItemId index_   = tree_->AppendItem(dbnode_, "index"  , 5, 5);
		auto dbmanager_ = sqlite3_client_.getSQLite3(dbname_);
		for(auto& clientdata_:dbmanager_->table_datas_)
			tree_->AppendItem(table_, clientdata_.name_, 2, 2);
		for(auto& clientdata_:dbmanager_->view_datas_)
			tree_->AppendItem(view_, clientdata_.name_, 3, 3);
		for(auto& clientdata_:dbmanager_->trigger_datas_)
			tree_->AppendItem(trigger_, clientdata_.name_, 4, 4);
		for(auto& clientdata_:dbmanager_->index_datas_)
			tree_->AppendItem(index_, clientdata_.name_, 5, 5);
		tree_->Collapse(dbnode_);
	}
}

/// 移除单个数据库节点
void RmSQLiteDBTreeCtrl(SQLite3ClientManager& sqlite3_client_, wxTreeCtrl* tree_, std::string dbname_)
{
	bool succ_ = sqlite3_client_.rm(dbname_);
	if(succ_)
	{
		wxTreeItemId root_ = tree_->GetRootItem();
		if(tree_->ItemHasChildren(root_))
		{
			wxTreeItemIdValue cookie_;
			wxTreeItemId id_(NULL);
			for (wxTreeItemId child_ = tree_->GetFirstChild(root_, cookie_); child_ != id_; child_ = tree_->GetNextChild(root_, cookie_))
			{
				wxString tmpstr_ = tree_->GetItemText(child_);
				if(dbname_ == tmpstr_.c_str())
				{
					tree_->DeleteChildren(child_);
					tree_->Delete(child_);
					return;
				}
			}
		}
	}
}

/// 移除所有节点
void RmAllDBTreeCtrl(SQLite3ClientManager& sqlite3_client_, wxTreeCtrl* tree_)
{
	sqlite3_client_.rm();
	tree_->DeleteAllItems();
	tree_->AddRoot("SQLite3", 0, 0);
}

/// 遍历Tree的数据库名称
void ErgodicTreeAllDBs(wxTreeCtrl* tree_, std::vector<std::string>& strs_)
{
	strs_.clear();
	wxTreeItemId root_ = tree_->GetRootItem();
	if(tree_->ItemHasChildren(root_))
	{
		wxTreeItemIdValue cookie_;
		wxTreeItemId id_(NULL);
		for (wxTreeItemId child_ = tree_->GetFirstChild(root_, cookie_); child_ != id_; child_ = tree_->GetNextChild(root_, cookie_))
		{
			wxString tmpstr_ = tree_->GetItemText(child_);
			strs_.push_back(std::string(tmpstr_.c_str()));
		}
	}
}

/// 获得wxTreeItemId的深度
void GetWxTreeItemIdDeep(wxTreeCtrl* tree_, wxTreeItemId id_, int& deep_)
{
	if(id_ == tree_->GetRootItem())
	{
		deep_ += 1;
		return;
	}
	wxTreeItemId id_parent_ = tree_->GetItemParent(id_);
	deep_ += 1;
	GetWxTreeItemIdDeep(tree_, id_parent_, deep_);
}

int GetWxTreeItemIdDeep(wxTreeCtrl* tree_, wxTreeItemId id_)
{
	int deep_ = 0;
	GetWxTreeItemIdDeep(tree_, id_, deep_);
	return deep_;
}