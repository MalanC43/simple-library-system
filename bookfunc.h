#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include "menufunc.h"

class book{
    public:
        std::string book_name;
        std::string writer;
        std::string ISBN;
        std::string content;
        std::string data;
        std::string entry;
        std::string master;
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
        void look_book(std::vector<book> books);
        void add_book_menu();
        void reload();

        std::vector<book> book_list;
        std::map<std::string,int> byname;
        std::map<std::string,int> bywriter;
        std::map<std::string,int> byISBN; 
};
