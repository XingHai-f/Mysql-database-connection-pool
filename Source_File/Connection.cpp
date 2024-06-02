#include "public.h"
#include "Connection.h"

#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

Connection::Connection()
{
	// ��ʼ�����ݿ�����
	_conn = mysql_init(nullptr);
	if (_conn == nullptr) {
		LOG("mysql_init failed!");
	}
}

Connection::~Connection()
{
	// �ͷ����ݿ�������Դ
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port,
	string username, string password, string dbname)
{
	// ����������Ϣ
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

	// �������ݿ�
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	if (p == nullptr) {
		LOG("�������ݿ�ʧ��: " + string(mysql_error(_conn)));
		return false;
	}
	return p != nullptr;
}

bool Connection::update(string sql)
{
	/*if (_conn == nullptr) {
		LOG("������Ч");
		return false;
	}

	if (mysql_ping(_conn)) {
		LOG("��⵽���ӶϿ���������������...");
		if (!connect(_ip, _port, _username, _password, _dbname)) {
			LOG("��������ʧ��" + string(mysql_error(_conn)));
			return false;
		}
	}*/
	

	LOG("DBName: " + _dbname + ", Length: " + std::to_string(_dbname.length()));
	LOG("���Ӳ��� - IP: " + _ip + ", Port: " + std::to_string(_port) + ", Username: " + _username + ", Password: " + _password + ", DBName: " + _dbname);

	if (_conn == nullptr) {
		LOG("������Ч");
		return false;
	}

	if (mysql_ping(_conn)) {
		LOG("��⵽���ӶϿ���������������...");
		LOG("��������ʹ�õĲ��� - IP: " + _ip + ", Port: " + std::to_string(_port) + ", Username: " + _username + ", DBName: " + _dbname);
		if (!connect(_ip, _port, _username, _password, _dbname)) {
			LOG("��������ʧ��: " + std::string(mysql_error(_conn)));
			return false;
		}
	}

	// ���²��� insert��delete��update
	if (mysql_query(_conn, sql.c_str()))
	{
		string error_msg = "����ʧ��: " + sql + ", ������Ϣ: " + mysql_error(_conn);
		LOG(error_msg);
		return false;
	}
	return true;
}

MYSQL_RES* Connection::query(string sql)
{
	if (_conn == nullptr) {
		LOG("������Ч");
		return nullptr;
	}

	if (mysql_ping(_conn)) {
		LOG("��⵽���ӶϿ���������������...");
		if (!connect(_ip, _port, _username, _password, _dbname)) {
			LOG("��������ʧ��" + string(mysql_error(_conn)));
			return nullptr;
		}
	}

	// ��ѯ���� select
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("��ѯʧ��: " + sql + ", ������Ϣ: " + mysql_error(_conn));
		return nullptr;
	}
	return mysql_use_result(_conn);
}

bool Connection::reconnect()
{
	int retries = 5;
	while (retries > 0) {
		// ������������
		if (connect(_ip, _port, _username, _password, _dbname)) {
			LOG("�������ӳɹ�");
			return true;
		}
		LOG("��������ʧ��: " + string(mysql_error(_conn)));
		retries--;
		this_thread::sleep_for(chrono::seconds(2)); // �ȴ�һ��ʱ��������
	}
	return false;
}
