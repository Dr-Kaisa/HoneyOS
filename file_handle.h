#ifndef __FILE_HANDLE_H__
#define __FILE_HANDLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"
#include "menu.h"

typedef struct Data
{
    int isDir;     // 是否为文件夹,0不是，1是
    char name[80]; // 文件（夹）名
    long int size; // 文件大小
    char create_time[30];

} Data;

typedef struct Node
{
    Data data;
    struct Node *last;
    struct Node *left;
    struct Node *right;
} Node;
// 声明全局变量
extern char now_path[200];
extern Node *now_node;
extern Node *target_node;

void serialize(Node *node, FILE *file);
void freeTree(Node *root);
void save_tree_to_file(Node *root);
Node *load_tree_from_file();
Node *deserialize(FILE *file, Node *parent);
void show_file(Node *p);
void show_file_list();
void create_file();
void delete_file();
void remove_node(Node *target);
void pwd();
void choose_file(int file_num);
Node *find(Node *root, char *name);
void write_to_file(char *arr);
void print_file();
void path_add();
void path_sub();
void cd();
void return_last_dir();
#endif
