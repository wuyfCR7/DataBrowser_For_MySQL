#include "mysql_c_api_data.h"

/// query
MySQLErrInfo MySQLDataFetchWrapper::query(MYSQL* mysql_, std::string sql_)
{
	MySQLErrInfo err_;
	int ret = mysql_real_query(mysql_, sql_.c_str(), unsigned int(sql_.size()));
	if (ret){
		err_.error_no_   = mysql_errno(mysql_);
		err_.error_info_ = mysql_error(mysql_);
		return err_;
	}
	return err_;
}

/// result operation
MySQLErrInfo MySQLDataFetchWrapper::get_results(MYSQL* mysql_, std::vector<std::string>& fieldnames_, boost::multi_array<std::string, 2>& data_)
{
	MySQLErrInfo err_;
	MYSQL_RES* res_ = NULL;
	
	// store results
	res_ = mysql_store_result(mysql_);
	if (res_ == NULL){
		err_.error_no_ = mysql_errno(mysql_);
		err_.error_info_ = mysql_error(mysql_);
		return err_;
	}

	// fieldnames_
	int fields_num_ = mysql_num_fields(res_);
	MYSQL_FIELD* fields_ = mysql_fetch_fields(res_);
	fieldnames_.resize(fields_num_);
	for (int i = 0; i < fields_num_; i++)
		fieldnames_[i] = fields_[i].name;

	// data
	MYSQL_ROW mysql_row_ = NULL;
	int rows_ = int(res_->row_count);
	if(rows_ == 0)
	{
		err_.error_no_ = 10086;
		err_.error_info_ = "Empty Data Set";
	}
	else
	{
		int cols_ = fields_num_;
		boost::array<int, 2> arr_;
		arr_[0] = rows_;
		arr_[1] = cols_; 
		data_.resize(arr_);
		int r = 0;
		while ((mysql_row_ = mysql_fetch_row(res_))){
			for (int c = 0; c < fields_num_; ++c){
				if (mysql_row_[c] == NULL)
					data_[r][c] = "";
				else
					data_[r][c] = mysql_row_[c];
			}
			++r;
		}
	}
	mysql_free_result(res_);
	res_ = NULL;
	return err_;
}

/// Show Databases
MySQLErrInfo MySQLDataFetchWrapper::mysql_fetch_databases(MYSQL* mysql_, std::vector<std::string>& databases_)
{
	std::string sql_  = "show databases";
	MySQLErrInfo err_ = MySQLDataFetchWrapper::query(mysql_, sql_);
	if(err_.error_no_ == 0)
	{
		std::vector<std::string> fieldnames_;
		boost::multi_array<std::string, 2> data_;
		err_ = MySQLDataFetchWrapper::get_results(mysql_, fieldnames_, data_);
		if(err_.error_no_ == 0)
		{
			int rows_ = data_.shape()[0];
			databases_.resize(rows_);
			for(int t = 0;t < rows_;++t)
			{
				databases_[t] = data_[t][0];
			}
		}
	}
	return err_;
}

/// Show Tables
MySQLErrInfo MySQLDataFetchWrapper::mysql_fetch_tables(MYSQL* mysql_, std::string db_name_, std::vector<std::string>& tables_)
{
	char sql_[256];
	memset(sql_, 0, 256);
	sprintf_s(sql_, "select table_name from information_schema.tables where table_schema = '%s'", db_name_.c_str());
	MySQLErrInfo err_ = MySQLDataFetchWrapper::query(mysql_, sql_);
	if(err_.error_no_ == 0)
	{
		std::vector<std::string> fieldnames_;
		boost::multi_array<std::string, 2> data_;
		err_ = MySQLDataFetchWrapper::get_results(mysql_, fieldnames_, data_);
		if(err_.error_no_ == 0)
		{
			int rows_ = data_.shape()[0];
			tables_.resize(rows_);
			for(int t = 0;t < rows_;++t)
			{
				tables_[t] = data_[t][0];
			}
		}
	}
	return err_;
}

/// Show Views
MySQLErrInfo MySQLDataFetchWrapper::mysql_fetch_views(MYSQL* mysql_, std::string db_name_, std::vector<std::string>& views_)
{
	char sql_[256];
	memset(sql_, 0, 256);
	sprintf_s(sql_, "select table_name from information_schema.views where table_schema = '%s'", db_name_.c_str());
	MySQLErrInfo err_ = MySQLDataFetchWrapper::query(mysql_, sql_);
	if(err_.error_no_ == 0)
	{
		std::vector<std::string> fieldnames_;
		boost::multi_array<std::string, 2> data_;
		err_ = MySQLDataFetchWrapper::get_results(mysql_, fieldnames_, data_);
		if(err_.error_no_ == 0)
		{
			int rows_ = data_.shape()[0];
			views_.resize(rows_);
			for(int t = 0;t < rows_;++t)
			{
				views_[t] = data_[t][0];
			}
		}
	}
	return err_;
}

/// Get Table/View Data
MySQLErrInfo MySQLDataFetchWrapper::mysql_fetch_table_data(
	MYSQL*mysql_, std::string dbname_, std::string tbname_, std::vector<std::string>& fields_, boost::multi_array<std::string, 2>& data_, int start_, int line_)
{
	char sql_[256];
	memset(sql_, 0, 256);
	sprintf_s(sql_, "select * from %s.%s limit %d,%d", dbname_.c_str(), tbname_.c_str(), start_, line_);
	MySQLErrInfo err_ = MySQLDataFetchWrapper::query(mysql_, sql_);
	if(err_.error_no_ == 0)
	{
		err_ = MySQLDataFetchWrapper::get_results(mysql_, fields_, data_);
	}
	return err_;
}