#include "bookfunc.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
namespace fs=std::filesystem;

func::func(){
    fs::path p="./books";
        if (!exists(p)) {
            create_directory(p);
        }
    for (const auto& entry : fs::directory_iterator(p)) {
        std::ifstream file(entry.path());
        if (file.is_open()) {
            book tmp;
            file >> tmp.book_name >> tmp.writer >> tmp.ISBN >> tmp.content;
            book_list.push_back(tmp);
            file.close();
        }
    }
}
void func::search_book_menu(){
    #ifdef testing 
    system("clear");
    #else 
    system("cls");
    #endif
    int op;
    while(1){
        puts("请用数字来操作");
        puts("1.用书名查询");
        puts("2.用作者查询");
        puts("3.用ISBN/ISSN查询");
        puts("4.返回");
        switch(op){
            case 1:{break;}
            case 2:{break;}
            case 3:{break;}
            case 4:{break;}
            default :{
                std::cerr << "错误输入！\n";
			    std::cin.clear();
			    std::cin.sync();
			    std::this_thread::sleep_for(std::chrono::milliseconds(233));//为了能在两边都能跑也是没谁了（
            }
        }
    }

}