/*
 * MySQL Client Manager
 * By WuyfCR7
*/
#ifndef __CLIENT_MANAGER_FOR_MYSQL_H__
#define __CLIENT_MANAGER_FOR_MYSQL_H__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include "mysql_c_api_foundation.h"


/// MySQLConnectorInfo
struct MySQLConnectorInfo
{
	unsigned int timeout_;
	long timerinterval_;
	std::string host_;
	std::string user_;
	std::string password_;
	unsigned int port_;
	std::string connector_name() const;
};


/// MySQLDataBaseManager
class MySQLDataBaseManager
{
public:
	std::string db_name_;
	std::vector<std::string> table_names_;
	std::vector<std::string> view_names_;
public:
	MySQLDataBaseManager(std::string);
	~MySQLDataBaseManager();
	friend bool operator==(const MySQLDataBaseManager&, const MySQLDataBaseManager&);
	MySQLErrInfo query_tables(MYSQL*);
	MySQLErrInfo query_views(MYSQL*);
};


/// MySQLConnectorManager
class MySQLConnectorManager:public boost::noncopyable
{
private:
	boost::shared_ptr<MySQLFoundationWrapper> connector_;
	MySQLConnectorInfo connector_info_;
public:
	std::string connector_name_;
	std::vector<MySQLDataBaseManager> dbs_;
public:
	MySQLConnectorManager(MySQLConnectorInfo);
	~MySQLConnectorManager();
	MySQLErrInfo start();
	void UpdateDataBaseInfo();
	void end();
	void wait();
	MYSQL* getMySQL();
	friend class MySQLClientManager;
};


/// MySQLClientManager
class MySQLClientManager:public boost::noncopyable
{
private:
	typedef boost::shared_ptr<boost::thread> ThreadPtr;
	std::vector<ThreadPtr> thread_manager_;
public:
	typedef boost::shared_ptr<MySQLConnectorManager> MySQLConnectorManagerPtr;
	std::vector<MySQLConnectorManagerPtr> mysql_conn_manager_;
public:
	MySQLClientManager();
	~MySQLClientManager();
	// Connector Management
	MySQLErrInfo add(unsigned int, long, std::string, std::string, std::string, unsigned int);
	MySQLErrInfo connect(std::string);
	void rm();
	void rm(std::string);
	static void remove_connectorptr_treatment(MySQLConnectorManagerPtr);
	static void remove_connectorptrs_treatment(std::vector<MySQLConnectorManagerPtr>);
	void UpdateDataBaseInfo();
	void UpdateDataBaseInfo(std::string);
	// File IO
	void import_xml(const char*);
	void export_xml(const char*);
	// End
	void end();
	void wait();
	// GetMYSQL
	MYSQL* getMySQL(std::string);
};









#endif