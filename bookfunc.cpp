#include "bookfunc.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
#define testing 
namespace fs=std::filesystem;

void book::output_menu(){
    std::cout << "书名: " << book_name << std::endl;
    std::cout << "作者: " << writer << std::endl;
    std::cout << "ISBN: " << ISBN << std::endl;
    char op;
    while(1){
        puts("是/否查看内容,请输入y/n");
        std::cin>>op;
        if(op=='y'){
            std::cout<<content<<std::endl;
            std::cin.clear();
		    std::cin.sync();
		    std::this_thread::sleep_for(std::chrono::milliseconds(233));
        }
        else if(op=='n'){
            return;
        }
        else{
            std::cerr << "错误输入！\n";
		    std::cin.clear();
		    std::cin.sync();
		    std::this_thread::sleep_for(std::chrono::milliseconds(233));
        }
    }
}

func::func(){
    fs::path p="./books";
        if (!exists(p)) {
            create_directory(p);
        }
        int id=0;
    for (const auto& entry : fs::directory_iterator(p)) {
        std::ifstream file(entry.path());
        if (file.is_open()) {
            book tmp;
            file >> tmp.book_name >> tmp.writer >> tmp.ISBN ;
            std::getline(file,tmp.content,'\0');
            book_list.push_back(tmp);
            byname[tmp.book_name]=bywriter[tmp.writer]=byISBN[tmp.ISBN]=++id;
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
    int op=0;
    while(1){
        puts("请用数字来操作");
        puts("1.用书名查询");
        puts("2.用作者查询");
        puts("3.用ISBN/ISSN查询");
        puts("4.返回");
        std::cin>>op;
        switch(op){
            case 1:{search_name_menu();break;}
            case 2:{search_writer_menu();break;}
            case 3:{search_ISBN_menu();break;}
            case 4:{return;}
            default :{
                std::cerr << "错误输入！\n";
			    std::cin.clear();
			    std::cin.sync();
			    std::this_thread::sleep_for(std::chrono::milliseconds(233));//为了能在两边都能跑也是没谁了（
            }
        }
    }
}
void func::search_name_menu(){
    #ifdef testing 
    system("clear");
    #else 
    system("cls");
    #endif
    std::string name; 
    int tmpid;
    while(1){
        puts("请输入书名进行查询，按‘e’返回");
        std::cin>>name;
        if(name=="e"){
            return;
        }
        tmpid=byname[name];
        if(!tmpid){
            std::cerr << "错误输入！\n";
			std::cin.clear();
			std::cin.sync();
			std::this_thread::sleep_for(std::chrono::milliseconds(233));
            continue;
        }
        puts("查询结果为:");
        book_list[tmpid-1].output_menu();
    }
}
void func::search_writer_menu(){
    #ifdef testing 
    system("clear");
    #else 
    system("cls");
    #endif
    puts("请输入书名进行查询，按‘e’退出");
    std::string name; 
    int tmpid;
    while(1){
        std::cin>>name;
        if(name=="e"){
            return;
        }
        tmpid=byname[name];
        if(!tmpid){
            std::cerr << "错误输入！\n";
			std::cin.clear();
			std::cin.sync();
			std::this_thread::sleep_for(std::chrono::milliseconds(233));
            continue;
        }
        puts("查询结果为:");
        book_list[tmpid-1].output_menu();
    }

}
void func::search_ISBN_menu(){
    #ifdef testing 
    system("clear");
    #else 
    system("cls");
    #endif
    puts("请输入书名进行查询，按‘e’退出");
    std::string name; 
    int tmpid;
    while(1){
        std::cin>>name;
        if(name=="e"){
            return;
        }
        tmpid=byname[name];
        if(!tmpid){
            std::cerr << "错误输入！\n";
			std::cin.clear();
			std::cin.sync();
			std::this_thread::sleep_for(std::chrono::milliseconds(233));
            continue;
        }
        puts("查询结果为:");
        book_list[tmpid-1].output_menu();
    }

}