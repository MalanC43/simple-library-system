#pragma once
#include <iostream>
#include <string.h>
#include <bookfunc.h>


class userfanc{
    public:
        void sub_menu();
        void borrow_book_menu();
        void return_book_menu();
        void load_my_data(std::string user_name);
        void change_my_info();
        //...
};