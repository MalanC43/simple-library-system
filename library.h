#pragma once
#include <iostream>
#include <vector>
#include "user_data.h"
#include "admin.h"
#include "bookfunc.h"

class library{
    public:
        void menu();
        void exitS();
        void login();
        void Register();
        //...
    private:
       
        func book;
};