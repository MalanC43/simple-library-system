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
        std::string data;
        bool borrowed;
        void output_menu();
        void change();
};

class func{
    public:
        func();
        void search_book_menu(int mod=0);
        void search_name_menu(int mod=0);
        void search_writer_menu(int mod=0);
        void search_ISBN_menu(int mod=0);
        void manage_book();
        void look_book();
        void change_book_menu();

        std::vector<book> book_list;
        std::map<std::string,int> byname;
        std::map<std::string,int> bywriter;
        std::map<std::string,int> byISBN; 
};
