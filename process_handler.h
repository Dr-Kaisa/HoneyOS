#ifndef __PROCESS_HANDLER_H__
#define __PROCESS_HANDLER_H__

// 包含必要的头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义宏
#define ready 1
#define runing 0

// 定义进程控制块
typedef struct
{
    int pid;
    char name[80];
    int status;
    int time;

} PCB;

// 定义队列节点结构体
typedef struct Process
{
    PCB pcb;
    struct Process *next;
    struct Process *prev;
} Process;

// 定义队列结构体
typedef struct Queue
{
    Process *front; // 队头
    Process *rear;  // 队尾
} Queue;

void initQueue(Queue *queue);
Process *create_process(int pid, char *name, int status, int time);
void enqueue(Queue *queue, Process *new_process);
void dequeue(Queue *queue);
void printQueue(Queue *queue);
void write_queue_to_file(Queue *queue);
void read_queue_from_file(Queue *queue);
Process *find_process(Queue *queue, char *name);
void shut_down_process(Queue *queue, Process *target);
void run(Queue *queue);

#endif