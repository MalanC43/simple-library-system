#pragma once
#include <iostream>
#include <string>

class func;

class book{
    public:
        bool borrowed;
        void output_menu(func &now);
        void change();
        void remove();
        void change_menu();

        std::string book_name;
        std::string writer;
        std::string ISBN;
        std::string content;
        std::string data;
        std::string entry;
        std::string master;
        int borrowed_num;
};