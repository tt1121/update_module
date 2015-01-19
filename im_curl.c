#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "curl/curl.h"
#include "im_curl.h"



int im_curl_init(CURL **curlhandle)
{
	CURLcode res;

	if (curlhandle == NULL)
	{
		IM_DEBUG("PARAM ERROR");
		return -1;
	}
	
	res = curl_global_init(CURL_GLOBAL_ALL);
	if (res != CURLE_OK)
	{
		IM_DEBUG("curl_global_init fail:%s", curl_easy_strerror(res));	
		return -1;
	}
	
	*curlhandle = curl_easy_init(); 
	if (*curlhandle == NULL)
	{
		IM_DEBUG("curl_easy_init error\n");
		return -1;
	}

	return res;
}

int im_curl_clean(CURL *curlhandle)
{
	if (curlhandle == NULL)
	{
		IM_DEBUG("PARAM ERROR");
		return -1;
	}
	
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();
}

int im_curl_perform(CURL *curlhandle)
{
	CURLcode res;
	long retcode = 0;

	if (curlhandle == NULL)
	{
		IM_DEBUG("PARAM ERROR");
		return -1;
	}
	
	res = curl_easy_perform(curlhandle);
	if (res != CURLE_OK)
	{
		  IM_DEBUG("%s\n", curl_easy_strerror(res));
		  return -1;
	}

	res = curl_easy_getinfo(curlhandle, CURLINFO_RESPONSE_CODE , &retcode);
	if ((res == CURLE_OK)&& ((retcode == 200) || (206 == retcode)))//  206断点续传下载
	{
		IM_DEBUG("download ok retcode:%d", retcode);
	}
	else 
	{
		IM_DEBUG("fail %s  retcode:%d\n", curl_easy_strerror(res), retcode);
		return -1;
	}

	return 0;
	
}

int im_curl_option(CURL *curlhandle, char *url, void *data, pfunc_option im_option)
{
	return im_option(curlhandle, url, data);
}


int im_construct_header(struct curl_slist **headers, pfunc_header im_header_func)
{
	return im_header_func(headers);
}


int im_curl_get_back(CURL *curlhandle, char *url, void *data)
{
	int ret = -1;
	
	if (!curlhandle || !url)
	{
		IM_DEBUG("Bad param!!!");
		return -1;
	}
	
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, 5);  // 设置连接超时，单位秒

	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	
	ret = im_curl_perform(curlhandle);
	

	return ret;

}

int im_curl_post_back(CURL *curlhandle, char *url, void *data)
{
	char *postdata = NULL;
	int ret = -1;
	
	if (!curlhandle || !url || !data)
	{
		IM_DEBUG("Bad param!!!");
		return -1;
	}

	postdata = (char *)data;
	
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, 5);  // 设置连接超时，单位秒

	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	
	curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, postdata); 
	//	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	ret = im_curl_perform(curlhandle);

	return ret;

}

int im_curl_head_back(CURL *curlhandle, char *url, void *data)
{
	int ret = -1;
	if (!curlhandle || !url)
	{
		IM_DEBUG("Bad param!!!");
		return -1;
	}
	
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_HEADER, NULL);
	curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	ret = im_curl_perform(curlhandle);

	return ret;
}


int im_header_back(struct curl_slist **headers)
{
	*headers = curl_slist_append(*headers, "Hey-server-hey: how are you?");
	*headers = curl_slist_append(*headers, "X-silly-content: haha");
	return 0;
}

int im_customer_header_back(CURL *curlhandle, char *url, void *data)
{
	struct curl_slist *headers = NULL;
	int ret = -1;
	pfunc_header p_headfunc = (pfunc_header) data;

	if (!curlhandle || !url || !data)
	{
		IM_DEBUG("Bad param!!!");
		return -1;
	}
	
	im_construct_header(&headers, p_headfunc);
	
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	
	ret = im_curl_perform(curlhandle);
	
	curl_slist_free_all(headers); /* free the header list */
	
	return ret;
}


size_t im_get_contentlength(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	int ret = -1;
	long len = 0;

	ret = sscanf(ptr, "Content-Length: %ld\n", &len);
	if (ret)
	{
      *((long *) stream) = len;
	}
	return size * nmemb;
}

