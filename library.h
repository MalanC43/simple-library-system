#pragma once
#include <iostream>
#include <vector>
#include "bookfunc.h"
#include "user_fanc.h"

class library{
    public:
        library() : user(books) {}  // 添加构造函数，初始化 user
        void menu();
        void login();
        void admin_menu();
        void user_menu();
        //...
        user_info *usernow;
        func books;
        fanc user;
};