#include "Lite3Client.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#pragma comment(lib, "SQLiteLib.lib")

void test_sqlite_foundation();
void test_sqlitedb();
void test_sqliteclient();

int main()
{
	system("pause");
	return NULL;
}

void test_sqliteclient()
{
	SQLite3ClientManager manager_;
	manager_.import_xml("SQLiteConfig.xml");

	manager_.rm("opt");
	manager_.query_fields("UFXDemo.VC");
	auto result_ = manager_.getSQLite3("UFXDemo.VC");
	manager_.rm();
}

void test_sqlitedb() 
{
	std::string dbname1_ = "G:/MYDRIVERSFILE/MyDriversSQLite/Detection/test.db";
	SQLite3DBManager dbm1_(dbname1_);
	dbm1_.connect();
	dbm1_.query_fields();

	dbname1_ = "G:/MYDRIVERSFILE/MyDriversSQLite/Detection/opt2.db";
	SQLite3DBManager dbm2_(dbname1_);
	dbm2_.connect();
	dbm2_.query_fields();

	dbm1_.disconnect();
	dbm2_.disconnect();
}

void test_sqlite_foundation()
{
	/// Query 
	std::string dbname1_ = "G:/MYDRIVERSFILE/MyDriversSQLite/Detection/opt.db";
	SQLite3Error err_;
	SQLite3DataBasePtr ptr_ = SQLite3Wrapper::sqlite3_connect_db(dbname1_, err_);
	std::cout << err_.msg_ << std::endl;
	if (err_.no_ == 0)
	{
		/// Query
		std::string sql_ = "select optcode, cp, k, t, undercode, multiplier from optinfo limit 30, 50;";
		err_ = SQLite3Wrapper::sqlite3_exec_db(ptr_, sql_, NULL, SQLite3Wrapper::sqlite3_qry_print_callback);
		std::cout << err_.msg_ << std::endl;

		sql_ = "select * from sqlite_master;";
		err_ = SQLite3Wrapper::sqlite3_exec_db(ptr_, sql_, NULL, SQLite3Wrapper::sqlite3_qry_print_callback);
		std::cout << err_.msg_ << std::endl;
	}
	SQLite3Wrapper::sqlite3_disconnect_db(ptr_);
	std::cout << std::endl << std::endl;

	/// Load Data Infile
	dbname1_ = "G:/MYDRIVERSFILE/MyDriversSQLite/Detection/opt2.db";
	ptr_ = SQLite3Wrapper::sqlite3_connect_db(dbname1_, err_);
	std::cout << err_.msg_ << std::endl;
	if (err_.no_ == 0)
	{
		std::string filepath_ = "G:/MYDRIVERSFILE/MyDriversSQLite/Detection/OptInfoStd.txt";
		std::string table_    = "OptInfo";
		void* userdata_ = NULL; 
		SQLite3DataType datatype_[7] = {SQLite3DataType::TEXT,
			SQLite3DataType::TEXT, SQLite3DataType::TEXT, 
			SQLite3DataType::REAL, SQLite3DataType::REAL, 
			SQLite3DataType::TEXT, SQLite3DataType::REAL};
		int col_amount_ = 7;
		err_ = SQLite3Wrapper::sqlite3_load_txt_table(ptr_, filepath_, table_, 
			userdata_, datatype_,col_amount_, SQLite3Wrapper::sqlite3_qry_print_callback);
		std::cout << err_.msg_ << std::endl;
	}
}