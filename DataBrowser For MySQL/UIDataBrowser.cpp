#include "UIDataBrowser.h"
#include "UITreeIcon.h"
#include "mysql_c_api_data.h"

Fl_Pixmap PIXMAP_HAS_CHILDREN(ICON_TAG_PARENT);
Fl_Pixmap PIXMAP_NONE_CHILDREN(ICON_TAG_CHILD);
Fl_Pixmap PIXMAP_WINDOW(ICON_WINDOW);

UIDataBrowserForMySQL::UIDataBrowserForMySQL():
	window_(NULL),
	menu_(NULL),
	tree_summary_(NULL),
	popupmenu_tree_(NULL),
	table_browser_(NULL),
	button_first_page_(NULL),
	button_next_page_(NULL),
	button_prev_page_(NULL),
	output_from_text_(NULL),
	output_from_edit_(NULL),
	output_line_text_(NULL),
	output_line_edit_(NULL)
{

}

UIDataBrowserForMySQL::~UIDataBrowserForMySQL()
{

}

void UIDataBrowserForMySQL::OnInitizeDialog()
{
	window_ = new Fl_Window(50, 50, 900, 700, "DataBrowser For MySQL v1.0.0");
	window_->box(FL_UP_BOX);
	window_->begin();

	menu_ = new Fl_Menu_Bar(0, 0, 900, 25);
	menu_->box(FL_UP_BOX);
	menu_->textfont(FL_TIMES_BOLD);

	tree_summary_ = new Fl_Tree(0, 30, 250, 665);
	tree_summary_->box(FL_UP_BOX);
	tree_summary_->color(FL_GRAY);
	tree_summary_->root()->labelfont(FL_TIMES_BOLD);
	tree_summary_->item_labelfont(FL_TIMES_BOLD);
	tree_summary_->when(FL_WHEN_CHANGED);
	tree_summary_->selectmode(FL_TREE_SELECT_SINGLE);

	popupmenu_tree_ = new Fl_Menu_Button(0, 30, 250, 665);
	popupmenu_tree_->box(FL_UP_BOX);
	popupmenu_tree_->type(Fl_Menu_Button::POPUP3);
	popupmenu_tree_->textfont(FL_TIMES_BOLD);

	table_browser_ = new Fl_Output_Table_Row(255, 30, 640, 610);
	table_browser_->setRowWidth(52);
	table_browser_->setColWidth(88);
	table_browser_->col_resize(1);
	table_browser_->setColorSetType(Fl_Output_Table_Row::COL_TYPE);
	window_->begin();

	button_first_page_ = new Fl_Button(255, 640, 100, 25, "FirstPage");
	button_next_page_  = new Fl_Button(360, 640, 100, 25, "NextPage@>");
	button_prev_page_  = new Fl_Button(465, 640, 100, 25, "PrevPage@<");
	button_first_page_->labelfont(FL_TIMES_BOLD);
	button_first_page_->color(FL_BLACK);
	button_first_page_->labelcolor(FL_RED);
	button_next_page_->labelfont(FL_TIMES_BOLD);
	button_next_page_->color(FL_BLACK);
	button_next_page_->labelcolor(FL_RED);
	button_prev_page_->labelfont(FL_TIMES_BOLD);
	button_prev_page_->color(FL_BLACK);
	button_prev_page_->labelcolor(FL_RED);

	output_from_text_ = new Fl_Output(255, 670, 90, 25, "");
	output_from_edit_ = new Fl_Output(350, 670, 70, 25, "");
	output_line_text_ = new Fl_Output(425, 670, 90, 25, "");
	output_line_edit_ = new Fl_Output(520, 670, 70, 25, "");
	output_from_text_->textfont(FL_TIMES_BOLD);
	output_from_edit_->textfont(FL_TIMES_BOLD);
	output_line_text_->textfont(FL_TIMES_BOLD);
	output_line_edit_->textfont(FL_TIMES_BOLD);
	output_from_text_->value("FROM");
	output_from_edit_->value("NULL");
	output_line_text_->value("LINE");
	output_line_edit_->value("NULL");
	output_from_text_->color(FL_BLACK);
	output_from_edit_->color(FL_BLACK);
	output_line_text_->color(FL_BLACK);
	output_line_edit_->color(FL_BLACK);
	output_from_text_->textcolor(FL_WHITE);
	output_line_text_->textcolor(FL_WHITE);
	output_from_edit_->textcolor(FL_YELLOW);
	output_line_edit_->textcolor(FL_YELLOW);

	window_->end();
	window_->resizable(window_);

	UIConnectorInfo::GetInstance().mysql_manager_ptr_ = &mysql_manager_;
	UIConnectorInfo::GetInstance().OnInitizeDialog();
}

