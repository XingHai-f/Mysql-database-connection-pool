#pragma once

//在输出日志时，打印出包含文件名、行号、时间的信息，有助于排查问题
#define LOG(str) \
	cout << __FILE__ << ":" << __LINE__ << " " << \
	__TIMESTAMP__ << " : " << str << endl;
