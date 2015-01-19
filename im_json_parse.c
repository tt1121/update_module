/** 文 件 名: im_json_parse.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-1
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-1   张斌创建本文件；
##############################################################################*/

#include "im_update.h"
#include "json.h"



extern 	int im_handle_msg(S_IM_MSG *msg);


/*********************************************
function:   获取JSON对象的字符串值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_get_string(json_object *json_msg, const char *key, char *value)
{
	json_object *my_obj = NULL;
	char *p_msg = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return -1;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return -1;
	}
	p_msg  = json_object_get_string(my_obj);
	strcpy(value, p_msg);

	json_object_put(my_obj);
	return 0;
}


/*********************************************
function:   获取JSON对象的整型值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_get_int(json_object *json_msg, const char *key, int *value)
{
	json_object *my_obj = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return -1;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return -1;
	}
	*value = json_object_get_int(my_obj);
	
	json_object_put(my_obj);
	return 0;
}


/*********************************************
function:   解析JSON对象
params:
[IN]p_msg:    json字符串
return: -1，0
***********************************************/
int im_json_parse(const char *p_msg)
{
	json_object *json_msg = NULL;
	json_object *json_header = NULL;
	json_object *im_data = NULL;
	json_object *update_obj = NULL;
	int ret = 0;

	if (p_msg == NULL)
	{
		IM_DEBUG("p_msg is null param error");
		return -1;
	}
	
	json_msg = json_tokener_parse(p_msg);
    if (is_error(json_msg)) 
    {
        IM_DEBUG("json_tokener_parse failed!\n");
		return -1;
    }
		
	json_header = json_object_object_get(json_msg, jason_key[JSON_HEADER]);
	if (json_header == NULL)
	{
		IM_DEBUG("json parse header fail\n");
		return -1;
	}
	//解析消息头
	ret |= im_json_get_int(json_header, jason_key[JSON_CMD], &s_im_clould_cmd.im_cmd);
	ret |= im_json_get_int(json_header, jason_key[JSON_VER], &s_im_clould_cmd.im_ver);
	ret |= im_json_get_int(json_header, jason_key[JSON_SEQ], &s_im_clould_cmd.im_seq);
	ret |= im_json_get_int(json_header, jason_key[JSON_DEVICE], &s_im_clould_cmd.im_device);
	ret |= im_json_get_int(json_header, jason_key[JSON_APPID], &s_im_clould_cmd.im_appid);
	ret |= im_json_get_int(json_header, jason_key[JSON_CODE], &s_im_clould_cmd.im_code);
	ret |= im_json_get_string(json_header, jason_key[JSON_SESSIONID], s_im_clould_cmd.im_sessionid);
	ret |= im_json_get_string(json_header, jason_key[JSON_SIGN], s_im_clould_cmd.im_sign);

	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data fail\n");
		return -1;
	}
	update_obj = json_object_array_get_idx(im_data, 0);
	if (update_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail\n");
		return -1;
	}
	ret |= im_json_get_string(update_obj, jason_key[JSON_SWNAME], s_im_clould_cmd.im_sw_name);
	ret |= im_json_get_string(update_obj, jason_key[JSON_URL], s_im_clould_cmd.im_url);
	ret |= im_json_get_string(update_obj, jason_key[JSON_MD5], s_im_clould_cmd.im_md5);

	if (ret < 0)
	{
		IM_DEBUG("json parse error");
	}
#if 0
	printf("im_cmd:[%d][%d][%d][%d][%d][%d]\n[%s][%s][%s][%s][%s]\n",
		s_im_clould_cmd.im_cmd,s_im_clould_cmd.im_ver,s_im_clould_cmd.im_seq,
		s_im_clould_cmd.im_device,s_im_clould_cmd.im_appid,s_im_clould_cmd.im_code,
		s_im_clould_cmd.im_sessionid,s_im_clould_cmd.im_sign,s_im_clould_cmd.im_sw_name,
		s_im_clould_cmd.im_url,s_im_clould_cmd.im_md5);
#endif
	json_object_put(update_obj);
	json_object_put(im_data);
	json_object_put(json_header);
	json_object_put(json_msg);
	
	return ret;
}


void *json_msg_thread(void *arg)
{
	char *p_msg = NULL;
	
	while(1)
	{
		if (IM_IsEmpty(msg_queue))
		{
			usleep(500);
		}

		IM_DeQueue(msg_queue, &p_msg); //取出消息
		
		if (im_json_parse(p_msg) == 0) //解析成功
		{
			im_handle_msg(s_im_clould_cmd);
		}
		else	//解析失败
		{
			
		}
		
	}
}


