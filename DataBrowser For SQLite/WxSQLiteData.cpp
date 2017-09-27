#include "WxSQLiteData.h"

SQLite3ClientManager SQLITE_CLIENT_MANAGER;
std::vector<wxColour> COLOR_LIST;
int DEFAULT_ONEPAGE_AMOUNT = 1000;

wxBEGIN_EVENT_TABLE(WxSQLiteData, wxPanel)
wxEND_EVENT_TABLE()

WxSQLiteData::WxSQLiteData(wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, long style) 
	:wxPanel(parent, id, pos, size, style),
	page_(-1)
{
	wxBoxSizer* sizer_;
	sizer_ = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* datasizer_;
	datasizer_ = new wxBoxSizer( wxVERTICAL );
	
	sqlite_data_ = new wxGrid( this, wxID_ANY, wxPoint( 790,600 ), wxDefaultSize, wxHSCROLL | wxVSCROLL);
	sqlite_data_->CreateGrid( 20, 9 );
	sqlite_data_->SetDefaultRowSize(25); 
	sqlite_data_->SetDefaultColSize(88);
	sqlite_data_->SetRowLabelSize(50);
	sqlite_data_->SetLabelFont(wxFontWrapper::GetBoldWxFont(11, "Times New Roman"));
	sqlite_data_->SetLabelBackgroundColour(wxCOLOR_BLUE);
	sqlite_data_->SetLabelTextColour(wxCOLOR_YELLOW);
	sqlite_data_->SetBackgroundColour(wxCOLOR_GRAY);
	sqlite_data_->SetDefaultCellBackgroundColour(wxCOLOR_BLACK);
	sqlite_data_->SetDefaultCellTextColour(wxCOLOR_WHITE);
	sqlite_data_->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTRE);
	datasizer_->Add( sqlite_data_, 0, wxALL, 2 );
	
	
	wxBoxSizer* opersizer_;
	opersizer_ = new wxBoxSizer( wxHORIZONTAL );
	btn_next_ = new wxButton( this, wxID_ANY, wxT("Next"), wxDefaultPosition, wxSize(70 , -1), 0 );
	opersizer_->Add( btn_next_, 0, wxALL, 2 );
	btn_prev_ = new wxButton( this, wxID_ANY, wxT("Prev"), wxDefaultPosition, wxSize(70 , -1), 0 );
	opersizer_->Add( btn_prev_, 0, wxALL, 2 );
	btn_first_ = new wxButton( this, wxID_ANY, wxT("First"), wxDefaultPosition, wxSize(70 , -1), 0 );
	opersizer_->Add( btn_first_, 0, wxALL, 2 );
	sql_edit_ = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400 , -1), wxTE_READONLY);
	opersizer_->Add( sql_edit_, 0, wxALL, 2 );
	current_page_ = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(110 , -1), wxTE_READONLY);
	total_page_ = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(110 , -1), wxTE_READONLY);
	opersizer_->Add( current_page_, 0, wxALL, 2 );
	opersizer_->Add( total_page_, 0, wxALL, 2 );

	sizer_->Add( datasizer_, 11, wxEXPAND, 2 );
	sizer_->Add( opersizer_, 1, wxEXPAND, 2 );
	
	COLOR_LIST.push_back(wxCOLOR_RED);
	COLOR_LIST.push_back(wxCOLOR_ORANGE);
	COLOR_LIST.push_back(wxCOLOR_YELLOW);
	COLOR_LIST.push_back(wxCOLOR_GREEN);
	COLOR_LIST.push_back(wxCOLOR_CYAN);
	COLOR_LIST.push_back(wxCOLOR_MAGENTA);
	COLOR_LIST.push_back(wxCOLOR_WHITE);

	btn_next_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxSQLiteData::OnNextCallBack ), NULL, this );
	btn_prev_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxSQLiteData::OnPrevCallBack ), NULL, this );
	btn_first_->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WxSQLiteData::OnFirstCallBack ), NULL, this );

	this->SetSizer( sizer_ );
	this->Layout();
}

void WxSQLiteData::OnNextCallBack( wxCommandEvent& )
{
	if(db_ptr_.get() == NULL)
		return;
	if(page_ + 1 > all_page_)
		return;
	++page_;
	char buffer_[512];
	sprintf_s(buffer_, "select * from %s limit %d,%d;", tableview_name_.c_str(), 
		DEFAULT_ONEPAGE_AMOUNT * (page_ - 1), DEFAULT_ONEPAGE_AMOUNT);
	std::string sql_ = buffer_;
	GridData griddata_;
	SQLite3Error error_ = SQLite3Wrapper::sqlite3_exec_db(db_ptr_, sql_, &griddata_, QryGridDataCallBack);
	if(error_.no_ == 0)
	{
		sql_edit_->SetValue(sql_.c_str());
		current_page_->SetValue(wxString::Format("Current:%d", page_));
		DataGridPresent(sqlite_data_, griddata_);
	}
}

