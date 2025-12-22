#define testing
#include "bookfunc.h"
#include <iostream>

int main(){
    func test;
    if (!test.book_list.empty()) {
        book temp = test.book_list[0];
        std::cout << "Book Name: " << temp.book_name << std::endl;
        std::cout << "Writer: " << temp.writer << std::endl;
        std::cout << "ISBN: " << temp.ISBN << std::endl;
        std::cout << "Content: " << temp.content << std::endl;
    } else {
        std::cout << "No books loaded" << std::endl;
    }
    return 0;
}