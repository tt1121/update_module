/** 文 件 名: im_download.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-15
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-15   张斌创建本文件；
##############################################################################*/


#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <assert.h>  
#include <error.h>  
//#include <linux/if.h > 
    /* For "open" function */  
#include <ifaddrs.h>

#include "im_getNetBand.h"


#define IFF_UP		0x01
#define IFF_RUNNING 0x40


void im_print_ifdata(struct if_data *ifa)
{
	printf("[%d] [%d] [%d]  [%d]\n",
		ifa->ifi_opackets,	/*	packets sent on interface */  
		ifa->ifi_ipackets,	/*	packets received on interface */  
		ifa->ifi_obytes,		/*	total number of octets sent */	
		ifa->ifi_ibytes		/*	total number of octets received */	
	);
	
}

/*********************************************
function:   获取指定接口RX,TX的字节数
params:
[IN]ndev:    传入接口名返回RX,TX数据
return: -1，0
***********************************************/
int im_get_if_dbytes(struct if_info* ndev)  
{     
    assert(ndev);  
      
    struct ifaddrs *ifa_list = NULL;  
    struct ifaddrs *ifa = NULL;  
    struct if_data *ifdata = NULL;  
    int     ret = 0;  
      
    ret = getifaddrs(&ifa_list);  
    if(ret < 0) 
	{   
        IM_DEBUG("Get Interface Address Fail:");  
        goto end;  
    }     
      
    for(ifa=ifa_list; ifa; ifa=ifa->ifa_next)
	{  
        if(!(ifa->ifa_flags & IFF_UP) && !(ifa->ifa_flags & IFF_RUNNING))  
            continue;  
        if(ifa->ifa_data == 0)  
            continue;  
          
        ret = strcmp(ifa->ifa_name, ndev->ifi_name);  
        if(ret == 0)
		{  
           ifdata= (struct if_data *)ifa->ifa_data;  
           ndev->ifi_ibytes = ifdata->ifi_ibytes;  
           ndev->ifi_obytes = ifdata->ifi_obytes;  
           break;  
        }  
    }  
  
    freeifaddrs(ifa_list);  

	
end:  
    return (ret ? -1 : 0);  
}  

/********************************************************
function:   获取指定接口指定时间内的数据变化
params:
[IN]ndev:    传入接口名和监控时间
		    返回RX,TX变化速度
return: -1，0
**********************************************************/
int im_get_if_speed(struct if_speed *ndev)  
{  
    assert(ndev);   
  
	struct if_info *p_info1 = NULL; 
	struct if_info *p_info2 = NULL;
	int ret = 0;	
  
    p_info1 = (struct if_info *)malloc(sizeof(struct if_info));  
    p_info2 = (struct if_info *)malloc(sizeof(struct if_info));  
    bzero(p_info1, sizeof(struct if_info));  
    bzero(p_info2, sizeof(struct if_info));  
  
    strncpy(p_info1->ifi_name, ndev->ifs_name, strlen(ndev->ifs_name));  
    strncpy(p_info2->ifi_name, ndev->ifs_name, strlen(ndev->ifs_name));  
  
    ret = im_get_if_dbytes(p_info1);  
    if(ret < 0)     
	{
		goto end; 
	}
	
    usleep(ndev->ifs_us);  
	
    ret = im_get_if_dbytes(p_info2);  
    if(ret < 0)
	{   
		goto end;  
	}
	
    ndev->ifs_ispeed = p_info2->ifi_ibytes - p_info1->ifi_ibytes;  
    ndev->ifs_ospeed = p_info2->ifi_obytes - p_info1->ifi_obytes;  
  
end:  
    free(p_info1);  
    free(p_info2);  
  
    return ret;  
}  


void im_test(void)
{  
	struct if_speed ndev;  
	int ret = 0;  
	float ispeed ,ospeed; 
	
	bzero(&ndev,sizeof(ndev));	
	sprintf(ndev.ifs_name,"eth1");	
  
	ndev.ifs_us = 1000000;	
  
	printf("Get %s Speed");  
	ret = im_get_if_speed(&ndev);  
	if(ret < 0)  
		printf("\t\t\t[Fail]\n");  
	else  
		printf("\t\t\t[OK]\n");  
	
	while(1)
	{  
		ispeed = ndev.ifs_ispeed * 1.0/(ndev.ifs_us/1000 * 0.001);	
		ospeed = ndev.ifs_ospeed * 1.0/(ndev.ifs_us/1000 * 0.001);	

		if (ospeed < 1024)
		{
			printf("%s: Up Speed: %f B/s || Down Speed: %f B/s					 \r\n",  
				ndev.ifs_name, ispeed, ospeed); 
		}
		else if (ospeed > 1024*1024)
		{
			printf("%s: Up Speed: %f MB/s || Down Speed: %f MB/s				   \r\n",  
				ndev.ifs_name,ispeed/(1024.0*1024.0),ospeed/(1024.0*1024.0)); 
		}
		else
		{
			printf("%s: Up Speed: %f KB/s || Down Speed: %f KB/s				   \r\n",  
				ndev.ifs_name,ispeed/(1024.0),ospeed/(1024.0));  
		}
		im_get_if_speed(&ndev);  
	}  
  
  
	return 0;  
}


int main (int argc, char **argv)  
{  
	im_test();  
  
    return 0;  
}


