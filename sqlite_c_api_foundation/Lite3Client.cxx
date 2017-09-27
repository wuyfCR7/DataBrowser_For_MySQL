#include "Lite3Client.h"
#include <io.h>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/// SQLite3ClientData
SQLite3ClientData::SQLite3ClientData()
{
}

SQLite3ClientData::SQLite3ClientData(SQLite3DBType ty_):
	type_(ty_)
{
}


/// SQLite3DBManager
SQLite3DBManager::SQLite3DBManager(std::string database_path_):
	dbpath_(database_path_)
{
}

SQLite3Error SQLite3DBManager::connect()
{
	SQLite3Error err_;
	if(dbptr_.get() == NULL)
	{
		dbptr_ = SQLite3Wrapper::sqlite3_connect_db(dbpath_, err_);
	}
	return err_;
}

void SQLite3DBManager::disconnect()
{
	if(dbptr_.get() == NULL)
		return;
	SQLite3Wrapper::sqlite3_disconnect_db(dbptr_);
}

SQLite3Error SQLite3DBManager::query_fields()
{
	SQLite3Error err_;
	if(dbptr_.get() == NULL)
	{
		err_.no_ = 6666;
		return err_;
	}
	table_datas_.clear();
	view_datas_.clear();
	trigger_datas_.clear();
	index_datas_.clear();
	std::string sql_ = "select * from sqlite_master;";
	return SQLite3Wrapper::sqlite3_exec_db(dbptr_, sql_, this, SQLite3DBManager::query_fields_callback);
}

int SQLite3DBManager::query_fields_callback(void* userdata_, int argc, char** argv, char** azColName)
{
	SQLite3DBManager* db_manager_ = (SQLite3DBManager*)userdata_;
	if(std::string(argv[0]) == "table")
	{
		db_manager_->table_datas_.push_back(SQLite3ClientData(SQLite3DBType::TABLE));
		db_manager_->table_datas_.back().name_     = argv[1] ? argv[1] : "";
		db_manager_->table_datas_.back().tbl_name_ = argv[2] ? argv[2] : "";
		db_manager_->table_datas_.back().sql_      = argv[4] ? argv[4] : "";
	}
	else if(std::string(argv[0]) == "view")
	{
		db_manager_->view_datas_.push_back(SQLite3ClientData(SQLite3DBType::VIEW));
		db_manager_->view_datas_.back().name_     = argv[1] ? argv[1] : "";
		db_manager_->view_datas_.back().tbl_name_ = argv[2] ? argv[2] : "";
		db_manager_->view_datas_.back().sql_      = argv[4] ? argv[4] : "";
	}
	else if(std::string(argv[0]) == "trigger")
	{
		db_manager_->trigger_datas_.push_back(SQLite3ClientData(SQLite3DBType::TRIGGER));
		db_manager_->trigger_datas_.back().name_     = argv[1] ? argv[1] : "";
		db_manager_->trigger_datas_.back().tbl_name_ = argv[2] ? argv[2] : "";
		db_manager_->trigger_datas_.back().sql_      = argv[4] ? argv[4] : "";
	}
	else if(std::string(argv[0]) == "index")
	{
		db_manager_->index_datas_.push_back(SQLite3ClientData(SQLite3DBType::INDEX));
		db_manager_->index_datas_.back().name_     = argv[1] ? argv[1] : "";
		db_manager_->index_datas_.back().tbl_name_ = argv[2] ? argv[2] : "";
		db_manager_->index_datas_.back().sql_      = argv[4] ? argv[4] : "";
	}
	return 0;
}


