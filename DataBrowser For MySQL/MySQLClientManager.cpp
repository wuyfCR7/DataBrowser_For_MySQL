#include "MySQLClientManager.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>
#include "mysql_c_api_data.h"

/// MySQLConnectorInfo
std::string MySQLConnectorInfo::connector_name() const
{
	std::string connector_name_ = host_ + std::string("|") + user_ + std::string("|") + std::to_string(port_);
	return connector_name_;
}


/// MySQLDataBaseManager
MySQLDataBaseManager::MySQLDataBaseManager(std::string dbname__):db_name_(dbname__)
{

}

MySQLDataBaseManager::~MySQLDataBaseManager()
{

}

bool operator==(const MySQLDataBaseManager& left_, const MySQLDataBaseManager& right_)
{
	return left_.db_name_ == right_.db_name_;
}

MySQLErrInfo MySQLDataBaseManager::query_tables(MYSQL* mysql_)
{
	MySQLErrInfo error_ = MySQLDataFetchWrapper::mysql_fetch_tables(mysql_, db_name_, table_names_);
	if(error_.error_no_ == 0){}
	else
		table_names_.clear();
	return error_;
}

MySQLErrInfo MySQLDataBaseManager::query_views(MYSQL* mysql_)
{
	MySQLErrInfo error_ = MySQLDataFetchWrapper::mysql_fetch_views(mysql_, db_name_, view_names_);
	if(error_.error_no_ == 0){}
	else
		view_names_.clear();
	return error_;
}


/// MySQLConnectorManager
MySQLConnectorManager::MySQLConnectorManager(MySQLConnectorInfo info_)
{
	connector_info_ = info_;
	connector_name_ = info_.connector_name();
	connector_.reset();
}

MySQLConnectorManager::~MySQLConnectorManager()
{

}

MySQLErrInfo MySQLConnectorManager::start()
{
	MySQLErrInfo error_;
	if(connector_.get() == NULL)
	{
		connector_.reset(new MySQLFoundationWrapper(
			connector_info_.timeout_, 
			connector_info_.timerinterval_, 
			connector_info_.host_, 
			connector_info_.user_, 
			connector_info_.password_, 
			connector_info_.port_));
		error_ = connector_->start();
	}else
	{
		if(connector_->is_connection_){}
		else
			error_ = connector_->start();
	}
	if(error_.error_no_ == 0){}
	else
		connector_.reset();
	return error_;
}

void MySQLConnectorManager::end()
{
	if(connector_.get() == NULL){}
	else
	{
		if(connector_->is_connection_)
			connector_->end();
	}
}

void MySQLConnectorManager::wait()
{
	if(connector_.get() == NULL){}
	else
	{
		if(connector_->is_connection_)
		{
			connector_->wait();
			connector_.reset();
		}
	}
}

void MySQLConnectorManager::UpdateDataBaseInfo()
{
	if(connector_.get() == NULL){}
	else
	{
		if(connector_->is_connection_)
		{
			std::vector<std::string> dbnamess_;
			MySQLErrInfo error_ = MySQLDataFetchWrapper::mysql_fetch_databases(connector_->getMySQL(), dbnamess_);
			if(error_.error_no_ == 0)
			{
				dbs_.clear();
				auto iter_ = dbnamess_.begin();
				while(iter_ != dbnamess_.end())
				{
					dbs_.push_back(MySQLDataBaseManager(*iter_));
					dbs_.back().query_tables(connector_->getMySQL());
					dbs_.back().query_views(connector_->getMySQL());
					++iter_;
				}
			}
			else
				dbs_.clear();
		}
	}
}

MYSQL* MySQLConnectorManager::getMySQL()
{
	if(connector_.get() == NULL){}
	else
	{
		if(connector_->is_connection_)
			return connector_->getMySQL();
	}
	return NULL;
}


/// MySQLClientManager
MySQLClientManager::MySQLClientManager()
{

}

MySQLClientManager::~MySQLClientManager()
{

}

MySQLErrInfo MySQLClientManager::add(
	unsigned int timeout_,
	long timerinterval_,
	std::string host_,
	std::string user_,
	std::string password_,
	unsigned int port_
	)
{
	MySQLErrInfo error_;
	MySQLConnectorInfo connector_info_;
	connector_info_.timeout_  = timeout_;
	connector_info_.timerinterval_ = timerinterval_;
	connector_info_.host_     = host_;
	connector_info_.user_     = user_;
	connector_info_.password_ = password_;
	connector_info_.port_     = port_;
	std::string connector_name_ = connector_info_.connector_name();

	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		if((*iter_)->connector_name_ == connector_name_)
		{
			error_.error_info_ = "Connector Already Exist";
			error_.error_no_ = 10088;
			return error_;
		}
		++iter_;
	}
	MySQLConnectorManagerPtr connector_ptr_(new MySQLConnectorManager(connector_info_));
	error_ = connector_ptr_->start();
	if(error_.error_no_ == 0)
	{
		connector_ptr_->UpdateDataBaseInfo();
		mysql_conn_manager_.push_back(connector_ptr_);
	}
	return error_;
}

