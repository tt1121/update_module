/** 文 件 名: im_queue.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-1
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-1   张斌创建本文件；
##############################################################################*/


void IM_InitQueue(Queue *q) //初始化队列  
{  
    q->front = q->rear = 0;  
} 

void IM_EnQueue(Queue *q, ElemType e) //让元素e进队  
{  
    if((q->rear + 1) % MAXQUEUELEN == q->front) //队列已满  
        return;  
    q->data[q->rear] = e; //元素e进队  
    q->rear = (q->rear + 1) % MAXQUEUELEN; //游标rear上前进一位,如果已达最后,就移到前面  
}  

void IM_DeQueue(Queue *q, ElemType *e) //队头的元素出队存入*e  
{  
    if(q->rear == q->front) //如果队列为空返回  
        return;  
    *e = q->data[q->front]; //返回队头的元素  
    q->front = (q->front + 1) % MAXQUEUELEN; //游标front向前移一位,如果是队列的末尾移动到最前面  
} 

bool IM_IsEmpty(Queue *q) //判断队列是否为空  
{  
    return q->front == q->rear ? true : false;  
} 

int IM_GetQueueLength(Queue *q) //返回队列的长度  
{  
    return (q->rear - q->front + MAXQUEUELEN) % MAXQUEUELEN;  
} 

void IM_Clear(Queue *q) //清空队列  
{  
    q->front = q->rear = 0;  
} 

void IM_Print(Queue *q) //打印队列  
{  
#if 0
    if(q->front == q->rear)  
        return;  
    else if(q->rear < q->front)  
    {  
        for(int i = q->front;i < MAXQUEUELEN;++i)  
            printf("%d ",q->data[i]);  
        for(int i = 0;i < q->rear;++i)  
            printf("%d ",q->data[i]);  
        printf("\n");  
    }  
    else{  
        for(int i = q->front;i < q->rear;++i)  
            printf("%d ",q->data[i]);  
        printf("\n");  
    }  
#endif
}  


