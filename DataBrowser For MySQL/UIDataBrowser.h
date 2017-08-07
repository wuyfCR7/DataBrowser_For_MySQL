/*
 * DataBrowser For MySQL UI
 * By WuyfCR7
*/
#ifndef __UI_DATA_BROWSER_MYSQL_H__
#define __UI_DATA_BROWSER_MYSQL_H__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <boost\serialization\singleton.hpp>
#include <boost\noncopyable.hpp>
#include <FL\Fl_Tree.H>
#include <FL\fl_message.H>
#include <FL\Fl_Window.H>
#include <FL\Fl_Button.H>
#include <FL\Fl_Output.H>
#include <FL\Fl_Menu_Bar.H>
#include <FL\Fl_Menu_Button.H>
#include "FLTKOutputRowTable.h"


class UIDataBrowserForMySQL:public boost::noncopyable
{
private:
	typedef boost::serialization::singleton<UIDataBrowserForMySQL> DataBrowserForMySQLSingleton;
private:
	Fl_Window*           window_;
	Fl_Menu_Bar*         menu_;
	Fl_Tree*             tree_summary_;
	Fl_Menu_Button*      popupmenu_tree_;
	Fl_Output_Table_Row* table_browser_;
	Fl_Button*           button_first_page_;
	Fl_Button*           button_next_page_;
	Fl_Button*           button_prev_page_;
	Fl_Output*           output_from_text_;
	Fl_Output*           output_from_edit_;
	Fl_Output*           output_line_text_;
	Fl_Output*           output_line_edit_;
public:
	UIDataBrowserForMySQL();
	~UIDataBrowserForMySQL();

	void OnInitizeDialog();
	void show();

	static UIDataBrowserForMySQL& GetInstance();
};




#endif