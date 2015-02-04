#ifndef __IM_DOWNLOAD_H__
#define __IM_DOWNLOAD_H__






struct if_data  
{     
    /*  generic interface information */  
    u_long ifi_opackets;    /*  packets sent on interface */  
    u_long ifi_ipackets;    /*  packets received on interface */  
    u_long ifi_obytes;      /*  total number of octets sent */  
    u_long ifi_ibytes;      /*  total number of octets received */  
};  

struct if_info  
{     
    char ifi_name[16];  
    unsigned long ifi_ibytes;  
    unsigned long ifi_obytes;  
};  
struct if_speed  
{     
    char ifs_name[16];  
    unsigned long ifs_ispeed;  
    unsigned long ifs_ospeed;  
    unsigned long ifs_us;  
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



extern 	int im_get_if_speed(struct if_speed *ndev);

#endif

