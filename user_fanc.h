#pragma once
#include <iostream>
#include <string.h>
#include "bookfunc.h"
#include "user_data.h"
#include <functional>
#include <utility>
#include <vector>

class fanc{
    public:
       fanc(func &now);
       void reload(func &now);
       std::pair<bool,user_info*> cheak(std::string username,std::string password);
       void look_user_menu();
       void search_user(int mod=0);
       void add_user(int mod=0);
       void manage_user();
        
    private:
        func * sys;
        std::vector<user_info> users;
        std::map<std::string,int> mp;
        std::hash<std::string> hash;
};