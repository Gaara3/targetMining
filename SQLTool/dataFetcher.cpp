#include <WinSock.h>  //һ��Ҫ�������������winsock2.h
#include <stdio.h>
#include <Windows.h>
#include "mysql.h"    //����mysqlͷ�ļ�(һ�ַ�ʽ����vcĿ¼�������ã�һ�����ļ��п�������Ŀ¼��Ȼ����������)

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

MYSQL mysqlConn;


bool connectDatabase() {
	mysql_init(&mysql);//��ʼ��mysql,����mysql

					   //����false������ʧ�ܣ�����true�����ӳɹ�
	if (!(mysql_real_connect(&mysql, "localhost", "root", "root", "test", 3306, NULL, 0))) {
		printf("Error connecting to datebase:%s\n", mysql_error(&mysql));
		return false;
	}
	else {
		printf("Connected...\n");
		return true;
	}
}

