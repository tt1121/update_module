#include "im_update.h"


extern 	int im_check_md5(char *md5, char *sw_name);
extern 	int im_download_wget(char *url, char *sw_name, int time_out);


void im_upgrade(char updatecmd, char *sw_name, char *url, char *md5)
{
	int ret = -1;
	char cmd[DOWNLOAD_PATH_LEN] = {0};
	
	ret = im_download_wget(url, sw_name, DOWNLOAD_TIME_OUT);
	if (ret != 0)
	{
		IM_DEBUG("download file fail");
		return -1;
	}
	
	ret = im_check_md5(md5, sw_name);
	if (ret != 0)
	{
		return -1;
	}

	snprintf(cmd, DOWNLOAD_PATH_LEN-1, "%s "FIRMWARE_DOWNLOAD_PATH"%s", updatecmd,sw_name);
	system(cmd);

}


int im_handle_msg(S_IM_MSG *msg)
{
	int ret = -1;
	if (msg == NULL)
	{
		IM_DEBUG("msg is null param error");
		return -1;
	}
	
	
	switch(msg->im_cmd)
	{
		case CMD_UPDATE_PACKAGE:
			im_upgrade("opkg",msg->im_sw_name);
			break;
		case CMD_UPDATE_FIRMWARE:
			im_upgrade("sysupgrade",msg->im_sw_name);
			break;
		default:
			IM_DEBUG("unkonw command:%d", msg->im_cmd);
			break;
	}

	return 0;
}
