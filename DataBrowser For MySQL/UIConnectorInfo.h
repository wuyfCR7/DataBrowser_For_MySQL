/*
 * DataBrowser Connector Info For MySQL UI
 * By WuyfCR7
*/
#ifndef __UI_CONNECTOR_INFO_MYSQL_H__
#define __UI_CONNECTOR_INFO_MYSQL_H__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include "MySQLClientManager.h"
#include <FL\Fl_Window.H>
#include <FL\Fl_Button.H>
#include <FL\Fl_Input.H>
#include <FL\Fl_Int_Input.H>
#include <string>
#include <boost\noncopyable.hpp>
#include <boost\serialization\singleton.hpp>


class UIConnectorInfo:public boost::noncopyable
{
	typedef boost::serialization::singleton<UIConnectorInfo> ConnectorInfoSingleton;
public:
	std::string host_;
	std::string user_;
	std::string password_;
	unsigned int port_;
	MySQLClientManager* mysql_manager_ptr_;
public:
	Fl_Window*    window_;
	Fl_Button*    button_return_;
	Fl_Button*    button_cancel_;
	Fl_Input*     input_host_;
	Fl_Input*     input_user_;
	Fl_Input*     input_password_;
	Fl_Int_Input* input_port_;
public:
	UIConnectorInfo();
	~UIConnectorInfo();

	void OnInitizeDialog();
	void show();
	void hide();
	void leave();

	static UIConnectorInfo& GetInstance();
};





#endif