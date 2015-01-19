/** 文 件 名: im_queue.h
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-1
** 描    述:循环队列的实现
** 版    本:
** 修改历史:
** 2014-12-1   张斌创建本文件；
##############################################################################*/

#ifndef IM_QUEUE_H  
#define IM_QUEUE_H 

/******************************************
队列在使用前需定义下面两个值
#define MAXQUEUELEN 20  //队列的最大长度  
typedef int ElemType; //队列的数据类型

*******************************************/
#define MAXQUEUELEN 20 
typedef char *ElemType;

typedef struct{  
    ElemType data[MAXQUEUELEN]; //队列  
    int front; //队头的游标  
    int rear;  //队尾的游标  
}Queue;  
  
void IM_InitQueue(Queue *q); //初始化队列  
  
void IM_EnQueue(Queue *q,ElemType e); //元素e进队  
  
void IM_DeQueue(Queue *q,ElemType *e); //队头的元素出队  
  
int IM_IsEmpty(Queue *q); //判断队列是否为空  
  
int IM_GetQueueLength(Queue *q); //返回队列的长度  
  
void IM_Clear(Queue *q); //清空队列  
  
void IM_Print(Queue *q); //打印队列  
  
#endif //IM_QUEUE_H  


