#ifndef __DEFINE_H__
#define __DEFINE_H__


#define CHAR_SIZE 255

//һ���������������
#define SQL_NUM 10000

//�е�ֵ
#define M_PI 3.14159265358979323846

//����뾶����λ��
#define Earth_Radius 6371004


enum AreaType
{
	AreaType_Unknown = 0,
	AreaType_Rectangle,		//����
	AreaType_Circular,		//Բ��
	AreaType_PreDef			//Ԥ��������
};


/**
* @brief ����mysql���ݿ����Ϣ
*/
typedef struct _MysqlInfo
{
	char			host[CHAR_SIZE];    //����
	char			user[CHAR_SIZE];    //�û���
	char			passwd[CHAR_SIZE];  //����
	char			db[CHAR_SIZE];		//���ݿ�
	unsigned int	port;				//�˿ں�
	unsigned long	client_flag;		// client_flag��ֵͨ��Ϊ0
	char			unix_socket[CHAR_SIZE];

	_MysqlInfo()
	{
		port = 0;
		client_flag = 0;
		memset(host, 0, sizeof(char) * CHAR_SIZE);
		memset(user, 0, sizeof(char) * CHAR_SIZE);
		memset(passwd, 0, sizeof(char) * CHAR_SIZE);
		memset(unix_socket, 0, sizeof(char) * CHAR_SIZE);
	}
}MysqlInfo;



