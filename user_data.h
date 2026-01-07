#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include <functional>
#include "book.h"

class func; 

class user_info{
    public:
    void output_menu(func &now);
    void change_menu();
    void remove();
    void change();
    void change_password();

    std::string username;
    size_t password; //better using MD5 结果只是普通哈希
    std::string level;
    std::string entry;
    int book_num;
    std::vector<std::string> borrowed_ISBN;
    std::vector<book> borrowed_book;
    std::hash<std::string> hash;
};