void WxSQLiteData::OnPrevCallBack( wxCommandEvent& )
{
	if(db_ptr_.get() == NULL)
		return;
	if(page_ - 1 < 1)
		return;
	--page_;
	char buffer_[512];
	sprintf_s(buffer_, "select * from %s limit %d,%d;", tableview_name_.c_str(), 
		DEFAULT_ONEPAGE_AMOUNT * (page_ - 1), DEFAULT_ONEPAGE_AMOUNT);
	std::string sql_ = buffer_;
	GridData griddata_;
	SQLite3Error error_ = SQLite3Wrapper::sqlite3_exec_db(db_ptr_, sql_, &griddata_, QryGridDataCallBack);
	if(error_.no_ == 0)
	{
		sql_edit_->SetValue(sql_.c_str());
		current_page_->SetValue(wxString::Format("Current:%d", page_));
		DataGridPresent(sqlite_data_, griddata_);
	}
}

void WxSQLiteData::OnFirstCallBack( wxCommandEvent& )
{
	if(db_ptr_.get() == NULL)
		return;
	char buffer_[512];
	sprintf_s(buffer_, "select * from %s limit 0,%d;", tableview_name_.c_str(), DEFAULT_ONEPAGE_AMOUNT);
	std::string sql_ = buffer_;
	GridData griddata_;
	SQLite3Error error_ = SQLite3Wrapper::sqlite3_exec_db(db_ptr_, sql_, &griddata_, QryGridDataCallBack);
	if(error_.no_ == 0)
	{
		page_ = 1;
		sql_edit_->SetValue(sql_.c_str());
		current_page_->SetValue("Current:1");
		DataGridPresent(sqlite_data_, griddata_);
	}
}

/// 数据的展示
void DataGridPresent(wxGrid* grid_, GridData& sqlite_query_data_)
{
	// 清除Grid
	int getrow_ = grid_->GetNumberRows();
	int getcol_ = grid_->GetNumberCols();
	int row_ = (int)sqlite_query_data_.data_.size();
	int col_ = (int)sqlite_query_data_.header_.size();
	if(row_ == getrow_ && col_ == getcol_)
	{
		if(row_ == 0)
			return;
	}else
	{
		if (getcol_ > 0)
			grid_->DeleteRows(0, getrow_);
		if (getrow_ > 0)
			grid_->DeleteCols(0, getcol_);
		if(row_ == 0)
			return;
		// 创建Grid
		grid_->InsertCols(0, col_);
		grid_->InsertRows(0, row_);
	}
	getrow_ = row_;
	getcol_ = col_;
	for (int c = 0; c < getcol_; ++c)
		grid_->SetColLabelValue(c, sqlite_query_data_.header_[c]);
	for (int r = 0; r < getrow_; ++r)
	{
		wxString buffer_ = wxString::Format("%d", r);
		grid_->SetRowLabelValue(r, buffer_);
	}
	// 设置单元格的数据

	int color_size_ = (int)COLOR_LIST.size();
	for (int r = 0; r < getrow_; ++r)
	{
		for (int c = 0; c < getcol_; ++c)
		{
			grid_->SetReadOnly(r, c, true);
			grid_->SetCellValue(r, c, sqlite_query_data_.data_[r][c]);
		}
	}
}

/// 数据查询回调函数
int QryGridDataCallBack(void* userdata_, int argc, char** argv, char** azColName)
{
	GridData* griddata_ = (GridData*)userdata_;
	if(griddata_->header_.empty())
	{
		griddata_->header_.resize(argc);
		for (int i = 0; i < argc; i++)
			griddata_->header_[i] = azColName[i];
	}
	griddata_->data_.push_back(std::vector<std::string>());
	griddata_->data_.back().resize(argc);
	for (int i = 0; i < argc; i++)
	{
		if (argv[i])
			griddata_->data_.back().at(i) = argv[i];
		else
			griddata_->data_.back().at(i) = "NULL";
	}
	return 0;
}

/// 页面总行数回调函数
int QryTotalLineCallBack(void* userdata_, int argc, char** argv, char** azColName)
{
	int* line_ = (int*)userdata_;
	*line_ = atoi(argv[0]);
	return 0;
}