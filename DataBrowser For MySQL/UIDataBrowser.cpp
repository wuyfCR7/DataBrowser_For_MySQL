#include "UIDataBrowser.h"

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

	popupmenu_tree_ = new Fl_Menu_Button(0, 30, 250, 665, "selection");
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
}

void UIDataBrowserForMySQL::show()
{
	if(window_ == NULL){}
	else 
		window_->show();
}

UIDataBrowserForMySQL& UIDataBrowserForMySQL::GetInstance()
{
	return DataBrowserForMySQLSingleton::get_mutable_instance();
}