#include "mysql_c_api_foundation.h"
#include "mysql_c_api_data.h"
#include "mysql_c_api_foundation.h"
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "libboost_thread-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_date_time-vc110-mt-1_58.lib")
#pragma comment(lib, "libboost_system-vc110-mt-1_58.lib")
void print(const std::vector<std::string>&);
void print(const boost::multi_array<std::string, 2>&);
int main()
{
	mysql_library_init(0, NULL, NULL);

	MySQLErrInfo error_;
	MySQLFoundationWrapper mysql1(5, 20, "localhost", "root", "root", 3306);
	if(mysql1.is_connection_){}
	else
		error_ = mysql1.start();

	if(error_.error_no_ == 0)
	{
		MYSQL* sql_ = mysql1.getMySQL();

		// DataBases
		std::vector<std::string> str_;
		boost::multi_array<std::string, 2> data_;
		error_ = MySQLDataFetchWrapper::mysql_fetch_databases(sql_, str_);
		if(error_.error_no_ == 0)
			print(str_);
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;

		// Table
		error_ = MySQLDataFetchWrapper::mysql_fetch_tables(sql_, "demo", str_);
		if(error_.error_no_ == 0)
			print(str_);
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;

		// View
		error_ = MySQLDataFetchWrapper::mysql_fetch_views(sql_, "demo", str_);
		if(error_.error_no_ == 0)
			print(str_);
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;

		// Table
		error_ = MySQLDataFetchWrapper::mysql_fetch_table_data(sql_, "demo", "fruits", str_, data_, 0, 30);
		if(error_.error_no_ == 0)
		{
			print(str_); 
			print(data_);
		}
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;

		// Table
		error_ = MySQLDataFetchWrapper::mysql_fetch_table_data(sql_, "demo", "fruits", str_, data_, 50, 30);
		if(error_.error_no_ == 0)
		{
			print(str_); 
			print(data_);
		}
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;

		// Table
		error_ = MySQLDataFetchWrapper::mysql_fetch_table_data(sql_, "demo", "suppliers", str_, data_, 0, 100);
		if(error_.error_no_ == 0)
		{
			print(str_); 
			print(data_);
		}
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;

		// View
		error_ = MySQLDataFetchWrapper::mysql_fetch_table_data(sql_, "demo", "mixed_view", str_, data_, 0, 100);
		if(error_.error_no_ == 0)
		{
			print(str_); 
			print(data_);
		}
		else
			std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;		
	}else
	{
		std::cout << error_.error_no_ << ":" << error_.error_info_ << std::endl;	
	}

	mysql1.end();
	system("pause");
	mysql1.wait();

	mysql_library_end();
	return NULL;
}

void print(const std::vector<std::string>& v_)
{
	std::ostream_iterator<std::string> os_(std::cout, "|");
	std::copy(v_.begin(), v_.end(), os_);
	std::cout << std::endl;
}

void print(const boost::multi_array<std::string, 2>& data_)
{
	int rows_ = data_.shape()[0];
	int cols_ = data_.shape()[1];
	for(int r = 0;r < rows_;++r)
	{
		for(int c = 0;c < cols_;++c)
		{
			std::cout << "[" << data_[r][c] << "]" << " ";
		}
		std::cout << std::endl;
	}
}