void UIDataBrowserForMySQL::OnInitizeCallBack()
{
	static Fl_Menu menu_button_[] = {
		{"Connector Operation", NULL, NULL, NULL, FL_SUBMENU},
			{"ADD One Conn", NULL, add_one_connector, this},
			{NULL},
		{NULL}
	};
	menu_->menu(menu_button_);

	popupmenu_tree_->add("REFRESH All Conns" , NULL , update_all_connectors , this);
	popupmenu_tree_->add("REFRESH This Conn" , NULL , update_this_connector , this);
	popupmenu_tree_->add("REMOVE All Conns"  , NULL , remove_all_connectors , this);
	popupmenu_tree_->add("REMOVE This Conn"  , NULL , remove_this_connector , this);
	popupmenu_tree_->add("CONNECT This Conn" , NULL , connect_this_connector, this);
	popupmenu_tree_->add("QUERY This Data"   , NULL , query_this_data       , this);

	button_first_page_->callback(query_this_data    , this);
	button_next_page_->callback(dataquery_next_page , this);
	button_prev_page_->callback(dataquery_prev_page , this);

	UIConnectorInfo::GetInstance().button_return_->callback(connect_info_return, this);
	UIConnectorInfo::GetInstance().button_cancel_->callback(connect_info_cancel, this);
}

void UIDataBrowserForMySQL::show()
{
	static Fl_RGB_Image IMAGE_WINDOW(&PIXMAP_WINDOW, FL_WHITE);
	if(window_ == NULL){}
	else
	{
		window_->default_icon(&IMAGE_WINDOW);
		window_->show();
	} 		
}

void UIDataBrowserForMySQL::join()
{
	mysql_manager_.end();
	auto iter_ = thread_manager_.begin();
	while(iter_ != thread_manager_.end())
	{
		if(iter_->get()== NULL){}
		else
			(*iter_)->join();
		++iter_;
	}
	mysql_manager_.wait();
}

UIDataBrowserForMySQL& UIDataBrowserForMySQL::GetInstance()
{
	return DataBrowserForMySQLSingleton::get_mutable_instance();
}

void UIDataBrowserForMySQL::add_one_connector(Fl_Widget*, void*)
{
	UIConnectorInfo::GetInstance().show();
}

void UIDataBrowserForMySQL::update_all_connectors(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	tool_update_tree_all_items(ui_, true);
}

void UIDataBrowserForMySQL::update_this_connector(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	Fl_Tree_Item* item_ = ui_->tree_summary_->first_selected_item();
	if(item_ == NULL){}
	else
	{
		if(item_->parent() == ui_->tree_summary_->root())
		{
			std::string connector_name_ = item_->label();
			tool_update_tree_one_item(ui_, connector_name_, true);
		}
	}
}

void UIDataBrowserForMySQL::remove_all_connectors(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	ui_->mysql_manager_.rm();
	tool_update_tree_all_items(ui_, false);
}

void UIDataBrowserForMySQL::remove_this_connector(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	Fl_Tree_Item* item_ = ui_->tree_summary_->first_selected_item();
	if(item_ == NULL){}
	else
	{
		if(item_->parent() == ui_->tree_summary_->root())
		{
			std::string connector_name_ = item_->label();
			ui_->mysql_manager_.rm(connector_name_);
			tool_update_tree_all_items(ui_, false);
		}
	}
}

