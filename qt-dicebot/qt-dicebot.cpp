#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
using namespace std;

int authcode = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;
mt19937 * mtrand = NULL;
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}

CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	authcode = AuthCode;
	return 0;
}

//Type=1001 酷Q启动
CQEVENT(int32_t, __eventStartup, 0)() {
	random_device rdd;
	mtrand = new mt19937(rdd());
	return 0;
}

//Type=1002 酷Q退出
CQEVENT(int32_t, __eventExit, 0)() {
	free(mtrand);
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
	
	unsigned int random_int = (*mtrand)();
	stringstream ret;
	ret << random_int;
	CQ_sendDiscussMsg(authcode, fromDiscuss, ret.str().c_str());
	return EVENT_BLOCK;
}
