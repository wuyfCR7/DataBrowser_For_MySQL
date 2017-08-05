#include "mysql_c_api_foundation.h"
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libboost_thread-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_date_time-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_system-vc110-mt-1_58.lib")
MySQLErrInfo query(MYSQL* mysql_, std::string sqlstr_);
MySQLErrInfo print(MYSQL* mysql_);
int main()
{
	mysql_library_init(0, NULL, NULL);

	MySQLFoundationWrapper mysql1(5, 15, "localhost", "root", "root942612", 3307);
	MySQLErrInfo error_ = mysql1.start();
	if(mysql1.is_connection_){}
	else
		error_ = mysql1.start();

	if(error_.error_no_ == 0)
	{
		MYSQL* sql_ = mysql1.getMySQL();

		/// 1,first 
		error_ = query(sql_, "select * from world.city limit 10,20");
		if(error_.error_no_ == 0)
			print(sql_);
		boost::this_thread::sleep(boost::posix_time::seconds(10));

		/// 2,second
		error_ = query(sql_, "show databases");
		if(error_.error_no_ == 0)
			print(sql_);
		boost::this_thread::sleep(boost::posix_time::seconds(5));

		/// 3,third
		error_ = query(sql_, "select table_schema from information_schema.views");
		if(error_.error_no_ == 0)
			print(sql_);
		boost::this_thread::sleep(boost::posix_time::seconds(5));
	}

	boost::this_thread::sleep(boost::posix_time::seconds(20));
	mysql1.end();
	system("pause");
	mysql1.wait();

	mysql_library_end();
	return NULL;
}

MySQLErrInfo query(MYSQL* mysql_, std::string sqlstr_){
	MySQLErrInfo err_;
	int ret = mysql_real_query(mysql_, sqlstr_.c_str(), unsigned int(sqlstr_.size()));
	if (ret){
		err_.error_no_   = mysql_errno(mysql_);
		err_.error_info_ = mysql_error(mysql_);
	}
	return err_;
}

MySQLErrInfo print(MYSQL* mysql_){
	MySQLErrInfo err_;
	MYSQL_RES* res_ = NULL;
	res_ = mysql_store_result(mysql_);
	if (res_ == NULL){
		err_.error_no_ = mysql_errno(mysql_);
		err_.error_info_ = mysql_error(mysql_);
		return err_;
	}
	int fields_num_ = mysql_num_fields(res_);
	MYSQL_FIELD* fields_ = mysql_fetch_fields(res_);
	for (int i = 0; i < fields_num_; i++)
		std::cout << "<" << fields_[i].name << ">" << " ";
	std::cout << std::endl;
	MYSQL_ROW row_ = NULL;
	std::cout << "row_count\t" << res_->row_count << std::endl;
	while ((row_ = mysql_fetch_row(res_))){
		for (int t = 0; t < fields_num_; ++t){
			if (row_[t] == NULL)
				std::cout << "[" << "]" << " ";
			else
				std::cout << "[" << row_[t] << "]" << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	mysql_free_result(res_);
	res_ = NULL;
	return err_;
}