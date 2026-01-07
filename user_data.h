#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include "book.h"

class func; 

class user_info{
    public:
    void output_menu(func &now);
    void change_menu();
    void remove();
    void change();

    std::string username;
    size_t password; //better using MD5
    std::string level;
    std::string entry;
    int book_num;
    std::vector<std::string> borrowed_ISBN;
    std::vector<book> borrowed_book;
};