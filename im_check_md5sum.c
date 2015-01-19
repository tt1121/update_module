#include <stdlib.h>
#include <stdio.h>
#include "im_update.h"


int get_cmd_value(char *name, char *value)
{
    char cmd[DOWNLOAD_PATH_LEN] = {0};
    FILE* fp = NULL;
    
    snprintf(cmd, sizeof(cmd) - 1, "%s", name);
    fp = popen(cmd, "r");
    if (!fp)
    {
    	IM_DEBUG("popen fail");
    	return -1;
    }

	fgets(value, MAX_STAS_LEN - 1, fp);
	if (value[strlen(value)-1] == 0x0a)
		value[strlen(value)-1] = '\0';
    pclose(fp);
    
    return 0;
}

int im_check_md5(char *md5, char *sw_name)
{
	char cmd[DOWNLOAD_PATH_LEN] = {0};
    char md5_str[64] = {0};
	int ret = -1;
	
	snprintf(cmd, DOWNLOAD_PATH_LEN-1, "md5sum "FIRMWARE_DOWNLOAD_PATH"%s.bin | cut -d ' ' -f 1", sw_name);
	ret = get_cmd_value(cmd, md5_str);
	if (ret != 0)
	{
		IM_DEBUG("(%s) failed!\n", cmd);
		return -1;
	}

	if (strcmp(md5, md5_str) != 0)
	{
		IM_DEBUG("MD5 not match");
		return -1;
	}


	return 0;
}



