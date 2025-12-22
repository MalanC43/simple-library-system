#pragma once
#include <iostream>
#include <string.h>
#include <vector>

class book{
    public:
        std::string book_name;
        std::string writer;
        std::string ISBN;
        std::string content;
};

class func{
    public:
        func();
        void search_book_menu();
        std::vector<book> book_list;

    
};
