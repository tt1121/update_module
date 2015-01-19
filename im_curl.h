

#ifndef __IM_CURL_H__
#define __IM_CURL_H__




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

typedef int (*pfunc_header)(struct curl_slist **headers);
typedef int (*pfunc_option)(CURL *curlhandle, char *url, void *data);
//typedef size_t (*pfunc_iorw)(void *ptr, size_t size, size_t nmemb, void *stream);

extern 	int im_curl_init(CURL **curlhandle);
extern  int im_curl_clean(CURL *curlhandle);
extern  int im_curl_perform(CURL *curlhandle);
extern  int im_curl_option(CURL *curlhandle, char *url, 
								void *data, pfunc_option im_option);
extern  int im_construct_header(struct curl_slist **headers, 
										pfunc_header im_header_func);
extern  int im_customer_header_back(CURL *curlhandle, char *url, void *data);

#endif