/// SQLite3ClientManager
bool SQLite3ClientManager::add(std::string dbname_, std::string dbpath_)
{
	if(std::find(db_paths_.begin(), db_paths_.end(), dbpath_) == db_paths_.end())
	{
		boost::shared_ptr<SQLite3DBManager> db_manager_ptr_(new SQLite3DBManager(dbpath_));
		auto err_ = db_manager_ptr_->connect();
		if(err_.no_ == 0)
		{
			db_manager_ptr_->query_fields();
			db_paths_.push_back(dbpath_);
			db_managers_.push_back(db_manager_ptr_);

			boost::iterator_range<std::string::iterator> rge_;
			rge_ = boost::ifind_last(dbname_, ".db");
			if(rge_.begin() != dbname_.end())
			{
				auto pos_ = std::distance(dbname_.begin(), rge_.begin());
				dbname_.erase(std::size_t(pos_));
			}
			while(std::find(db_names_.begin(), db_names_.end(), dbname_) != db_names_.end())
			{
				dbname_ += "I";
			}
			db_names_.push_back(dbname_);
			return true;
		}
	}
	return false;
}

bool SQLite3ClientManager::rm(std::string dbname_)
{
	auto iter_ = std::find(db_names_.begin(), db_names_.end(), dbname_);
	if(iter_ != db_names_.end())
	{
		auto distance_ = std::distance(db_names_.begin(), iter_);
		auto manager_iter_ = db_managers_.begin();
		std::advance(manager_iter_, distance_);
		(*manager_iter_)->disconnect();
		db_managers_.erase(manager_iter_);

		db_names_.erase(iter_);
		auto path_iter_ = db_paths_.begin();
		std::advance(path_iter_, distance_);
		db_paths_.erase(path_iter_);
		return true;
	}
	return false;
}

void SQLite3ClientManager::rm()
{
	db_names_.clear();
	db_paths_.clear();
	for(auto& one_manager_:db_managers_)
	{
		one_manager_->disconnect();
	}
	db_managers_.clear();
}

void SQLite3ClientManager::query_fields(std::string dbname_)
{
	auto iter_ = std::find(db_names_.begin(), db_names_.end(), dbname_);
	if(iter_ != db_names_.end())
	{
		auto distance_ = std::distance(db_names_.begin(), iter_);
		auto manager_iter_ = db_managers_.begin();
		std::advance(manager_iter_, distance_);
		(*manager_iter_)->query_fields();
	}
}

boost::shared_ptr<SQLite3DBManager> SQLite3ClientManager::getSQLite3(std::string dbname_)
{
	auto iter_ = std::find(db_names_.begin(), db_names_.end(), dbname_);
	if(iter_ != db_names_.end())
	{
		auto distance_ = std::distance(db_names_.begin(), iter_);
		auto manager_iter_ = db_managers_.begin();
		std::advance(manager_iter_, distance_);
		return *manager_iter_;
	}
	return boost::shared_ptr<SQLite3DBManager>();
}

void SQLite3ClientManager::import_xml(const char* filename_)
{
	if(_access(filename_, 0) == 0)
	{
		boost::property_tree::ptree pt_;
		boost::property_tree::read_xml(filename_, pt_);
		boost::property_tree::ptree connectors_tree_ = pt_.get_child("SQLite3Config");
		auto iter_ = connectors_tree_.begin();
		std::vector<std::string> buf_;
		while (iter_ != connectors_tree_.end())
		{
			buf_.push_back(iter_->second.get_value<std::string>());
			++iter_;
		}
		std::vector<std::string> tmp_str_;
		for(auto& str_:buf_)
		{
			tmp_str_.clear();
			boost::split(tmp_str_, str_, boost::is_any_of("/\\"), boost::token_compress_on);
			add(tmp_str_.back(), str_);
		}
	}
}

void SQLite3ClientManager::export_xml(const char* filename_)
{
	if(db_paths_.empty())
		return;
	boost::property_tree::ptree ptch_;
	for(auto& onepath_:db_paths_)
		ptch_.add("DBPath", onepath_);
	boost::property_tree::ptree pt;
	pt.add_child("SQLite3Config", ptch_);
	boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
	boost::property_tree::write_xml(filename_, pt, std::locale(), settings);
}