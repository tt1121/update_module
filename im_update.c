/** 文 件 名: im_update.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-1
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-1   张斌创建本文件；
##############################################################################*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "msg.h"
#include "im_update.h"
#include "im_queue.h"
#include "im_json_parse.h"



#define UPDATE_MODULE		"im_update"




int init_jason_pthread(void)
{
	int ret = -1;
	int jason_pid = -1;
	
	ret = pthread_create(&jason_pid, NULL, (void *)json_msg_thread, NULL);
	if (ret < 0)
    { 
    	return ret;
    }
    pthread_detach(jason_pid);
}

int main (int argc, char *argv[])
{
	int server_socket = -1;
	int accept_socket = -1;
	int ret = -1;

	/*消息格式为:{"header":{"cmd":%d, "ver":%d, "seq":%d, 
	 "device":%d, "appid": %d,"code":%d, "sessionid":"%s", "sign":""}, 
	 "data":[]}
	*/
	char *p_msg = NULL;


	if (init_jason_pthread() < 0)
	{
		IM_DEBUG("init_jason_pthread error");
		return -1;
	}
	
	server_socket = IM_DomainServerInit(UPDATE_MODULE);  //初始化本地通信socket
	if (server_socket < 0)
	{
		IM_DEBUG("IM_DomainServerInit error");
		return -1;
	}
	
	IM_InitQueue(&msg_queue);

	while(1)
    {
    	accept_socket = IM_ServerAcceptClient(server_socket);
		if (accept_socket < 0 )
		{
			if (accept_socket == EINTR)
			{
				continue;
			}
			else
			{
				perror("IM_ServerAcceptClient ERROR");
				return -1;
			}
		}

		ret = IM_MsgReceive(accept_socket, &p_msg, NULL);	
		if (ret < 0)
        {
            IM_DEBUG("msg receive failed accept_socket = %d!\n",  accept_socket);
            IM_DomainServerDeinit(accept_socket);
			continue;
        }

		IM_EnQueue(msg_queue, p_msg);
		
	}
	
}

