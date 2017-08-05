/*
 * mysql_c_api_foundation header file is a cplusplus header. 
 * it is to integrate with mysql c api easily
 * it is just a wrapper of mysql c api mysql_real_connection functionalities.
 * By WuyfCR7
 */
#ifndef _MYSQL_C_API_FOUNDATION_H_
#define _MYSQL_C_API_FOUNDATION_H_
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <mysql_c_api_error.h>
#include <boost\bind.hpp>
#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <boost\smart_ptr.hpp>
#include <boost\noncopyable.hpp>



class MySQLFoundationWrapper:public boost::noncopyable{
private:
	/// wait timeout seconds, timer interval seconds
	unsigned int timeout_seconds_;
	long timer_interval_seconds_;

	/// connection information
	std::string  host_;
	std::string  user_;
	std::string  password_;
	unsigned int port_;

	boost::asio::io_service          service_;
	boost::asio::deadline_timer      timer_;
	boost::shared_ptr<boost::thread> ios_running_thread_;
	
	/// mysql
	MYSQL mysql_;
	char reconnect_value_;

	/// connection information
	bool disconnect_;
public:
	bool is_connection_;
private:

	MySQLErrInfo connect(); 

public:
	// timeout_seconds_, timer_interval_seconds_, host_, user_, password_, port_
	MySQLFoundationWrapper(unsigned int, long, std::string, std::string, std::string, unsigned int);
	~MySQLFoundationWrapper();
	
	MySQLErrInfo start();
	void end();
	void wait(); 

	const char*  getHost()     const;
	const char*  getUser()     const;
	const char*  getPassword() const;
	unsigned int getPort()     const;

	MYSQL* getMySQL();
	const MYSQL* getMySQL() const;

	void ping_timer_callback(const boost::system::error_code&);
};













#endif