#include "public.h"
#include "Connection.h"

#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

Connection::Connection()
{
	// 初始化数据库连接
	_conn = mysql_init(nullptr);
	if (_conn == nullptr) {
		LOG("mysql_init failed!");
	}
}

Connection::~Connection()
{
	// 释放数据库连接资源
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port,
	string username, string password, string dbname)
{
	// 保存连接信息
	_ip = ip;
	_port = port;
	_username = username;
	_password = password;
	_dbname = dbname;

	if (_conn == nullptr) {
		_conn = mysql_init(nullptr);
		if (_conn == nullptr) {
			LOG("mysql_init failed!");
			return false;
		}
	}

	// 连接数据库
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	if (p == nullptr) {
		LOG("连接数据库失败: " + string(mysql_error(_conn)));
		return false;
	}
	return p != nullptr;
}

bool Connection::update(string sql)
{
	/*if (_conn == nullptr) {
		LOG("连接无效");
		return false;
	}

	if (mysql_ping(_conn)) {
		LOG("检测到连接断开，尝试重新连接...");
		if (!connect(_ip, _port, _username, _password, _dbname)) {
			LOG("重新连接失败" + string(mysql_error(_conn)));
			return false;
		}
	}*/
	

	LOG("DBName: " + _dbname + ", Length: " + std::to_string(_dbname.length()));
	LOG("连接参数 - IP: " + _ip + ", Port: " + std::to_string(_port) + ", Username: " + _username + ", Password: " + _password + ", DBName: " + _dbname);

	if (_conn == nullptr) {
		LOG("连接无效");
		return false;
	}

	if (mysql_ping(_conn)) {
		LOG("检测到连接断开，尝试重新连接...");
		LOG("重新连接使用的参数 - IP: " + _ip + ", Port: " + std::to_string(_port) + ", Username: " + _username + ", DBName: " + _dbname);
		if (!connect(_ip, _port, _username, _password, _dbname)) {
			LOG("重新连接失败: " + std::string(mysql_error(_conn)));
			return false;
		}
	}

	// 更新操作 insert、delete、update
	if (mysql_query(_conn, sql.c_str()))
	{
		string error_msg = "更新失败: " + sql + ", 错误信息: " + mysql_error(_conn);
		LOG(error_msg);
		return false;
	}
	return true;
}

MYSQL_RES* Connection::query(string sql)
{
	if (_conn == nullptr) {
		LOG("连接无效");
		return nullptr;
	}

	if (mysql_ping(_conn)) {
		LOG("检测到连接断开，尝试重新连接...");
		if (!connect(_ip, _port, _username, _password, _dbname)) {
			LOG("重新连接失败" + string(mysql_error(_conn)));
			return nullptr;
		}
	}

	// 查询操作 select
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("查询失败: " + sql + ", 错误信息: " + mysql_error(_conn));
		return nullptr;
	}
	return mysql_use_result(_conn);
}

bool Connection::reconnect()
{
	int retries = 5;
	while (retries > 0) {
		// 尝试重新连接
		if (connect(_ip, _port, _username, _password, _dbname)) {
			LOG("重新连接成功");
			return true;
		}
		LOG("重新连接失败: " + string(mysql_error(_conn)));
		retries--;
		this_thread::sleep_for(chrono::seconds(2)); // 等待一段时间再重试
	}
	return false;
}