MySQLErrInfo MySQLClientManager::connect(std::string connector_name_)
{
	MySQLErrInfo error_;
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		if((*iter_)->connector_name_ == connector_name_)
		{
			error_ = (*iter_)->start();
			if(error_.error_no_ == 0)
				(*iter_)->UpdateDataBaseInfo();
			break;
		}
		++iter_;
	}
	return error_;
}

void MySQLClientManager::rm()
{
	if(mysql_conn_manager_.empty())
		return;
	ThreadPtr thread_(new boost::thread(remove_connectorptrs_treatment, mysql_conn_manager_));
	thread_manager_.push_back(thread_);
	mysql_conn_manager_.clear();
}

void MySQLClientManager::remove_connectorptrs_treatment(std::vector<MySQLConnectorManagerPtr> mysql_conn_manager_)
{
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		(*iter_)->end();
		++iter_;
	}
	iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		(*iter_)->wait();
		++iter_;
	}
}

void MySQLClientManager::rm(std::string connector_name_)
{
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		if((*iter_)->connector_name_ == connector_name_)
		{
			ThreadPtr thread_(new boost::thread(remove_connectorptr_treatment, *iter_));
			thread_manager_.push_back(thread_);
			mysql_conn_manager_.erase(iter_);
			break;
		}
		++iter_;
	}
}

void MySQLClientManager::remove_connectorptr_treatment(MySQLConnectorManagerPtr connector_ptr_)
{
	connector_ptr_->end();
	connector_ptr_->wait();
}

void MySQLClientManager::end()
{
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		(*iter_)->end();
		++iter_;
	}
}

void MySQLClientManager::wait()
{
	auto thread_iter_ = thread_manager_.begin();
	while(thread_iter_ != thread_manager_.end())
	{
		(*thread_iter_)->join();
		++thread_iter_;
	}
	auto conn_iter_ = mysql_conn_manager_.begin();
	while(conn_iter_ != mysql_conn_manager_.end())
	{
		if((*conn_iter_).get() == NULL)
		{
			++conn_iter_;
			continue;
		}
		(*conn_iter_)->wait();
		++conn_iter_;
	}
}

void MySQLClientManager::UpdateDataBaseInfo(std::string connector_name_)
{
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		if((*iter_)->connector_name_ == connector_name_)
		{
			(*iter_)->UpdateDataBaseInfo();
			break;
		}
		++iter_;
	}
}

void MySQLClientManager::UpdateDataBaseInfo()
{
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		(*iter_)->UpdateDataBaseInfo();
		++iter_;
	}
}

MYSQL* MySQLClientManager::getMySQL(std::string connector_name_)
{
	MYSQL* mysql_ = NULL;
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		if((*iter_)->connector_name_ == connector_name_)
		{
			mysql_ = (*iter_)->getMySQL();
			return mysql_;
		}
		++iter_;
	}
	return NULL;
}

void MySQLClientManager::export_xml(const char* filename_)
{
	if(mysql_conn_manager_.empty())
		return;
	std::string xml_file_ = filename_;
	if(boost::filesystem::exists(xml_file_))
		boost::filesystem::remove(xml_file_);
	boost::property_tree::ptree pt;
	boost::property_tree::ptree unit_pt;
	auto iter_ = mysql_conn_manager_.begin();
	while(iter_ != mysql_conn_manager_.end())
	{
		if((*iter_).get() == NULL)
		{
			++iter_;
			continue;
		}
		boost::property_tree::ptree child_pt;
		child_pt.put("host_",     (*iter_)->connector_info_.host_);
		child_pt.put("user_",     (*iter_)->connector_info_.user_);
		child_pt.put("password_", (*iter_)->connector_info_.password_);
		child_pt.put("port_",     (*iter_)->connector_info_.port_);
		unit_pt.add_child("Connector", child_pt);
		++iter_;
	}
	pt.put_child("Client", unit_pt);
	boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
	boost::property_tree::write_xml(xml_file_.c_str(), pt, std::locale(), settings);
}

void MySQLClientManager::import_xml(const char* filename_)
{
	std::string xml_file_ = filename_;
	if(boost::filesystem::exists(xml_file_))
	{
		boost::property_tree::ptree pt_;
		boost::property_tree::read_xml(xml_file_, pt_);
		boost::property_tree::ptree connectors_tree_ = pt_.get_child("Client");
		auto iter_ = connectors_tree_.begin();
		while(iter_ != connectors_tree_.end())
		{
			unsigned int timeout_ = 5;
			long timerinterval_ = 120;
			std::string host_ = iter_->second.get<std::string>("host_");
			std::string user_ = iter_->second.get<std::string>("user_");
			std::string password_ = iter_->second.get<std::string>("password_");
			unsigned int port_    = iter_->second.get<unsigned int>("port_");
			MySQLConnectorInfo connector_info_;
			connector_info_.timeout_  = timeout_;
			connector_info_.timerinterval_ = timerinterval_;
			connector_info_.host_     = host_;
			connector_info_.user_     = user_;
			connector_info_.password_ = password_;
			connector_info_.port_     = port_;
			MySQLConnectorManagerPtr connector_(new MySQLConnectorManager(connector_info_));
			mysql_conn_manager_.push_back(connector_);
			++iter_;
		}
	}
}