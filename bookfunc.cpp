#include "bookfunc.h"
#include <limits>

namespace fs=std::filesystem;

void book::output_menu(){
    std::cout << "书名: " << book_name << std::endl;
    std::cout << "作者: " << writer << std::endl;
    std::cout << "ISBN: " << ISBN << std::endl;
    std::cout << "日期: " << data << std::endl;
    std::cout << "借阅情况: " << ((borrowed)?"已借阅":"未借阅") << std::endl;
    std::cout << "借阅人: " << master << std::endl;
    char op;
    while(1){
        puts("是/否查看内容,请输入y/n");
        std::cin>>op;
        if(op=='y'){
            std::cout<<content<<std::endl;
            std::cin.clear();
		    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

void book::change(){//borrow没改完
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
            std::string tmp;
            if(!borrowed){
                std::cout<<"原借阅人："<<master<<std::endl;
                puts("请输入更改后的借阅人");
                std::cin>>tmp;
                master=tmp;
                /*等待施工*/
            }
            else{
                puts("输入任意键继续");
                std::cin>>tmp;
            }
            
            break;
        }
        case 5:{
            menus::clear();
            std::cout<<"原内容："<<content<<std::endl;
            puts("请输入更改后内容,输入单个字符串“end”结束");
            std::string tmp,res;
            std::getline(std::cin,tmp,'\n');
            while(tmp!="end"){res+=tmp+'\n';std::getline(std::cin,tmp,'\n');}
            content=res;
            puts("更改成功");break;
        }
        case 6:{
            return;
        }
        default:{
            menus::error_menu();
            break;
        }
    }
        std::ofstream out(entry);
        if (!out.is_open()) {
        std::cerr << "无法打开文件: " << entry << std::endl;
        return;
        }
        out << book_name <<' '<< writer <<' '<<  ISBN <<' '<< data <<' '<< borrowed <<' '<< master <<' '<<content;
        out.close();
    }
}

void func::reload(){
    byname.clear();bywriter.clear();byISBN.clear();book_list.clear();
    fs::path p="./books";
        if (!exists(p)) {
            create_directory(p);
        }
        int id=0;
    for (const auto& entry : fs::directory_iterator(p)) {
        std::ifstream file(entry.path());
        if (file.is_open()) {
            book tmp;tmp.entry=entry.path();
            file >> tmp.book_name >> tmp.writer >> tmp.ISBN >> tmp.data >> tmp.borrowed >> tmp.master;
            std::getline(file,tmp.content,'\0');
            book_list.push_back(tmp);
            byname[tmp.book_name]=bywriter[tmp.writer]=byISBN[tmp.ISBN]=++id;
            file.close();
        }
    }
}

func::func(){
    reload();
}

