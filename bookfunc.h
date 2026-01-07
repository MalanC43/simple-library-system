#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include "menufunc.h"
#include "book.h"
#include "user_data.h"

class func{
    public:
        func();
        func& operator=(const func& /*other*/) {
        return *this;
        }
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
        user_info *mine;  
};
