#include <WinSock.h>  //一定要包含这个，或者winsock2.h
#include <stdio.h>
#include <Windows.h>
#include "mysql.h"    //引入mysql头文件(一种方式是在vc目录里面设置，一种是文件夹拷到工程目录，然后这样包含)

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

MYSQL mysqlConn;


bool connectDatabase() {
	mysql_init(&mysql);//初始化mysql,连接mysql

					   //返回false则连接失败，返回true则连接成功
	if (!(mysql_real_connect(&mysql, "localhost", "root", "root", "test", 3306, NULL, 0))) {
		printf("Error connecting to datebase:%s\n", mysql_error(&mysql));
		return false;
	}
	else {
		printf("Connected...\n");
		return true;
	}
}

