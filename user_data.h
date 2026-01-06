#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include "bookfunc.h"

class user_info{
    public:
    std::string username;
    std::string password; //better using MD5
    std::string level;
    std::string entry;
    std::vector<std::string> borrowed_ISBN;
    std::vector<book> borrowed_book;
};