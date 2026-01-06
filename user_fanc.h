#pragma once
#include <iostream>
#include <string.h>
#include "bookfunc.h"
#include <functional>
#include <utility>
#include <vector>

class user_info{
    public:
    std::string username;
    size_t password; //better using MD5
    std::string level;
    std::string entry;
    int book_num;
    std::vector<std::string> borrowed_ISBN;
    std::vector<book> borrowed_book;
};

class fanc{
    public:
       fanc(func &now);
       void reload(func &now);
       std::pair<bool,user_info> cheak(std::string username,std::string password);
       void look_user_menu();

    private:
        std::vector<user_info> users;
        std::map<std::string,int> mp;
        std::hash<std::string> hash;
};