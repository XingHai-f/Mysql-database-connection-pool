#pragma once

//�������־ʱ����ӡ�������ļ������кš�ʱ�����Ϣ���������Ų�����
#define LOG(str) \
	cout << __FILE__ << ":" << __LINE__ << " " << \
	__TIMESTAMP__ << " : " << str << endl;
