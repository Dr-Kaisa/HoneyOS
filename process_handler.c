#include "process_handler.h"
// 初始化队列
void initQueue(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}
// 创建新进程
Process *create_process(int pid, char *name, int status, int time)
{
    Process *newProcess = (Process *)malloc(sizeof(Process));
    if (newProcess == NULL)
    {
        printf("内存分配失败\n");
        return NULL;
    }
    // 进程初始化
    newProcess->pcb.pid = pid;
    strcpy(newProcess->pcb.name, name);
    newProcess->pcb.status = status;
    newProcess->pcb.time = time;

    newProcess->prev = NULL;
    newProcess->next = NULL;
    return newProcess;
}
// 将新进程并加入进程队列
void enqueue(Queue *queue, Process *new_process)
{
    // 如果队列为空
    if (queue->rear == NULL)
    {
        queue->front = new_process;
        queue->rear = new_process;
    }
    else
    {
        // 新节点接在队尾
        queue->rear->next = new_process;
        // 新节点的prev指向上原来的尾节点
        new_process->prev = queue->rear;
        // 尾节点后移
        queue->rear = new_process;
    }
}

// 出队操作
void dequeue(Queue *queue)
{
    if (queue->front == NULL)
    {
        printf("队列为空\n");
        return;
    }

    Process *temp = queue->front;

    if (queue->front == queue->rear)
    {
        queue->front = NULL;
        queue->rear = NULL;
    }
    else
    {
        // 队头元素的下一个元素的prev指针赋空
        queue->front->next->prev = NULL;
        // 队头后移
        queue->front = queue->front->next;
    }

    free(temp);
    return;
}

// 打印队列元素
void printQueue(Queue *queue)
{
    Process *current = queue->front;
    if (current == NULL)
    {
        printf("队列为空\n");
        return;
    }
    printf("==============================================================================================================\n");
    printf("进程ID              进程名              进程状态          剩余时间            \n");
    while (current != NULL)
    {
        printf("%-20d%-20s%-20s%-20d\n", current->pcb.pid, current->pcb.name, current->pcb.status ? "就绪" : "运行", current->pcb.time);
        current = current->next;
    }
}

// 将队列存入文件
void write_queue_to_file(Queue *queue)
{
    Process *current = queue->front;
    if (current == NULL)
    {
        printf("队列为空\n");
        return;
    }
    FILE *file = fopen("process_queue.txt", "w");
    if (file == NULL)
    {
        printf("无法创建文件进行读写！\n");
        return;
    }
    while (current != NULL)
    {
        // 暂存该节点，等下用来free
        Process *temp = current;
        // 该节点写入文件
        fprintf(file, "%d %s %d %d\n", current->pcb.pid, current->pcb.name, current->pcb.status, current->pcb.time);
        current = current->next;
        // 释放内存
        free(temp);
    }
    fclose(file);
}
// 将队列从文件中读取
void read_queue_from_file(Queue *queue)
{

    FILE *file = fopen("process_queue.txt", "r");
    if (file == NULL)
    {
        printf("无法打开文件进行读取或文件不存在！\n");
        return;
    }

    while (!feof(file))
    {
        int pid, status, time;
        char name[80];
        if (fscanf(file, "%d %s %d %d\n", &pid, name, &status, &time) == 4)
        {
            Process *newProcess = create_process(pid, name, status, time);
            enqueue(queue, newProcess);
        }
    }

    fclose(file);
}

// 根据name找到进程并返回
Process *find_process(Queue *queue, char *name)
{
    Process *current = queue->front;
    if (current == NULL)
    {
        printf("队列为空!\n");
        return NULL;
    }

    while (current != NULL)
    {
        if (strcmp(current->pcb.name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    // 没找到进程
    return NULL;
}

// 将进程从队列中移除
void shut_down_process(Queue *queue, Process *target)
{
    // 移除的是队头
    if (queue->front == target)
    {
        // 下一个元素的prev赋NULL
        target->next->prev = NULL;
        // 队头后移
        queue->front = target->next;
        free(target);
        return;
    }
    // 移除的是队尾
    if (queue->rear == target)
    {
        // 上一个元素的next赋NULL
        target->prev->next = NULL;
        // 队尾前移
        queue->rear = target->prev;
        free(target);
        return;
    }
    // 上一个节点的next指向下一个节点
    target->prev->next = target->next;
    // 下一个节点的prev指向上一个节点
    target->next->prev = target->prev;
    free(target);
}
// 将队头元素设为运行状态，并减少其运行时间，若时间小于等于0则移除队列
void run(Queue *queue)
{
    // 进程队列为空
    if (!queue->front)
    {
        return;
    }
    Process *head = queue->front;
    head->pcb.status = runing;
    head->pcb.time -= 10;
    if (head->pcb.time <= 0)
    {
        dequeue(queue);
    }
}