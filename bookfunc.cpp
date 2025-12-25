#pragma once
#include "bookfunc.h"
#include <filesystem>
#include <fstream>
#include "menufunc.h"

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
            menus::error_menu();
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
            file >> tmp.book_name >> tmp.writer >> tmp.ISBN >> tmp.data >> tmp.borrowed;
            std::getline(file,tmp.content,'\0');
            book_list.push_back(tmp);
            byname[tmp.book_name]=bywriter[tmp.writer]=byISBN[tmp.ISBN]=++id;
            file.close();
        }
    }
}

void func::search_book_menu(){
    menus::clear();
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
                menus::error_menu();
            }
        }
    }
}
void func::search_name_menu(){
    menus::clear();
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
            menus::error_menu();
            continue;
        }
        puts("查询结果为:");
        book_list[tmpid-1].output_menu();
    }
}
void func::search_writer_menu(){
    menus::clear();
    std::string name; 
    int tmpid;
    while(1){
        puts("请输入作者进行查询，按‘e’返回");
        std::cin>>name;
        if(name=="e"){
            return;
        }
        tmpid=bywriter[name];
        if(!tmpid){
            menus::error_menu();
            continue;
        }
        puts("查询结果为:");
        book_list[tmpid-1].output_menu();
    }

}
void func::search_ISBN_menu(){
    menus::clear();
    std::string name; 
    int tmpid;
    while(1){
        puts("请输入ISBN/ISSN进行查询，按‘e’返回");
        std::cin>>name;
        if(name=="e"){
            return;
        }
        tmpid=byISBN[name];
        if(!tmpid){
            menus::error_menu();
            continue;
        }
        puts("查询结果为:");
        book_list[tmpid-1].output_menu();
    }

}

void func::manage_book(){
    menus::clear();
    int op=0;
    while(1){
        puts("请用数字来操作");
        puts("1.搜索书籍");
        
        // puts("2.用作者查询");
        // puts("3.用ISBN/ISSN查询");
        // puts("4.返回");
        std::cin>>op;
        switch(op){
             case 1:{search_name_menu();break;}
            // case 2:{search_writer_menu();break;}
            // case 3:{search_ISBN_menu();break;}
            // case 4:{return;}
            default :{
                menus::error_menu();
            }
        }
    }
}
void func::look_book(){
    int n=book_list.size(),now=0;
    char op;
    menus::clear();
    while(1){
        std::cout<<std::left<<std::setw(15)<<"书名"<<
        std::left<<std::setw(15)<<"作者"<<
        std::left<<std::setw(15)<<"ISBN"<<
        std::left<<std::setw(15)<<"借阅情况";
        while(1){
            for(int i=0;i<=9;i++){
                std::cout<<std::left<<std::setw(15)<<book_list[now+i].book_name<<
                std::left<<std::setw(15)<<book_list[now+i].writer<<
                std::left<<std::setw(15)<<book_list[now+i].ISBN<<
                std::left<<std::setw(15)<<((book_list[now+i].borrowed)?"已借阅":"未借阅");    
            }
            puts("输入0-9来查看详情");
            std::cin>>op;
            if(isdigit(op)){
                menus::clear();
                book_list[now+(op-'0')].output_menu();
            }
            else{
                menus::error_menu();
            }
        }
        
    }
}