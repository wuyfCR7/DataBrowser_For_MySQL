#include "lite_found.h"
#include <fstream>
#include <sstream>

/// SQLite3 DB Close Method
void SQLite3Wrapper::sqlite3_close_db(sqlite3* db_)
{
	if (db_ == NULL) return;
	sqlite3_close(db_);
}

/// SQLite3 DB Connect Method
SQLite3DataBasePtr SQLite3Wrapper::sqlite3_connect_db(std::string db_path_, SQLite3Error& error_)
{
	sqlite3* db_;
	int result_;
	result_ = sqlite3_open(db_path_.c_str(), &db_);
	error_.no_ = result_;
	SQLite3DataBasePtr dbptr_;
	if (result_)
		error_.msg_ = std::string("Can't Open DataBase: ") + sqlite3_errmsg(db_) + "!";
	else
	{
		dbptr_.reset(db_, SQLite3Wrapper::sqlite3_close_db);
		char str_[512];
		sprintf_s(str_, "Opened %s DataBase Successfully!", db_path_.c_str());
		error_.msg_ = str_;
	}
	return dbptr_;
}

/// SQLite3 DB Auto DisConnect Method
void SQLite3Wrapper::sqlite3_disconnect_db(SQLite3DataBasePtr& dbptr_)
{
	if(dbptr_.get() == NULL) return;
	SQLite3Wrapper::sqlite3_close_db(dbptr_.get());
	dbptr_.reset();
}

/// SQLite3 DB Exec SQL Method
SQLite3Error SQLite3Wrapper::sqlite3_exec_db(SQLite3DataBasePtr& dbptr_, std::string sql_, void* userdata_, SQLite3ExecCallback cb_)
{
	SQLite3Error error_;
	if (dbptr_.get() == NULL)
	{
		error_.no_  = -8888;
		error_.msg_ = "SQLite3DataBasePtr Is Empty, Without Connection!";
		return error_;
	}
	char* zErrMsg = NULL;
	int result_ = sqlite3_exec(dbptr_.get(), sql_.c_str(), cb_, userdata_, &zErrMsg);
	error_.no_  = result_;
	char str_[512];
	if (result_)
	{
		sprintf_s(str_, "Operation Done SQL Error: %s!", zErrMsg);
		error_.msg_ = str_;
		sqlite3_free(zErrMsg);
	}
	else if (result_ == SQLITE_OK)
	{
		sprintf_s(str_, "Operation Done Successfully!");
		error_.msg_ = str_;
	}
	return error_;
}

/// SQLite3 DB Print Example CallBack
int SQLite3Wrapper::sqlite3_qry_print_callback(void* userdata_, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		if (argv[i])
			fprintf(stdout, "[%s:%s] ", azColName[i], argv[i]);
		else
			fprintf(stdout, "[%s:%s] ", azColName[i], "NULL");
	}
	fprintf(stdout, "\n");
	return 0;
}

int SQLite3Wrapper::sqlite3_qry_empty_callback(void*, int, char**, char**)
{
	return 0;
} 

/// SQLite3 DB Load From TXT Method
SQLite3Error SQLite3Wrapper::sqlite3_load_txt_table(
	SQLite3DataBasePtr& dbptr_, 
	std::string filepath_, 
	std::string table_, 
	void* userdata_, 
	SQLite3DataType* datatype_,
	int col_amount_,
	SQLite3ExecCallback cb_)
{
	SQLite3Error error_;
	if (dbptr_.get() == NULL)
	{
		error_.no_  = -8888;
		error_.msg_ = "SQLite3DataBasePtr Is Empty, Without Connection!";
		return error_;
	}
	std::ifstream fileio_(filepath_.c_str(), std::ios::in | std::ios::_Nocreate);
	if(fileio_)
	{
		std::string sql_ = "INSERT INTO " + table_ + " VALUES";
		std::string line_buffer_;
		std::istringstream stringtream_buffer_;
		while(!fileio_.eof())
		{
			sql_ += "(";
			std::getline(fileio_, line_buffer_);
			stringtream_buffer_.clear();
			stringtream_buffer_.str(line_buffer_);
			for(int col = 0;col < col_amount_;++col)
			{
				switch(datatype_[col])
				{
				case SQLite3DataType::INTEGER:
					{
						std::string val_;
						stringtream_buffer_ >> val_;
						if(col == (col_amount_ - 1))
							sql_ += val_;
						else
							sql_ += val_ + ",";
					}
					break;
				case SQLite3DataType::REAL:
					{
						std::string val_;
						stringtream_buffer_ >> val_;
						if(col == (col_amount_ - 1))
							sql_ += val_;
						else
							sql_ += val_ + ",";
					}
					break;
				case SQLite3DataType::TEXT:
					{
						std::string val_;
						stringtream_buffer_ >> val_;
						if(col == (col_amount_ - 1))
							sql_ += "'" + val_ + "'";
						else
							sql_ += "'" + val_ + "'" + ",";
					}
					break;
				}
			}
			sql_ += "),";
		}
		fileio_.close();
		sql_.back() = ';';
		return SQLite3Wrapper::sqlite3_exec_db(dbptr_, sql_, userdata_, cb_);
	}
	else
	{
		error_.no_ = -6666;
		error_.msg_ = filepath_ + " File Is Not Exist!";
		return error_;
	}
}