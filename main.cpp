#include "library.h"
#include <iostream>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <clocale>
#include <locale>
#endif

int main(){
#if defined(_WIN32) || defined(_WIN64)
    // Enable UTF-8 input/output on Windows console to avoid mojibake
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, ".UTF-8");
#endif
    library test;
    test.menu();
    return 0;
}