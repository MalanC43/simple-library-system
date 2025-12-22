#pragma once
#include <iostream>
#include <string.h>
#include <vector>

class book{
    public:
        std::string ISBN;
        std::string book_name;
        std::string write;
        std::string content;
};

class func{
    public:
        func();
    private:
        std::vector<book> book_list;
        
    
};
