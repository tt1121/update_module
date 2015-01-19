
#include "json-c/json.h"
extern int im_json_get_string(json_object *json_msg, const char *key, char *value);
extern int im_json_get_int(json_object *json_msg, const char *key, int *value);
extern	void *json_msg_thread(void *arg);


