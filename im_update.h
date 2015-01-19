/** 文 件 名: im_update.h
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-1
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-1   张斌创建本文件；
##############################################################################*/
#ifndef __IM_UPDATE_H__
#define __IM_UPDATE_H__
#include "im_queue.h"

#define CMD_UPDATE_PACKAGE		1
#define CMD_UPDATE_FIRMWARE		2

#define FIRMWARE_DOWNLOAD_PATH          "/tmp/"
#define DOWNLOAD_PATH_LEN               256
#define DOWNLOAD_TIME_OUT   			60		//60 s
#define MAX_STAS_LEN        (18*64)


#define MAX_SESSIONID   33		//im_sessionid数组的长度
#define MAX_SIGN		33		//im_sign 数组的长度
#define MAX_STRING		33

//返回消息码类型
enum MSG_CODE  
{
	MSG_CODE_SUCESS,
	MSG_CODE_UNKNOWN,		//未知错误
	MSG_CODE_TIMEOUT,		//超时
	MSG_CODE_PORT_ERR,		//端口错误
	MSG_CODE_FORMAT_ERR,	//格式错误
	MSG_CODE_ATTR_ERR,		//属性错误
	MSG_CODE_NO_ATTR,		//缺少属性
	MSG_CODE_NO_PERMIT		//没有权限
};

//交互命令结构体中im_device 类型
enum IM_DEVICE
{
	IM_DEV_ANDROID 	= 1,
	IM_DEV_IOS		= 2,
	IM_DEV_PC		= 3
};	

//交互命令结构体
typedef struct 
{
	int 	im_cmd;
	int 	im_ver;
	int 	im_seq;
	int 	im_device;
	int 	im_appid;
	int 	im_code;
	char 	im_sessionid[MAX_SESSIONID];
	char 	im_sign[MAX_SIGN];
	char 	im_sw_name[MAX_STRING];
	char	im_url[MAX_STRING];
	char 	im_md5[MAX_STRING];
}S_IM_MSG;


//JSON命令KEY值索引
enum JSON_IM_CMD
{
	JSON_HEADER,
	JSON_CMD,
	JSON_VER,
	JSON_SEQ,
	JSON_DEVICE,
	JSON_APPID,
	JSON_CODE,
	JSON_SESSIONID,
	JSON_SIGN,
	JSON_DATA,
	JSON_SWNAME,
	JSON_URL,
	JSON_MD5
};

//JSON 命令的KEY
const char *jason_key[]=
{
	"header",
	"cmd",
	"ver",
	"seq",
	"device",
	"appid",
	"code",
	"sessionid",
	"sign",
	"data",
	"sw_name",
	"url",
	"md5"
};



#define IM_DEBUG_FLAG 1


#if IM_DEBUG_FLAG
#define IM_DEBUG(format, ...) \
    do { \  
        fprintf(stderr, "[%s@%s,%d] " format "\n", \
             __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)  
#else
#define IM_DEBUG(format, ...)
#endif


//定义全局变量
Queue msg_queue;
S_IM_MSG s_im_clould_cmd;




#endif // end __IM_UPDATE_H__