void func::search_book_menu(int mod){
    /*
    0: admin
    1: change
    2: remove
    3: users/borrow
    */
    int op=0;
    while(1){
        menus::clear();
        if(mod==1)puts("请先找到需要更改的书");
        if(mod==2)puts("请先找到需要删除的书");
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
    std::string name; 
    int tmpid;
    while(1){
        menus::clear();
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
        else if(mod==2){
            std::remove(book_list[tmpid-1].entry.c_str());
            reload();
        }
        else{
            puts("查询结果为:");
            book_list[tmpid-1].output_menu();
        }
    }
}
void func::search_writer_menu(int mod){
    std::string name; 
    int tmpid;
    while(1){
        menus::clear();
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
        else if(mod==2){
            std::remove(book_list[tmpid-1].entry.c_str());
            reload();
        }
        else{
            puts("查询结果为:");
            book_list[tmpid-1].output_menu();
        }   
    }

}
void func::search_ISBN_menu(int mod){
    
    std::string name; 
    int tmpid;
    while(1){
        menus::clear();
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
        else if(mod==2){
            std::remove(book_list[tmpid-1].entry.c_str());
            reload();
        }
        else{
            puts("查询结果为:");
            book_list[tmpid-1].output_menu();
        }
    }

}

void func::manage_book(){
    int op=0;
    while(1){
        menus::clear();
        puts("请用数字来操作");
        puts("1.搜索书籍");
        puts("2.更改已有书籍");
        puts("3.增加书籍");
        puts("4.删除书籍");
        puts("5.浏览书籍");
        puts("6.返回");
        std::cin>>op;
        switch(op){
            case 1:{search_book_menu();break;}
            case 2:{search_book_menu(1);break;}
            case 3:{add_book_menu();break;}
            case 4:{search_book_menu(2);break;}
            case 5:{look_book(book_list);break;}
            case 6:{return;}
            default :{
                menus::error_menu();
            }
        }
    }
}
void func::look_book(std::vector<book> books){
    int n=(int)books.size(),now=0;
    char op;
    menus::clear();
    while(1){
        std::cout<<std::left<<std::setw(25)<<"代号"<<
        std::left<<std::setw(25)<<"书名"<<
        std::left<<std::setw(25)<<"作者"<<
        std::left<<std::setw(25)<<"ISBN"<<
        std::left<<std::setw(25)<<"借阅情况"<<std::endl;
            if(now+9>=n){
                for(int i=now;i<n;i++){
                    std::cout<<std::left<<std::setw(25)<<i-now<<
                    std::left<<std::setw(25)<<books[i].book_name<<
                    std::left<<std::setw(25)<<books[i].writer<<
                    std::left<<std::setw(25)<<books[i].ISBN<<
                    std::left<<std::setw(25)<<((books[i].borrowed)?"已借阅":"未借阅")<<std::endl;
                }
                puts("输入代号来查看详情,按e提出");
                bool f1=0;
                if(now){
                    printf("上一页按b ");f1=1;
                }
                std::cin>>op;
                if(isdigit(op)){
                    menus::clear();
                    books[now+(op-'0')].output_menu();
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
                    std::cout<<std::left<<std::setw(25)<<i<<
                    std::left<<std::setw(25)<<books[now+i].book_name<<
                    std::left<<std::setw(25)<<books[now+i].writer<<
                    std::left<<std::setw(25)<<books[now+i].ISBN<<
                    std::left<<std::setw(25)<<((books[now+i].borrowed)?"已借阅":"未借阅")<<std::endl;    
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
                    books[now+(op-'0')].output_menu();
                }
                else if(op=='b'&&f1){
                    now-=10;
                }
                else if(op=='e'){
                    menus::clear();return;
                }
                else if(op=='f'&&f2){
                    now+=10;
                }
                else{
                menus::error_menu();
                }
            }
        menus::clear();
    }
}

void func::add_book_menu(){
    menus::clear();
    puts("请输入书名");
    book tmp;
    while(1){
        std::cin>>tmp.book_name;
        if(byname[tmp.book_name]){
        puts("已存在此书名，请重新输入");
        }
        else break;
    }
    puts("请输入作者");
    while(1){
        std::cin>>tmp.writer;
        if(bywriter[tmp.writer]){
        puts("已存在此作者，请重新输入");
        }
        else break;
    }
    puts("请输入ISBN/ISSN");
    while(1){
        std::cin>>tmp.ISBN;
        if(byISBN[tmp.ISBN]){
        puts("已存在此ISBN/ISSN，请重新输入");
        }
        else break;
    }
    puts("请输入日期,如20070101");
    std::cin>>tmp.data;
    tmp.borrowed=0;
    tmp.master="无";
    puts("请输入内容");
    std::cin>>tmp.content;
    tmp.entry="./books/"+tmp.book_name;
    std::ofstream out(tmp.entry);
        if (!out.is_open()) {
        std::cerr << "无法打开文件: " << tmp.entry << std::endl;
        return;
        }
        out << tmp.book_name <<' '<< tmp.writer <<' '<<  tmp.ISBN <<' '<< tmp.data <<' '<< tmp.borrowed <<' '<< tmp.content;
        out.close();
        int id=book_list.size();
        book_list.push_back(tmp);
        byname[tmp.book_name]=bywriter[tmp.writer]=byISBN[tmp.ISBN]=id;
        puts("添加成功");
        menus::clear();
}

