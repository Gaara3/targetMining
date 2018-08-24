#include "stdafx.h"
//#include "DataMiner.h"
//#include "SqlTool.h"
//#include "Track.h"
//#include "TrackDetail.h"
//#include <vector>
//#include <string>
//
//using std::vector;
//using std::string;
//
//MYSQL_RES *res;
//SqlTool sqlTool;
//MYSQL_ROW column;
//
//
//int testmain() {
//	vector<char*> targets;
//	vector<vector<Track>> TracksOfAllTargets;    //轨迹集合的集合，即所有tar的轨迹集合
//
//	if (!sqlTool.connectDB()) {
//		return 0;//连接数据库异常
//	}
//	if (!sqlTool.operationExcutor(Track::getTargetsQuery, res))
//		return 0;//执行数据库操作异常,程序结束
//	int targetNum = mysql_num_fields(res);  // 获取列数 
//	while (column = mysql_fetch_row(res))//对每个目标进行操作
//	{
//		targets.push_back(column[0]);//column 0 是targetID      sqlTool.operationExcutor("SELECT ")
//		printf(column[0]);
//	}
//	mysql_free_result(res);
//
//	
//	int targetsNum = targets.size();
//	for (int counter = 0; counter < targetsNum; counter++) {	//对每个对象进行轨迹提取为了性能考虑暂时不用迭代器
//		printf("=================new target================\n");
//		//分段开始
//		int orderCounter = 0;
//		int lastPosixTime = 0;
//		char* currentTrackID =new char;
//		vector<Track> TracksOfOneTarget;
//		
//		if (sqlTool.operationExcutor(Track::getTargetRecords(targets[counter]),res)) {//单个target的所有记录
//			MYSQL_RES *targetRecords = res;
//			while (column = mysql_fetch_row(targetRecords)) {
//				//无论是否为起点，先生成点记录
//				TrackDetail tmp = TrackDetail(column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7], column[8]);
//				if (!tmp.headOfTrack(lastPosixTime)) {	//当前点迹不是起点
//					tmp.setOderNumber(++orderCounter);				
//					sqlTool.operationExcutor(tmp.insertSQL().data(), res); //插入历史轨迹sub表	
//				}
//				else {		//当前点轨迹是起点
//					printf("----------------new track-----------\n");
//					if (!TracksOfOneTarget.empty()) {//此目标此前已有轨迹，需对上一条轨迹进行最后的处理并入库（需更新上一轨迹的endTime,orderNumber）
//						TracksOfOneTarget.back().setPointAmount(orderCounter);
//						TracksOfOneTarget.back().setEndTime(lastPosixTime);
//						//存入数据库
//						sqlTool.operationExcutor(TracksOfOneTarget.back().insertSQL(),res);
//						orderCounter = 0;	
//					}
//
//					//对本轨迹起点的处理
//					Track tmpTrack = Track(0,column[0],column[2],column[9],column[6],column[1]);//SELECT TARGETID 0,POSIXTIME 1,SOURCE 2,LONGITUDE 3,LATITUDE 4,ALTITUDE 5,OPERATOR 6,RESERVE1 7,RESERVE2 8,TASKINFO 9;					
//					orderCounter = 1;					
//					//currentTrackID = sqlTool.uuidGenerator(res);
//					//tmpTrack.setTrackID(currentTrackID);
//					tmpTrack.setTrackID(1111);
//					TracksOfOneTarget.push_back(tmpTrack);	//新建轨迹并加入该tar的列表
//				}
//				tmp.setTrackID(1111);
//				TracksOfOneTarget.back().TrackDetail.push_back(tmp);//当前轨迹存入该点
//				lastPosixTime = atoi(column[1]);//无论是否开启新段，都需要更新lastPosixTime
//				printf("         new point     %d   \n",orderCounter);
//			}
//			//对本tar的最后一个track进行封装、入库
//			TracksOfOneTarget.back().setEndTime(lastPosixTime);
//			TracksOfOneTarget.back().setPointAmount(orderCounter);
//			//sqlTool.operationExcutor(TracksOfOneTarget.back().insertSQL(), res);	
//			//sqlTool.operationExcutor(TracksOfOneTarget.back().insertSQL(), res);//这里四行可封装
//
//			printf("result rows:%d", targetRecords->row_count);
//			//mysql_free_result(res);
//			mysql_free_result(targetRecords);
//		}
//
//		TracksOfAllTargets.push_back(TracksOfOneTarget);//将当前tar的所有轨迹放入总vector
//	}
//
//	system("pause");
//	//char *str_field[32];  //定义一个字符串数组存储字段信息  
//	//for (int i = 0; i<9; i++)   //在已知字段数量的情况下获取字段名  
//	//{
//	//	str_field[i] = mysql_fetch_field(res)->name;
//	//	printf("%10s\t", str_field[i]);
//	//}
//	//printf("result rows:%d", res->row_count);
// //	while (column = mysql_fetch_row(res)) {
//	//	printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3], column[4], column[5],column[6],column[7],column[8]);  //column是列数组  
//	//}
//
//	system("pause");
//	return 0;
//}
//
