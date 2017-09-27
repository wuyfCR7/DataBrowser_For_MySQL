#ifndef __SQLITE_C_API_FOUNDATION_H__
#define __SQLITE_C_API_FOUNDATION_H__
#ifdef _MSC_VER > 1000
#pragma once
#endif
#include <sqlite3.h>
#include <string>
#include <memory>

struct SQLite3Error
{
	int no_;
	std::string msg_;
	SQLite3Error() :no_(0), msg_(""){}
};

typedef std::shared_ptr<sqlite3> SQLite3DataBasePtr;
typedef int (*SQLite3ExecCallback)(void*, int, char**, char**);
typedef enum
{
	INTEGER,
	REAL,
	TEXT
}SQLite3DataType;

struct SQLite3Wrapper
{
	/// SQLite3 DB Close Method
	/*
	import:
		sqlite3* SQLite3数据库
	*/
	static void sqlite3_close_db(sqlite3*);

	/// SQLite3 DB Connect Method
	/*
	import:
		std::string  数据库的完整路径
		SQLite3Error 数据库连接问题
	export:
		SQLite3DataBasePtr SQLite3数据库智能Pointer
	*/
	static SQLite3DataBasePtr sqlite3_connect_db(std::string, SQLite3Error&);

	/// SQLite3 DB Auto DisConnect Method
	/*
	import:
		SQLite3DataBasePtr SQLite3数据库智能Pointer
	*/
	static void sqlite3_disconnect_db(SQLite3DataBasePtr&);

	/// SQLite3 DB Exec SQL Method
	/*
	import:
		SQLite3DataBasePtr  SQLite3数据库智能Pointer
		std::string         SQL语句
		void*               UserData
		SQLite3ExecCallback CallBack Function
	export:
		SQLite3Error 数据库操作问题
	*/
	static SQLite3Error sqlite3_exec_db(SQLite3DataBasePtr&, std::string, void*, SQLite3ExecCallback);

	/// SQLite3 DB Load From TXT Method
	/*
	import:
		SQLite3DataBasePtr  SQLite3数据库智能Pointer
		std::string         文件路径名称
		std::string         Table名称
		void*               UserData
		SQLite3DataType*    文本数据类型
		int                 数据的列数
		SQLite3ExecCallback CallBack Function
	export:
		SQLite3Error 数据库操作问题
	*/
	static SQLite3Error sqlite3_load_txt_table(SQLite3DataBasePtr&, std::string, std::string, void*, SQLite3DataType*, int, SQLite3ExecCallback);

	/// SQLite3 DB Print Example CallBack
	static int sqlite3_qry_print_callback(void*, int, char**, char**);  
	static int sqlite3_qry_empty_callback(void*, int, char**, char**);  
};











#endif