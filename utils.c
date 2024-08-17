#include "utils.h"
// 获取当前时间字符串并传给指针
void get_now_time(char *timeString)
{

    time_t current_time;
    struct tm *time_info;

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(timeString, 21, "%Y-%m-%d %H:%M:%S", time_info);
}
// 传入文件指针，返回文件所占字节数,只能计算文件的大小，不能计算文件夹
long int get_file_size(FILE *fp)
{

    int len;
    if (fp == NULL)
    {
        perror("打开文件错误");
        return (-1);
    }
    fseek(fp, 0, SEEK_END);

    len = ftell(fp);

    return len;
}

// 计算A类不确定度
void work_typeA_uncertainty()
{

    int n;
    printf("数据共有几组？请输入(3<n<11)\n");
    scanf("%d", &n);
    if (n < 3)
    {
        printf("数据过少!\n");
        return;
    }
    getchar();
    double d = n;
    double tp, count, sum1 = 0, sum2, averdata = 0;
    double data[10];
    printf("请输入数据（最多保留五位小数）\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%lf", &data[i]);
        sum1 = data[i] + sum1;
    }
    averdata = sum1 / d;
    switch (n - 1)
    {
    case 2:
        tp = 1.32;
        break;
    case 3:
        tp = 1.20;
        break;
    case 4:
        tp = 1.14;
        break;
    case 5:
        tp = 1.11;
        break;
    case 6:
        tp = 1.09;
        break;
    case 7:
        tp = 1.08;
        break;
    case 8:
        tp = 1.07;
        break;
    case 9:
        tp = 1.06;
        break;
    case 10:
        tp = 1.05;
        break;
    }
    for (int i = 0; i < n; i++)
    {
        sum2 = sum2 + pow(data[i] - averdata, 2);
    }
    double cc = tp * sqrt(sum2 / d / (d - 1));
    printf("A类不确定度为：%f\n", cc);
}

// 在当前目录创建root目录
void create_root_dir()
{
#ifdef _WIN32
    mkdir("root");
#else
    mkdir("root", 0777);
#endif
}