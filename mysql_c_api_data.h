/*
 * mysql_c_api_data header file is a cplusplus header. 
 * it is to integrate with mysql c api easily
 * it is just a wrapper of mysql c api mysql_real_query functionalities.
 * By WuyfCR7
 */
#ifndef _MYSQL_C_API_DATA_H_
#define _MYSQL_C_API_DATA_H_
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <mysql_c_api_error.h>
#include <vector>
#include <boost\array.hpp>
#include <boost\multi_array.hpp>


class MySQLDataFetchWrapper
{
private:
	/// query
	static MySQLErrInfo query(MYSQL*, std::string);

	/// result operation
	static MySQLErrInfo get_results(MYSQL*, std::vector<std::string>&, boost::multi_array<std::string, 2>&);

public:
	/// Show Databases
	static MySQLErrInfo mysql_fetch_databases(MYSQL*, std::vector<std::string>&);

	/// Show Tables
	static MySQLErrInfo mysql_fetch_tables(MYSQL*, std::string, std::vector<std::string>&);

	/// Show Views
	static MySQLErrInfo mysql_fetch_views(MYSQL*, std::string, std::vector<std::string>&);

	/// Get Table/View Data
	static MySQLErrInfo mysql_fetch_table_data(MYSQL*, std::string, std::string, std::vector<std::string>&, boost::multi_array<std::string, 2>&, int, int);
};









#endif