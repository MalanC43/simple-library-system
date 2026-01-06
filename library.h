#pragma once
#include <iostream>
#include <vector>
#include "admin.h"
#include "bookfunc.h"
#include "user_fanc.h"

class library{
    public:
        void menu();
        void exitS();
        void login();
        void Register();
        //...
    private:
        user_info usernow;
        func book;
};