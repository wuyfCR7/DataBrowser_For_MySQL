#ifndef __CLIENT_MANAGER_FOR_SQLITE_H__
#define __CLIENT_MANAGER_FOR_SQLITE_H__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include "lite_found.h"
#include <set>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

/// SQLite3DBType
typedef enum
{
	TABLE,
	VIEW,
	TRIGGER,
	INDEX
}SQLite3DBType;


/// SQLite3ClientData
struct SQLite3ClientData
{
	SQLite3DBType type_;
	std::string name_;
	std::string tbl_name_;
	std::string sql_;
	SQLite3ClientData();
	SQLite3ClientData(SQLite3DBType);
};


/// SQLite3DBManager
class SQLite3DBManager:boost::noncopyable
{
public:
	std::string dbpath_;
	SQLite3DataBasePtr dbptr_;
	std::list<SQLite3ClientData> table_datas_;
	std::list<SQLite3ClientData> view_datas_;
	std::list<SQLite3ClientData> trigger_datas_;
	std::list<SQLite3ClientData> index_datas_;
public:
	SQLite3DBManager(std::string);
	SQLite3Error query_fields();
private:
	static int query_fields_callback(void*, int, char**, char**);
public:
	SQLite3Error connect();
	void disconnect();
};


/// SQLite3ClientManager
class SQLite3ClientManager:boost::noncopyable
{
public:
	std::list<std::string> db_names_;
	std::list<std::string> db_paths_;
	std::list<boost::shared_ptr<SQLite3DBManager>> db_managers_;
public:
	bool add(std::string, std::string);
	bool rm(std::string);
	void rm();
	void query_fields(std::string);
	boost::shared_ptr<SQLite3DBManager> getSQLite3(std::string);
	void import_xml(const char*);
	void export_xml(const char*);
};












#endif