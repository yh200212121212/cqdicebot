#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include "MersenneTwister.h"
using namespace std;

int authcode = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;
MersenneTwister * mt_rand = NULL;

CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}

CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	authcode = AuthCode;
	return 0;
}

//Type=1001 酷Q启动
CQEVENT(int32_t, __eventStartup, 0)() {
	mt_rand = new MersenneTwister((int)time(NULL));
	return 0;
}

//Type=1002 酷Q退出
CQEVENT(int32_t, __eventExit, 0)() {
	free(mt_rand);
	return 0;
}

//Type=1003 应用已被启用
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


//Type=1004 应用将被停用
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}

//Type=21 私聊消息
//subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	return EVENT_IGNORE;
}


//Type=2 群消息
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	if(!enabled) return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
	CQ_sendGroupMsg(authcode, fromGroup, "");
	return EVENT_BLOCK;
}

//Type=4 讨论组消息
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	if(!enabled) return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
	int random_int = mt_rand->rand();
	stringstream ret;
	ret << random_int;
	CQ_sendDiscussMsg(authcode, fromDiscuss, ret.str().c_str());
	return EVENT_BLOCK;
}

//Type=101 群事件-管理员变动
//subType 子类型，1/被取消管理员 2/被设置管理员
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

//Type=102 群事件-群成员减少
//subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
//fromQQ 操作者QQ(仅subType为2、3时存在)
//beingOperateQQ 被操作QQ
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

//Type=103 群事件-群成员增加
//subType 子类型，1/管理员已同意 2/管理员邀请
//fromQQ 操作者QQ(即管理员QQ)
//beingOperateQQ 被操作QQ(即加群的QQ)
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

//Type=201 好友事件-好友已添加
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

//Type=301 请求-好友添加
//msg 附言
//responseFlag 反馈标识(处理请求用)
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}
