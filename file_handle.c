#include "file_handle.h"
/*
    不是我喜欢使用全局变量，是tm的C语言又不支持面向对象编程我有什么办法！
*/

// 当前路径字符串
char now_path[200];
// 当前文件夹(节点)
Node *now_node = NULL;
// 选中的文件夹(节点)
Node *target_node = NULL;

// 序列化文件树
void serialize(Node *node, FILE *file)
{
    if (node == NULL)
    {
        fprintf(file, "NULL\n"); // 使用 "NULL" 标记空节点
        return;
    }

    // 将节点的Data写入文件
    fprintf(file, "%d %s %ld %s\n", node->data.isDir, node->data.name, node->data.size, node->data.create_time);

    // 递归写入左子树和右子树
    serialize(node->left, file);
    serialize(node->right, file);
}
// 释放以root为根节点的二叉树所有节点内存
void freeTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }

    // 递归释放左子树和右子树
    freeTree(root->left);
    freeTree(root->right);

    // 释放当前节点
    free(root);
}
// 将文件内容加载进文件树，并释放内存
void save_tree_to_file(Node *root)
{
    FILE *file = fopen("file_tree.txt", "w");
    if (file == NULL)
    {
        printf("无法打开文件进行读取\n");
        return;
    }
    serialize(root, file);
    fclose(file);
    free(root);
}
// 从文件中加载文件树并返回根节点
Node *load_tree_from_file()
{
    FILE *file = fopen("file_tree.txt", "r");
    if (file == NULL)
    {
        printf("无法打开文件进行读取或文件不存在！\n");
        return NULL;
    }
    Node *root = deserialize(file, NULL);
    fclose(file);
    return root;
}
// 反序列化
Node *deserialize(FILE *file, Node *parent)
{
    // 假设一行最多256个字符，先读一行
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        return NULL; // 文件读取失败或结束
    }
    // 如果前4个字节为NULL结束
    if (strncmp(buffer, "NULL", 4) == 0)
    {
        return NULL; // 读取到 "NULL" 标记，表示该节点为空
    }

    // 创建新节点并从文本中读取数据
    Node *node = (Node *)malloc(sizeof(Node));
    // 文件创建时间字符串中间有两个空格，需要特殊处理
    char str1[21];
    char str2[21];
    sscanf(buffer, "%d %s %ld %s %s", &node->data.isDir, node->data.name, &node->data.size, str1, str2);
    strcpy(node->data.create_time, str1);
    // 加两个空格
    strcat(node->data.create_time, " ");
    strcat(node->data.create_time, str2);

    // 设置父节点指针
    node->last = parent;

    // 递归还原左子树和右子树
    node->left = deserialize(file, node);
    node->right = deserialize(file, node);

    return node;
}

