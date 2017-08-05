#include "mysql_c_api_foundation.h"
#include <boost\date_time\posix_time\posix_time.hpp>

MySQLFoundationWrapper::MySQLFoundationWrapper(
	unsigned int timeout__, 
	long timerinterval__,
	std::string host__,
	std::string user__,
	std::string password__,
	unsigned int port__):
	timeout_seconds_(timeout__),
	timer_interval_seconds_(timerinterval__),
	host_(host__), 
	user_(user__), 
	password_(password__), 
	port_(port__),
	timer_(service_, boost::posix_time::seconds(timer_interval_seconds_)),
	reconnect_value_(1),
	disconnect_(false),
	is_connection_(false)
{
}

const char*  MySQLFoundationWrapper::getHost() const
{
	return host_.c_str();
}

const char*  MySQLFoundationWrapper::getUser() const
{
	return user_.c_str();
}

const char*  MySQLFoundationWrapper::getPassword() const
{
	return password_.c_str();
}

unsigned int MySQLFoundationWrapper::getPort() const
{
	return port_;
}

MYSQL* MySQLFoundationWrapper::getMySQL()
{
	return const_cast<MYSQL*>(static_cast<const MySQLFoundationWrapper*>(this)->getMySQL());
}

const MYSQL* MySQLFoundationWrapper::getMySQL() const
{
	return static_cast<const MYSQL*>(&mysql_);
}

MySQLErrInfo MySQLFoundationWrapper::connect()
{
	MySQLErrInfo err_;
	mysql_init(&mysql_);
	int ret = mysql_options(&mysql_, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&timeout_seconds_);
	if (ret){
		err_.error_no_   = mysql_errno(&mysql_);
		err_.error_info_ = mysql_error(&mysql_);
		return err_;
	}
	MYSQL* sql_ = mysql_real_connect(&mysql_, host_.c_str(), user_.c_str(), password_.c_str(), "", port_, NULL, NULL);
	if (sql_ == NULL){
		err_.error_no_   = mysql_errno(&mysql_);
		err_.error_info_ = mysql_error(&mysql_);
	}
	else{
		mysql_options(&mysql_, MYSQL_OPT_RECONNECT, &reconnect_value_);
	}
	return err_;
}

MySQLErrInfo MySQLFoundationWrapper::start()
{
	MySQLErrInfo err_ = connect();
	if(err_.error_no_ == 0)
	{
		timer_.async_wait(boost::bind(&MySQLFoundationWrapper::ping_timer_callback, this, boost::asio::placeholders::error));
		if(ios_running_thread_.get() == NULL){
			ios_running_thread_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &service_)));
		}
		is_connection_ = true;
	}
	return err_;
}

void MySQLFoundationWrapper::ping_timer_callback(const boost::system::error_code& error_code_)
{
	if(disconnect_){}
	else
	{
		mysql_thread_init();
		int ping_ = mysql_ping(&mysql_);
		if(ping_ == 0){}
		else{
			is_connection_ = false;
			MySQLErrInfo err_ = connect();
			if(err_.error_no_ == 0)
				is_connection_ = true;
		}
		mysql_thread_end();
		timer_.expires_from_now(boost::posix_time::seconds(timer_interval_seconds_));
		timer_.async_wait(boost::bind(&MySQLFoundationWrapper::ping_timer_callback, this, boost::asio::placeholders::error));
	}
}

void MySQLFoundationWrapper::end()
{
	disconnect_ = true;
}

void MySQLFoundationWrapper::wait()
{
	if(ios_running_thread_.get() == NULL){}
	else{
		ios_running_thread_->join();
		ios_running_thread_.reset();
		mysql_close(&mysql_);
	}
}

MySQLFoundationWrapper::~MySQLFoundationWrapper()
{
	mysql_close(&mysql_);
}