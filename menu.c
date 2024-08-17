#include "menu.h"

// 显示主菜单
void show_main_menu()
{
    int section = 5;
    do
    {
        printf("==============================================================================================================\n");
        printf("请选择你的的操作:\n\t1、打开资源管理器\n\t2、打开进程管理器\n\t3、计算A类不确定度\n\t4、退出系统\n");
        scanf("%d", &section);
        getchar();
        switch (section)
        {
        case 1:
            enter_file_manager();
            break;
        case 2:
            enter_process_manager();
            break;
        case 3:
            work_typeA_uncertainty();
            break;
        case 4:
            printf("正在退出...\n");
            break;
        default:
            printf("请输入正确数字\n");
            break;
        }
    } while (section != 4);
}
// 进入文件资源管理器
void enter_file_manager()
{
    printf("==============================================================================================================\n");
    printf("欢迎使用文件资源管理器喵~\n");
    printf("==============================================================================================================\n");
    printf("正在加载根目录，请稍候喵~\n");
    printf("==============================================================================================================\n");
    // 当前节点
    extern Node *now_node;
    // 目标节点
    extern Node *target_node;
    // 当前文件路径字符串
    extern char now_path[200];
    // 创建根节点
    Node *root = NULL;
    // 将文件树从文件中加载进来
    root = load_tree_from_file();
    // 文件树不存在，说明是第一次使用，创建一个root节点,同时在宿主系统创建root文件夹
    if (!root)
    {

        // 文件树根节点初始化
        root = (Node *)malloc(sizeof(Node));
        char time_str[30];
        get_now_time(time_str);

        root->data.isDir = 1;
        root->data.size = 0;
        strcpy(root->data.create_time, time_str);
        strcpy(root->data.name, "root");
        root->last = NULL;
        root->left = NULL;
        root->right = NULL;
    }

    // 令当前节点为根节点
    now_node = root;
    // 令当前路径为根目录
    strcpy(now_path, "root");

    // 调用进入目录时的菜单
    show_enter_file_menu();

    // 将文件树写入文件
    save_tree_to_file(root);
    root = NULL;
    printf("已退出文件资源管理器，欢迎下次使用喵~\n");
}
// 选择文件时的菜单
void show_select_file_menu()
{
    int section = 5;
    do
    {
        // 先展示当前文件夹中的内容
        show_file_list();

        printf("==============================================================================================================\n");
        printf("请选择你的的操作:\n\t1、写点东西进去\n\t2、浏览文件\n\t3、删除文件\n\t4、进入文件夹\n\t5、取消选中\n");
        scanf("%d", &section);
        getchar();
        switch (section)
        {
        // 向文件写入
        case 1:
            printf("请输入一行要写入的内容\n");
            char line_to_write[300];
            scanf("%s", line_to_write);
            write_to_file(line_to_write);
            break;
        // 输出文件全部内容
        case 2:
            print_file();
            break;
        // 删除文件
        case 3:
            delete_file();
            break;
        // 进入文件夹
        case 4:
            cd();
            show_enter_file_menu();
            break;
        // 取消操作
        case 5:
            printf("已取消!\n");
            break;
        default:
            printf("请输入正确数字\n");
            break;
        }
    } while (!(section == 3 || section == 5));
}
// 进入文件夹时的菜单
void show_enter_file_menu()
{

    int section = 4;
    do
    {
        // 显示菜单前先展示当前目录中的文件
        show_file_list();
        printf("==============================================================================================================\n");
        printf("请选择你的的操作:\n\t1、搜索文件夹\n\t2、创建文件\n\t3、选中文件\n\t4、返回\n");
        scanf("%d", &section);
        getchar();
        switch (section)
        {
        // 搜索文件
        case 1:
            printf("请输入文件名\n");
            char file_name[80];
            scanf("%s", file_name);
            getchar();
            printf("==============================================================================================================\n");
            Node *res = find(now_node, file_name);
            break;

        // 创建新文件
        case 2:
            create_file();
            break;

        // 选择文件
        case 3:
            printf("选择哪个文件？(1或2)\n");
            int file_num = 0;
            scanf("%d", &file_num);
            getchar();
            choose_file(file_num);
            show_select_file_menu();
            break;
        // 返回
        case 4:
            return_last_dir();

            break;
        default:
            printf("请输入正确数字\n");
            break;
        }
    } while (section != 4);
}

// 打开进程管理器
void enter_process_manager()
{
    printf("==============================================================================================================\n");
    printf("欢迎使用进程管理器喵~\n");
    printf("==============================================================================================================\n");
    printf("正在加载进程列表，请稍候喵~\n");
    Queue queue;
    initQueue(&queue);

    // 从文件中加载进程队列
    read_queue_from_file(&queue);

    // 显示进程管理菜单
    show_process_management_menu(&queue);

    // 将进程队列装进文件
    write_queue_to_file(&queue);
    printf("已退出进程管理器，欢迎下次使用喵~\n");
}

// 显示进程管理菜单
void show_process_management_menu(Queue *queue)
{
    int section = 4;
    do
    {
        // 展示正在运行的进程
        printQueue(queue);

        printf("==============================================================================================================\n");
        printf("请选择你的的操作:\n\t1、创建新进程\n\t2、终止进程\n\t3、刷新进程列表\n\t4、返回\n");
        scanf("%d", &section);
        getchar();
        switch (section)
        {
        // 创建新进程
        case 1:
            printf("请输入进程名、进程耗时（秒）\n");
            char new_process_name[80];
            int time;
            scanf("%s%d", new_process_name, &time);
            getchar();
            enqueue(queue, create_process(rand(), new_process_name, ready, time));
            printf("新进程创建成功!\n");
            break;

        // 终止进程
        case 2:
            printf("请输入进程名喵~\n");
            char shut_down_name[80];
            scanf("%s", shut_down_name);
            Process *t = NULL;
            t = find_process(queue, shut_down_name);
            if (!t)
            {
                printf("未找到进程!\n");
            }
            else
            {
                shut_down_process(queue, t);
                printf("进程终止成功!\n");
            }
            break;

        // 刷新进程列表
        case 3:
            run(queue);
            break;
        // 返回
        case 4:
            break;
        default:
            printf("请输入正确数字\n");
            break;
        }
    } while (section != 4);
}