void UIDataBrowserForMySQL::connect_this_connector(Fl_Widget*, void* v)
{
	static std::string error_str_;
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	Fl_Tree_Item* item_ = ui_->tree_summary_->first_selected_item();
	if(item_ == NULL){}
	else
	{
		if(item_->parent() == ui_->tree_summary_->root())
		{
			std::string connector_name_ = item_->label();
			MySQLErrInfo error_ = ui_->mysql_manager_.connect(connector_name_);
			if(error_.error_no_ == 0)
				tool_update_tree_one_item(ui_, connector_name_, true);
			else
			{
				error_str_ = std::to_string(error_.error_no_) + ":" + error_.error_info_;
				fl_alert(error_str_.c_str());
			}
		}
	}
}

void UIDataBrowserForMySQL::query_this_data(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	MYSQL* mysql_ = NULL;
	std::string connector_name_;
	std::string db_name_;
	std::string data_name_;
	bool res_ = tool_database_info_fromtree(ui_, &mysql_, connector_name_, db_name_, data_name_);
	if(res_)
	{
		int start_ = 0;
		int line_  = 500;
		tool_database_show_table(mysql_, connector_name_, db_name_, data_name_, start_, line_, ui_);
	}
}

void UIDataBrowserForMySQL::dataquery_prev_page(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	MYSQL* mysql_ = NULL;
	std::string connector_name_;
	std::string db_name_;
	std::string data_name_;
	bool res_ = tool_database_info_fromtree(ui_, &mysql_, connector_name_, db_name_, data_name_);
	if(res_)
	{
		int start_ = atoi(ui_->output_from_edit_->value());
		int line_  = atoi(ui_->output_line_edit_->value());
		if(start_ == 0)
		{
			line_ = 500;
			tool_database_show_table(mysql_, connector_name_, db_name_, data_name_, start_, line_, ui_);
		}
		else
		{
			start_ -= 500;
			line_ = 500;
			tool_database_show_table(mysql_, connector_name_, db_name_, data_name_, start_, line_, ui_);
		}
	}
}

void UIDataBrowserForMySQL::dataquery_next_page(Fl_Widget*, void* v)
{
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	MYSQL* mysql_ = NULL;
	std::string connector_name_;
	std::string db_name_;
	std::string data_name_;
	bool res_ = tool_database_info_fromtree(ui_, &mysql_, connector_name_, db_name_, data_name_);
	if(res_)
	{
		int start_ = atoi(ui_->output_from_edit_->value());
		int line_  = atoi(ui_->output_line_edit_->value());
		start_ += line_;
		tool_database_show_table(mysql_, connector_name_, db_name_, data_name_, start_, line_, ui_);
	}
}

void UIDataBrowserForMySQL::connect_info_return(Fl_Widget*, void* v)
{
	static std::string error_str_;
	UIDataBrowserForMySQL* ui_ = (UIDataBrowserForMySQL*)v;
	UIConnectorInfo::GetInstance().hide();
	MySQLConnectorInfo connector_info_;
	connector_info_.timeout_  = 5;
	connector_info_.timerinterval_ = 120;
	connector_info_.host_     = UIConnectorInfo::GetInstance().host_;
	connector_info_.user_     = UIConnectorInfo::GetInstance().user_;
	connector_info_.password_ = UIConnectorInfo::GetInstance().password_;
	connector_info_.port_     = UIConnectorInfo::GetInstance().port_;
	
	// add one connector
	MySQLErrInfo error_ = ui_->mysql_manager_.add(connector_info_.timeout_, connector_info_.timerinterval_, 
		connector_info_.host_, connector_info_.user_, connector_info_.password_, connector_info_.port_);
	if(error_.error_no_ == 0)
	{
		Fl_Tree_Item* it_ = tool_add_one_connector(ui_, ui_->mysql_manager_.mysql_conn_manager_.back());
		tool_renew_tree_item_icons(it_);
		tool_close_tree_item_child(it_);
	}
	else
	{
		error_str_ = std::to_string(error_.error_no_) + ":" + error_.error_info_;
		fl_alert(error_str_.c_str());
	}
}

