/*
 * define the mysql_c_api error header
 * By WuyfCR7
*/
#ifndef __MYSQL_C_API_ERROR_H__
#define __MYSQL_C_API_ERROR_H__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <WinSock2.h>
#include <mysql.h>
#include <string>


struct MySQLErrInfo{
	unsigned int error_no_;
	std::string error_info_;
	MySQLErrInfo() :error_no_(0), error_info_(""){}
	~MySQLErrInfo(){}
};







#endif