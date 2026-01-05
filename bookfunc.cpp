#include "bookfunc.h"
#include <filesystem>
#include <fstream>
#include "menufunc.h"
namespace fs=std::filesystem;

void book::output_menu(){
    std::cout << "书名: " << book_name << std::endl;
    std::cout << "作者: " << writer << std::endl;
    std::cout << "ISBN: " << ISBN << std::endl;
    std::cout << "借阅情况: " << ((borrowed)?"已借阅":"未借阅") << std::endl;
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

void book::change(){
    while(1){
         menus::clear();
    puts("请输入数字来操作");
    puts("1，更改书名");
    puts("2，更改作者");
    puts("3，更改ISBN/ISSN");
    puts("4，更改借阅情况");
    puts("5，更改内容");
    puts("6，退出");
    int op;
    std::cin>>op;
    switch(op){
        case 1:{
            menus::clear();
            std::cout<<"原书名："<<book_name<<std::endl;
            puts("请输入更改后书名");
            std::string tmp;
            std::cin>>tmp;
            book_name=tmp;
            puts("更改成功");break;
        }
        case 2:{
            menus::clear();
            std::cout<<"原作者："<<writer<<std::endl;
            puts("请输入更改后作者");
            std::string tmp;
            std::cin>>tmp;
            writer=tmp;
            puts("更改成功");break;
        }
        case 3:{
            menus::clear();
            std::cout<<"原ISBN/ISSN："<<ISBN<<std::endl;
            puts("请输入更改后ISBN/ISSN");
            std::string tmp;
            std::cin>>tmp;
            ISBN=tmp;
            puts("更改成功");break;
        }
        case 4:{
            menus::clear();
            std::cout<<"原借阅情况："<<((borrowed)?"已借阅":"未借阅")<<std::endl;
            puts("按任意键更改");
            std::string tmp;
            std::cin>>tmp;
            borrowed^=1;
            puts("更改成功");break;
        }
        case 5:{
            menus::clear();
            std::cout<<"原内容："<<content<<std::endl;
            puts("请输入更改后内容");
            std::string tmp;
            std::getline(std::cin,tmp,'\0');
            content=tmp;
            puts("更改成功");break;
        }
        case 6:{
            return;
        }
        default:{
            menus::error_menu();break;
        }
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

void func::search_book_menu(int mod){
    menus::clear();
    int op=0;
    while(1){
        if(mod==1)puts("请先找到需要更改的书");
        puts("请用数字来操作");
        puts("1.用书名查询");
        puts("2.用作者查询");
        puts("3.用ISBN/ISSN查询");
        puts("4.返回");
        std::cin>>op;
        switch(op){
            case 1:{search_name_menu(mod);break;}
            case 2:{search_writer_menu(mod);break;}
            case 3:{search_ISBN_menu(mod);break;}
            case 4:{return;}
            default :{
                menus::error_menu();
            }
        }
    }
}
void func::search_name_menu(int mod){
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
        if(mod==1){
            book_list[tmpid-1].change();
        }
        else{
            puts("查询结果为:");
            book_list[tmpid-1].output_menu();
        }
    }
}
void func::search_writer_menu(int mod){
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
        if(mod==1){
            book_list[tmpid-1].change();
        }
        else{
            puts("查询结果为:");
            book_list[tmpid-1].output_menu();
        }   
    }

}
void func::search_ISBN_menu(int mod){
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
        if(mod==1){
            book_list[tmpid-1].change();
        }
        else{
            puts("查询结果为:");
            book_list[tmpid-1].output_menu();
        }
    }

}

void func::manage_book(){
    menus::clear();
    int op=0;
    while(1){
        puts("请用数字来操作");
        puts("1.搜索书籍");
        puts("2.更改已有书籍");
        // puts("2.用作者查询");
        // puts("3.用ISBN/ISSN查询");
        // puts("4.返回");
        std::cin>>op;
        switch(op){
            case 1:{search_book_menu();break;}
            case 2:{search_book_menu(1);break;}
            // case 3:{search_ISBN_menu();break;}
            // case 4:{return;}
            default :{
                menus::error_menu();
            }
        }
    }
}
void func::look_book(){
    int n=(int)book_list.size(),now=0;
    char op;
    menus::clear();
    while(1){
        std::cout<<std::left<<std::setw(15)<<"代号"<<
        std::left<<std::setw(15)<<"书名"<<
        std::left<<std::setw(15)<<"作者"<<
        std::left<<std::setw(15)<<"ISBN"<<
        std::left<<std::setw(15)<<"借阅情况"<<std::endl;
            if(now+9>=n){
                for(int i=now;i<n;i++){
                    std::cout<<std::left<<std::setw(15)<<i-now<<
                    std::left<<std::setw(15)<<book_list[i].book_name<<
                    std::left<<std::setw(15)<<book_list[i].writer<<
                    std::left<<std::setw(15)<<book_list[i].ISBN<<
                    std::left<<std::setw(15)<<((book_list[i].borrowed)?"已借阅":"未借阅")<<std::endl;
                }
                puts("输入代号来查看详情,按e提出");
                bool f1=0;
                if(now){
                    printf("上一页按b ");f1=1;
                }
                std::cin>>op;
                if(isdigit(op)){
                    menus::clear();
                    book_list[now+(op-'0')].output_menu();
                }
                else if(op=='b'&&f1){
                    now-=10;
                }
                else if(op=='e'){
                    return;
                }
                else{
                menus::error_menu();
                }
            }
            else{
                for(int i=0;i<=9;i++){
                    std::cout<<std::left<<std::setw(15)<<i<<
                    std::left<<std::setw(15)<<book_list[now+i].book_name<<
                    std::left<<std::setw(15)<<book_list[now+i].writer<<
                    std::left<<std::setw(15)<<book_list[now+i].ISBN<<
                    std::left<<std::setw(15)<<((book_list[now+i].borrowed)?"已借阅":"未借阅")<<std::endl;    
                }
                puts("输入代号来查看详情,按e提出");
                bool f1=0,f2=0;
                if(now){
                    printf("上一页按b ");f1=1;
                }
                if(now+9!=n-1){
                    printf("下一页按f ");f2=1;
                }
                puts("");
                std::cin>>op;
                if(isdigit(op)){
                    menus::clear();
                    book_list[now+(op-'0')].output_menu();
                }
                else if(op=='b'&&f1){
                    now-=10;
                }
                else if(op=='e'){
                    return;
                }
                else if(op=='f'&&f2){
                    now+=10;
                }
                else{
                menus::error_menu();
                }
            }
        
    }
}