void UIDataBrowserForMySQL::connect_info_cancel(Fl_Widget*, void*)
{
	UIConnectorInfo::GetInstance().leave();
}

void tool_renew_tree_all_icons(Fl_Tree* tree_)
{
	for (Fl_Tree_Item* item_ = tree_->first(); item_; item_ = tree_->next(item_))
	{
		if (item_->has_children())
			item_->usericon(&PIXMAP_HAS_CHILDREN);
		else
			item_->usericon(&PIXMAP_NONE_CHILDREN);
	}
}

void tool_renew_tree_item_icons(Fl_Tree_Item* item_)
{
	if(item_->has_children())
	{
		item_->usericon(&PIXMAP_HAS_CHILDREN);
		int n_child_ = item_->children();
		for (int t = 0; t < n_child_; ++t)
		{
			Fl_Tree_Item* it_ = item_->child(t);
			tool_renew_tree_item_icons(it_);
		}
	}
	else
		item_->usericon(&PIXMAP_NONE_CHILDREN);
}

Fl_Tree_Item* tool_add_one_connector(UIDataBrowserForMySQL* ui_, MySQLClientManager::MySQLConnectorManagerPtr connector_)
{
	if(connector_.get() == NULL)
		return NULL;
	else
	{
		Fl_Tree* tree_ = ui_->tree_summary_;

		// 1 depth
		Fl_Tree_Item* item_conn_ = tree_->add(connector_->connector_name_.c_str());
		item_conn_->labelfgcolor(FL_DARK_RED);
		auto db_iter_ = connector_->dbs_.begin();
		auto db_end_  = connector_->dbs_.end();

		while(db_iter_ != db_end_)
		{
			// 2 depth
			Fl_Tree_Item* item_db_ = tree_->add(item_conn_, db_iter_->db_name_.c_str());
			item_db_->labelfgcolor(FL_DARK_BLUE);

			// 3 depth
			Fl_Tree_Item* item_table_ = tree_->add(item_db_, "TABLE");
			Fl_Tree_Item* item_tree_  = tree_->add(item_db_, "TREE");
			item_table_->labelfgcolor(FL_DARK_MAGENTA);
			item_tree_->labelfgcolor(FL_DARK_MAGENTA);

			// TABLE
			auto table_iter_ = db_iter_->table_names_.begin();
			auto table_end_  = db_iter_->table_names_.end();
			while(table_iter_ != table_end_)
			{
				// 4 depth
				tree_->add(item_table_, table_iter_->c_str())->labelfgcolor(Fl_Color(98));
				++table_iter_;
			}

			// VIEW
			auto view_iter_ = db_iter_->view_names_.begin();
			auto view_end_  = db_iter_->view_names_.end();
			while(view_iter_ != view_end_)
			{
				// 4 depth
				tree_->add(item_tree_, view_iter_->c_str())->labelfgcolor(Fl_Color(98));
				++view_iter_;
			}

			++db_iter_;
		}
		return item_conn_;
	}
}

void tool_update_tree_all_items(UIDataBrowserForMySQL* ui_, bool update_db_info_)
{
	Fl_Tree* tree_ = ui_->tree_summary_;
	tree_->clear();
	auto iter_ = ui_->mysql_manager_.mysql_conn_manager_.begin();
	auto end_  = ui_->mysql_manager_.mysql_conn_manager_.end();
	if(update_db_info_)
		ui_->mysql_manager_.UpdateDataBaseInfo();
	while(iter_ != end_)
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		tool_add_one_connector(ui_, (*iter_));
		++iter_;
	}
	tool_renew_tree_all_icons(tree_);
	tool_close_tree_all_child(tree_);
	tree_->redraw();
}