// 显示该文件信息
void show_file(Node *p)
{

    printf("%-20s%-20s%-20ld%-20s\n", p->data.isDir ? "File Folder" : "Text Document", p->data.name, p->data.size, p->data.create_time);
}
// 显示当前文件夹内所有文件
void show_file_list()
{
    pwd();
    printf("类型                名称                文件大小            创建时间            \n");
    if (!now_node->left && !now_node->right)
    {
        printf("该文件夹为空!\n");
        return;
    }
    Node *p = NULL;
    if (now_node->left)
    {
        p = now_node->left;
        show_file(p);
    }
    if (now_node->right)
    {
        p = now_node->right;
        show_file(p);
    }
    p = NULL;
}
// 在当前目录创建一个新的文件（文件夹）
void create_file()
{

    Node *p = now_node;

    if (p->left && p->right)
    {
        printf("该文件夹已满!\n");
        return;
    }

    // 获取文件元数据
    printf("请输入文件类型(1表示文件夹,0表示文本文件),文件名\n");
    int file_type;
    char name_str[80];
    char time_str[30];
    get_now_time(time_str);
    scanf("%d%s", &file_type, name_str);
    // 新节点初始化
    Node *new = (Node *)malloc(sizeof(Node));

    new->data.isDir = file_type;
    new->data.size = 0;
    strcpy(new->data.create_time, time_str);
    strcpy(new->data.name, name_str);
    new->last = now_node;
    new->left = NULL;
    new->right = NULL;
    // 将节点添加至文件树
    if (!p->left)
    {
        p->left = new;
    }
    else if (!p->right)
    {
        p->right = new;
    }
    // 创建对应的文件
    if (new->data.isDir)
    {
#ifdef _WIN32
        mkdir(new->data.name);
#else
        mkdir(new->data.name, 0777);
#endif
    }
    else
    {
        FILE *file = fopen(new->data.name, "w+");
        fclose(file);
    }
    now_node = p;

    printf("创建成功!\n");
}
// 选择文件（文件夹）
void choose_file(int file_num)
{

    if (file_num == 1)
    {
        if (!now_node->left)
        {
            printf("文件不存在!\n");
            return;
        }
        target_node = now_node->left;
    }
    else if (file_num == 2)
    {
        if (!now_node->right)
        {
            printf("文件不存在!\n");
            return;
        }
        target_node = now_node->right;
    }
}
// 递归地前序遍历二叉树，根据name寻找目标文件(文件夹)
Node *find(Node *root, char *name)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (strcmp(name, root->data.name) == 0)
    {
        printf("找到了亲!\n");
        show_file(root);
        printf("==============================================================================================================\n");
        return root; // 找到目标节点，返回该节点
    }

    Node *leftResult = find(root->left, name); // 在左子树中查找
    if (leftResult != NULL)
    {
        return leftResult; // 如果在左子树中找到，直接返回结果
    }

    Node *rightResult = find(root->right, name); // 在右子树中查找
    if (rightResult != NULL)
    {
        return rightResult; // 如果在右子树中找到，直接返回结果
    }
    printf("喵~未找到文件!");
    printf("==============================================================================================================\n");
    return NULL; // 如果左右子树都没有找到，返回NULL
}
// 向选中文件写入
void write_to_file(char *arr)
{
    if (target_node->data.isDir)
    {
        printf("不可以向文件夹写入喵!\n");
        return;
    }
    FILE *file = fopen(target_node->data.name, "a+");
    fprintf(file, "%s\n", arr);
    target_node->data.size = get_file_size(file);
    fclose(file);
    printf("写入成功!\n");
}
// 输出目标文件全部内容
void print_file()
{
    FILE *file = fopen(target_node->data.name, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    printf("文件内容如下：\n");
    char buffer[255];
    while (fgets(buffer, 255, file) != NULL)
    {
        printf("%s", buffer);
    }
    printf("==============================================================================================================\n");

    fclose(file);
}
// 删除文件或空文件夹
void delete_file()
{
    // 节点不存在，直接返回
    if (!target_node)
    {
        printf("节点不存在\n");
        return;
    }
    // 节点为目录文件
    if (target_node->data.isDir)
    {
        // 删除对应目录
        if (rmdir(target_node->data.name) == 0)
        {
            remove_node(target_node);
            printf("目录 %s 删除成功!\n", target_node->data.name);
            return;
        }
        else
        {
            perror("目录非空!\n");
            return;
        }
    }
    else // 非目录
    {
        // 删除对应文件
        remove(target_node->data.name);
        remove_node(target_node);
        printf("文件删除成功!\n");
    }
}
// 将节点从文件树中移除并释放内存
void remove_node(Node *target)
{
    // 找到父节点
    Node *p = target->last;
    if (p->left == target)
    {
        p->left = NULL;
    }
    else
    {
        p->right = NULL;
    }
    free(target);
}
// 增改当前路径信息
void path_add()
{
    if (now_node == NULL)
    {
        return;
    }

    strcat(now_path, "/");
    strcat(now_path, target_node->data.name);
}
// 删减路径信息
void path_sub()
{
    char *pos = strrchr(now_path, '/'); // 找到最后一个 '/' 的位置

    if (pos != NULL)
    {
        *pos = '\0'; // 截断字符串，删除最后一个 '/'
    }
}
// 显示当前文件路径
void pwd()
{
    printf("当前文件路径：%s\n", now_path);
}
// 进入文件夹
void cd()
{
    if (!target_node->data.isDir)
    {
        printf("亲，这是个文件哦~\n");
        return;
    }
    // 重新拼接当前路径
    path_add();

    now_node = target_node;
    // 切换当前实际工作目录
    chdir(now_node->data.name);
    printf("切换成功!\n");
}
// 返回上级目录
void return_last_dir()
{
    if (now_node->last == NULL)
    {
        return;
    }
    path_sub();
    now_node = now_node->last;
    // 将工作目录切换到上一个目录
    chdir("..");
    printf("已返回\n");
}