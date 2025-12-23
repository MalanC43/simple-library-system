#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

class book{
    public:
        std::string book_name;
        std::string writer;
        std::string ISBN;
        std::string content;
        void output_menu();
};

class func{
    public:
        func();
        void search_book_menu();
        void search_name_menu();
        void search_writer_menu();
        void search_ISBN_menu();

        std::vector<book> book_list;
        std::map<std::string,int> byname;
        std::map<std::string,int> bywriter;
        std::map<std::string,int> byISBN; 
};