void tool_update_tree_one_item(UIDataBrowserForMySQL* ui_, std::string connector_name_, bool update_db_info_)
{
	Fl_Tree* tree_ = ui_->tree_summary_;
	Fl_Tree_Item* item_ = tree_->find_item(connector_name_.c_str());
	if(item_ == NULL){}
	else
	{
		tree_->remove(item_);
		auto iter_ = ui_->mysql_manager_.mysql_conn_manager_.begin();
	    auto end_  = ui_->mysql_manager_.mysql_conn_manager_.end();
		while(iter_ != end_)
		{
			if((*iter_).get() == NULL)
			{
				++iter_;
				continue;
			}
			if((*iter_)->connector_name_ == connector_name_)
			{
				if(update_db_info_)
					(*iter_)->UpdateDataBaseInfo();
				Fl_Tree_Item* it_ = tool_add_one_connector(ui_, (*iter_));
				tool_renew_tree_item_icons(it_);
				tool_close_tree_item_child(it_);
				break;
			}
			++iter_;
		}
	}
	tree_->redraw();
}

void tool_close_tree_all_child(Fl_Tree* tree_)
{
	for (Fl_Tree_Item* item_ = tree_->first(); item_; item_ = tree_->next(item_)){
		if (item_->has_children()){
			item_->close();
		}
	}
}

void tool_close_tree_item_child(Fl_Tree_Item* item_)
{
	if(item_->has_children())
	{
		item_->close();
		int n_child_ = item_->children();
		for (int t = 0; t < n_child_; ++t)
		{
			Fl_Tree_Item* it_ = item_->child(t);
			tool_close_tree_item_child(it_);
		}
	}
}

bool tool_database_info_fromtree(UIDataBrowserForMySQL* ui_, MYSQL** mysql_, std::string& connector_name_, std::string& db_name_, std::string& data_name_)
{
	Fl_Tree_Item* item_ = ui_->tree_summary_->first_selected_item();
	if(item_ == NULL)
		return false;
	else
	{
		Fl_Tree_Item* parent_item_ = item_->parent();
		if(parent_item_ == NULL)
			return false;
		std::string parent_label_ = parent_item_->label();
		if(parent_label_ == "TREE" || parent_label_ == "TABLE")
		{
			Fl_Tree_Item* db_item_   = parent_item_->parent();
			Fl_Tree_Item* conn_item_ = db_item_->parent();
			connector_name_ = conn_item_->label();
			db_name_        = db_item_->label();
			data_name_      = item_->label();
			(*mysql_) = ui_->mysql_manager_.getMySQL(connector_name_);
		}
		else
			return false;
	}
	return true;
}

void tool_database_show_table(MYSQL* mysql_, std::string connector_name_, std::string db_name_, std::string data_name_, 
	int start_, int line_, UIDataBrowserForMySQL* ui_)
{
	static std::string str_;
	std::vector<std::string> col_name_;
	std::vector<std::string> row_name_;
	boost::multi_array<std::string, 2> data_;
	MySQLErrInfo error_	= MySQLDataFetchWrapper::mysql_fetch_table_data(mysql_, db_name_, data_name_, col_name_, data_, start_, line_);
	if(error_.error_no_ == 0)
	{
		int row_amount_ = data_.shape()[0];
		row_name_.resize(row_amount_);
		for(int t = 0;t < row_amount_;++t)
			row_name_[t] = std::to_string(t + start_ + 1);
		ui_->table_browser_->setData(data_, col_name_, row_name_);

		str_ = std::to_string(start_);
		ui_->output_from_edit_->value(str_.c_str());
		str_ = std::to_string(line_);
		ui_->output_line_edit_->value(str_.c_str());
	}
	else
	{
		str_ = std::to_string(error_.error_no_) + ":" + error_.error_info_;
		fl_alert(str_.c_str());
	}
}