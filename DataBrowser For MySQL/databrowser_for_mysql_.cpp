/*
 * This is Data Browser For MySQL main program 
 * By WuyfCR7
*/
#include "UIDataBrowser.h"
#include <iostream>
#include <FL\Fl.H>
#pragma comment(lib, "fltk.lib")
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libboost_thread-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_date_time-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_system-vc110-mt-1_58.lib")
int main()
{
	UIDataBrowserForMySQL::GetInstance().OnInitizeDialog();
	UIDataBrowserForMySQL::GetInstance().OnInitizeCallBack();
	UIDataBrowserForMySQL::GetInstance().show();

	const char* filename_ = "config.xml";
	UIDataBrowserForMySQL::GetInstance().mysql_manager_.import_xml(filename_);
	tool_update_tree_all_items(&UIDataBrowserForMySQL::GetInstance(), false);

	Fl::run();

	UIDataBrowserForMySQL::GetInstance().mysql_manager_.export_xml(filename_);
	UIDataBrowserForMySQL::GetInstance().join();

	return NULL;
}