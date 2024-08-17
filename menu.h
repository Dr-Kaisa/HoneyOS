#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include <unistd.h>
#include "file_handle.h"
#include "process_handler.h"
// 声明函数原型
void show_main_menu();

void enter_file_manager();
void show_select_file_menu();
void show_enter_file_menu();

void enter_process_manager();
void show_process_management_menu(Queue *queue);

#endif