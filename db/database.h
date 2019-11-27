//
// ffiirree@20191016
//

#ifndef DB_DATABASE_H
#define DB_DATABASE_H

#include <string>
#include <tuple>
#include <mysql.h>
#include <cassert>
#include "system.h"

class Database {
public:
	static Database& instance() {
		static Database instance(
			System::instance().mysql_.username_, 
			System::instance().mysql_.password_, 
			System::instance().mysql_.database_, 
			System::instance().mysql_.port_
		);
		return instance;
	}

	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;

	bool connect()
	{
		mysql_init(&connection_);

		if (!mysql_real_connect(&connection_, "localhost", username_.c_str(), password_.c_str(), database_.c_str(), port_, NULL, 0)) {
			is_connected_ = false;
			assert(is_connected_ == false);
			return false;
		}
		is_connected_ = true;
		return true;
	}

	bool isConnected() const { return is_connected_; }

	std::tuple<MYSQL_RES*, int, int> query(std::string str)
	{
		if (mysql_query(&connection_, str.c_str())) {
			return { nullptr, 0, 0 };
		}
		auto res = mysql_store_result(&connection_);
		auto rows = mysql_num_rows(res);
		auto cols = mysql_num_fields(res);
		return  std::make_tuple(res, rows, cols);
	}

	int insert(std::string str) 
	{
		return mysql_query(&connection_, str.c_str());
	}

private:
	Database() {
		connect();
	}

	Database(std::string username, std::string pwd, std::string db, unsigned int port = 3306)
	: username_(username), password_(pwd), database_(db), port_(port) {
		connect();
	}

	std::string username_ = "root";
	std::string password_ = "";
	std::string database_ = "";
	unsigned int port_ = 3306;

	MYSQL connection_;
	//MYSQL_RES * res_ = nullptr;

	bool is_connected_ = false;
};

#endif // DB_DATABASE_H
