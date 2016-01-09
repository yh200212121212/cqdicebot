#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include "MersenneTwister.h"
using namespace std;

int authcode = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
MersenneTwister * mt_rand = NULL;

CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}

CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	authcode = AuthCode;
	return 0;
}

//Type=1001 ��Q����
CQEVENT(int32_t, __eventStartup, 0)() {
	mt_rand = new MersenneTwister((int)time(NULL));
	return 0;
}

//Type=1002 ��Q�˳�
CQEVENT(int32_t, __eventExit, 0)() {
	free(mt_rand);
	return 0;
}

//Type=1003 Ӧ���ѱ�����
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


//Type=1004 Ӧ�ý���ͣ��
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}

//Type=21 ˽����Ϣ
//subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return EVENT_IGNORE;
}


//Type=2 Ⱥ��Ϣ
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	if(!enabled) return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
	CQ_sendGroupMsg(authcode, fromGroup, "");
	return EVENT_BLOCK;
}

//Type=4 ��������Ϣ
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	if(!enabled) return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
	int random_int = mt_rand->rand();
	stringstream ret;
	ret << random_int;
	CQ_sendDiscussMsg(authcode, fromDiscuss, ret.str().c_str());
	return EVENT_BLOCK;
}

//Type=101 Ⱥ�¼�-����Ա�䶯
//subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

//Type=102 Ⱥ�¼�-Ⱥ��Ա����
//subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
//fromQQ ������QQ(��subTypeΪ2��3ʱ����)
//beingOperateQQ ������QQ
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

//Type=103 Ⱥ�¼�-Ⱥ��Ա����
//subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
//fromQQ ������QQ(������ԱQQ)
//beingOperateQQ ������QQ(����Ⱥ��QQ)
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

//Type=201 �����¼�-���������
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

//Type=301 ����-�������
//msg ����
//responseFlag ������ʶ(����������)
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}