/**
* @brief �û�ɸѡ������Ϣ��
*/
typedef struct _SelFilterRule
{
	char			GUID[CHAR_SIZE];		//Ψһ��ʶ��
	int				Id;						//�������
	int				TargetId;				//Ŀ����
	char			ShipBoardNum[CHAR_SIZE];//���Ϻ�
	char			TargetModelNum[CHAR_SIZE];//Ŀ���ͺ�
	char			TargetName[CHAR_SIZE];	//Ŀ������
	char			TargetType[CHAR_SIZE];	//Ŀ������
	char			TargetAttr[CHAR_SIZE];	//Ŀ�����ԣ��з����ҷ����ѷ���������������
	char			TargetAttrFlag[CHAR_SIZE];//Ŀ�������򣬺���½����
	int				Militarycivil;			//Ŀ�����ʣ����¡�����
	char			GB[CHAR_SIZE];			//����
	char			Source[CHAR_SIZE];		//��Դ
	char			StartTime[CHAR_SIZE];	//��ʼʱ��
	char			EndTime[CHAR_SIZE];		//����ʱ��
	char			Operator[CHAR_SIZE];	//����Ա
	char			Reserve1[CHAR_SIZE];	//Ԥ��1
	char			Reserve2[CHAR_SIZE];	//Ԥ��2

	_SelFilterRule()
	{
		Id = 0;
		TargetId = 0;
		Militarycivil = 0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(ShipBoardNum, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetModelNum, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetName, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetType, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetAttr, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetAttrFlag, 0, sizeof(char) * CHAR_SIZE);
		memset(GB, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(StartTime, 0, sizeof(char) * CHAR_SIZE);
		memset(EndTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Operator, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}SelFilterRule;



/**
* @brief �û�ѡ��������Ϣ��
*/
typedef struct _SelArea
{
	char			GUID[CHAR_SIZE];		//Ψһ��ʶ��
	int				Id;						//�������
	int				SelBatchNum;			//ѡ������
	char			RecTime[CHAR_SIZE];		//��¼ʱ��
	AreaType		AreaShape;				//������״ 1.���� 2.Բ�� 3.Ԥ����
	int				AreaId;					//����ID
	char			Operator[CHAR_SIZE];	//����Ա
	char			Reserve1[CHAR_SIZE];	//Ԥ��1
	char			Reserve2[CHAR_SIZE];	//Ԥ��2

	_SelArea()
	{
		Id = 0;
		SelBatchNum = 0;
		AreaShape = AreaType_Unknown;
		AreaId = 0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(RecTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Operator, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}SelArea;



/**
* @brief �켣�ֶ���������
*/
typedef struct _TrackMain
{
	char			GUID[CHAR_SIZE];		//Ψһ��ʶ��
	int				TrackId;				//�켣ID
	int				ContainsPoints;			//��������
	int				TargetId;				//Ŀ����
	char			StartTime[CHAR_SIZE];	//��ʼʱ��
	char			EndTime[CHAR_SIZE];		//����ʱ��
	double			TrackLength;			//�켣����
	char			Source[CHAR_SIZE];		//��Դ
	char			TaskInfo[CHAR_SIZE];	//����
	double			ConfidenceLevel;		//���Ŷ�
	char			Operator[CHAR_SIZE];	//����Ա
	char			Reserve1[CHAR_SIZE];	//Ԥ��1
	char			Reserve2[CHAR_SIZE];	//Ԥ��2

	_TrackMain()
	{
		TrackId = 0;
		ContainsPoints = 0;
		TargetId = 0;
		TrackLength = 0.0;
		ConfidenceLevel = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(StartTime, 0, sizeof(char) * CHAR_SIZE);
		memset(EndTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(TaskInfo, 0, sizeof(char) * CHAR_SIZE);
		memset(Operator, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackMain;



/**
* @brief �켣�ֶ������ӱ�
*/
typedef struct _TrackSub
{
	char			GUID[CHAR_SIZE];		//Ψһ��ʶ��
	int				Id;						//�������
	int				TrackId;				//�켣ID
	int				OrderNum;				//˳��ţ��ڹ켣�е������
	char			Time[CHAR_SIZE];		//�켣��ʱ��
	int				PosixTime;				//POSIXʱ�䣬��1970.01.01 00:00:00����ǰ�켣��ʱ�������
	char			Source[CHAR_SIZE];		//��Դ
	double			CentralLongitude;		//���ľ���
	double			CentralLatitude;		//����γ��
	double			CentralAltitude;		//���ĸ߶�
	double			UpperLeftLongitude;		//���Ͼ���
	double			UpperLeftLatitude;		//����γ��
	double			UpperLeftAltitude;		//���ϸ߶�
	double			UpperRightLongitude;	//���Ͼ���
	double			UpperRightLatitude;		//����γ��
	double			UpperRightAltitude;		//���ϸ߶�
	double			LowerRightLongitude;	//���¾���
	double			LowerRightLatitude;		//����γ��
	double			LowerRightAltitude;		//���¸߶�
	double			LowerLeftLongitude;		//���¾���
	double			LowerLeftLatitude;		//����γ��
	double			LowerLeftAltitude;		//���¸߶�
	double			Speed;					//����
	double			Angle;					//�Ƕ�
	double			ConfidenceLevel;		//���Ŷ�
	char			Reserve1[CHAR_SIZE];	//Ԥ��1
	char			Reserve2[CHAR_SIZE];	//Ԥ��2

	_TrackSub()
	{
		Id = 0;
		TrackId = 0;
		OrderNum = 0;
		PosixTime = 0;
		CentralLongitude = 0.0;
		CentralLatitude = 0.0;
		CentralAltitude = 0.0;
		UpperLeftLongitude = 0.0;
		UpperLeftLatitude = 0.0;
		UpperLeftAltitude = 0.0;
		UpperRightLongitude = 0.0;
		UpperRightLatitude = 0.0;
		UpperRightAltitude = 0.0;
		LowerRightLongitude = 0.0;
		LowerRightLatitude = 0.0;
		LowerRightAltitude = 0.0;
		LowerLeftLongitude = 0.0;
		LowerLeftLatitude = 0.0;
		LowerLeftAltitude = 0.0;
		ConfidenceLevel = 0.0;
		Speed = 0.0;
		Angle = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(Time, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackSub;


/**
* @brief �켣�ֶ������ӱ��߶���ʽ
*/
typedef struct _TrackSubGeom
{
	char			GUID[CHAR_SIZE];		//Ψһ��ʶ��
	int				Id;						//�������
	int				TrackId;				//�켣ID
	int				OrderNum;				//˳��ţ�����ڹ켣�е������
	char			StartTime[CHAR_SIZE];	//����ʱ��
	char			EndTime[CHAR_SIZE];		//�յ��ʱ��
	int				StartPosixTime;			//����POSIXʱ�䣬��1970.01.01 00:00:00����ǰ�켣��ʱ�������
	int				EndPosixTime;			//�յ��POSIXʱ�䣬��1970.01.01 00:00:00����ǰ�켣��ʱ�������
	char			Source[CHAR_SIZE];		//��Դ
	double			StartLongitude;			//���ľ���
	double			StartLatitude;			//����γ��
	double			EndLongitude;			//�յ�ľ���
	double			EndLatitude;			//�յ��γ��
	double			ConfidenceLevel;		//���Ŷ�
	char			Reserve1[CHAR_SIZE];	//Ԥ��1
	char			Reserve2[CHAR_SIZE];	//Ԥ��2

	_TrackSubGeom()
	{
		Id = 0;
		TrackId = 0;
		OrderNum = 0;
		StartPosixTime = 0;
		EndPosixTime = 0;
		StartLongitude = 0.0;
		StartLatitude = 0.0;
		EndLongitude = 0.0;
		EndLatitude = 0.0;
		ConfidenceLevel = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(StartTime, 0, sizeof(char) * CHAR_SIZE);
		memset(EndTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackSubGeom;



/**
* @brief �켣�ֶ����ݳ�ϡ�ӱ�
*/
typedef struct _TrackSamSub
{
	char			GUID[CHAR_SIZE];		//Ψһ��ʶ��
	int				Id;						//�������
	int				TrackId;				//�켣ID
	int				OrderNum;				//˳��ţ���ϡ֮���ڹ켣�е������
	int				OriginOrderNum;			//˳��ţ���ϡ֮ǰ�ڹ켣�е������
	char			Time[CHAR_SIZE];		//�켣��ʱ��
	int				PosixTime;				//POSIXʱ�䣬��1970.01.01 00:00:00����ǰ�켣��ʱ�������
	int				SamplingLevel;			//��ϡ����
	char			Source[CHAR_SIZE];		//��Դ
	double			CentralLongitude;		//���ľ���
	double			CentralLatitude;		//����γ��
	double			CentralAltitude;		//���ĸ߶�
	double			Speed;					//����
	double			Angle;					//�Ƕ�
	double			ConfidenceLevel;		//���Ŷ�
	char			Reserve1[CHAR_SIZE];	//Ԥ��1
	char			Reserve2[CHAR_SIZE];	//Ԥ��2

	_TrackSamSub()
	{
		Id = 0;
		TrackId = 0;
		OrderNum = 0;
		OriginOrderNum = 0;
		PosixTime = 0;
		SamplingLevel = 0;
		CentralLongitude = 0.0;
		CentralLatitude = 0.0;
		CentralAltitude = 0.0;
		ConfidenceLevel = 0.0;
		Speed = 0.0;
		Angle = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(Time, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackSamSub;


#endif //__DEFINE_H__