int im_get_remote_file_len_back(long *filesize, char *url)
{
	int err = 0;
	CURL *curlhandle = NULL;
	CURLcode res; 

	err = im_curl_init(&curlhandle);
	if (err < 0)
	{
		IM_DEBUG("im_curl_init");
		return err;
	}
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, 5);  // 设置连接超时，单位秒
	//设置http 头部处理函数
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, im_get_contentlength);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, filesize);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curlhandle);
	if (res != CURLE_OK)
	{
		  IM_DEBUG("%s\n", curl_easy_strerror(res));
		  err = -1;
	}
	IM_DEBUG("filesize:%d", *filesize);

	im_curl_clean(curlhandle);
	return err;
}

size_t im_download_data(void *ptr, size_t size, size_t nmemb, void *stream)  
{
	int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

int im_progress_func(char *progress_data,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
	int per = (int)(d*100.0/t);
	long dlen = (long) (d);
	long tlen = (long) (t);
	
  return 0;
}

int im_curl_download_back(CURL *curlhandle, char *url, void *data)
{
	CURLcode res;			//定义CURLcode类型的变量，保存返回状态码
	FILE *file = NULL;
	long retcode = 0;
	int err = 0;
	struct stat file_info;
	curl_off_t local_file_len = -1 ;
	int use_resume = 0;
	char *local_url = NULL;
	
	if(!curlhandle || !url || !data)
	{
		IM_DEBUG("Bad argument");
		return -1;
	}
	
	
	err = im_curl_init(&curlhandle);
	if (err < 0)
	{
		IM_DEBUG("im_curl_init");
		return err;
	}

	
	local_url = (char *)data;
	if(stat(local_url, &file_info) == 0) 
	{
		local_file_len =  file_info.st_size;
		use_resume	= 1;
	}


	file = fopen(local_url, "ab+"); 
	if (file == NULL) 
	{
		IM_DEBUG("open file %s fail", local_url);
		err = -1;
		goto openfile_err;
	}

	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, 5);  // 设置连接超时，单位秒

	// 设置文件续传的位置给libcurl
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, im_download_data);
	//跟踪下载进度
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curlhandle, CURLOPT_PROGRESSFUNCTION, im_progress_func);
	curl_easy_setopt(curlhandle, CURLOPT_PROGRESSDATA, "*");

	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 0L);
	
	err = im_curl_perform(curlhandle);

out:
		fclose(file);
openfile_err:
		im_curl_clean(curlhandle);
	
	return err;
}


size_t im_update_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int n = 0;
	
	n = fread((char *)ptr, size, nmemb, (FILE *)stream);
	//printf("%s ptr=%s\n",__func__, (char *)ptr);
	return n;
}

int im_curl_upload_back(CURL *curlhandle, char *url, void *data)
{
	int ret = 0;
	char *local_url = NULL;
	FILE *file = NULL;

	if (!curlhandle || !url || !data)
	{
		IM_DEBUG("Bad param!!!");
		return -1;
	}
	
	local_url = (char *)data;
	file = fopen(local_url, "r");
	if (file == NULL)
	{
		IM_DEBUG("fopen error:%s", local_url);
		return -1;
	}
	
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);

	curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, im_update_data);
	curl_easy_setopt(curlhandle, CURLOPT_READDATA, file);

	curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
	
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	
	ret = im_curl_perform(curlhandle);
	fclose(file);
	
	return ret;

}

int test(char *p_url)
{
	CURL *curlhandle = NULL;			 //定义CURL类型的指针  
	int res = -1;  
	char url[100] = {0};
	
	strcpy(url, p_url);
	
	res = im_curl_init(&curlhandle);
	if (res < 0)
	{
		IM_DEBUG("im_curl_init fail\n");
		return -1;
	}
	
	res = im_curl_option(curlhandle, url, "/tmp/123.txt", im_curl_download_back);
	if (res < 0)
	{
		IM_DEBUG("im_curl_get fail\n");
		return -1;
	}
	
	res = im_curl_clean(curlhandle);
	if (res < 0)
	{
		IM_DEBUG("im_curl_clean fail\n");
		return -1;
	}

	return 0;
}


int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("usage:\n");
		printf("cmd url\n");
		return 0;
	}
	test(argv[1]);
}

