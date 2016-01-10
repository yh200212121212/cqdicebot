#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
using namespace std;

int authcode = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
mt19937 * mtrand = NULL;
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}

CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	authcode = AuthCode;
	return 0;
}

//Type=1001 ��Q����
CQEVENT(int32_t, __eventStartup, 0)() {
	random_device rdd;
	mtrand = new mt19937(rdd());
	return 0;
}

//Type=1002 ��Q�˳�
CQEVENT(int32_t, __eventExit, 0)() {
	free(mtrand);
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
	
	unsigned int random_int = (*mtrand)();
	stringstream ret;
	ret << random_int;
	CQ_sendDiscussMsg(authcode, fromDiscuss, ret.str().c_str());
	return EVENT_BLOCK;
}
