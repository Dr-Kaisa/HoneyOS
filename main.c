#include "boot.h"
#include "menu.h"

int main()
{
    boot();
    // 该系统依赖于root目录作为系统目录
    create_root_dir();
    chdir("root");

    show_main_menu();
    return 0;
}