#pragma once
#include <iostream>
#include <string.h>
#include <vector>

class user_info{
    public:
    std::string username;
    std::string password; //better using MD5
    std::vector<std::string> borrowed_books;
};