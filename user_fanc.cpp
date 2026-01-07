#include "user_fanc.h"

namespace fs=std::filesystem;

void user_info::output_menu(func &now){
    std::cout<<"用户名:"<<username<<std::endl
    <<"身份:"<< ((level=="admin")?"管理员":"用户")<<std::endl
    <<"借阅书籍数:"<<borrowed_book.size()<<std::endl;
    int op;
    while(1){
        puts("1.查看借阅详情");
        puts("2.返回");
        std::cin>>op;
        switch(op){
            case 1:{now.look_book(borrowed_book);break;}
            case 2:{menus::clear();return;break;}
            default :{
                menus::error_menu();
            }
        }
    }
}

void user_info::change_menu(){
    menus::clear();
    int op;
    while(1){
        puts("请输入数字来选择");
        puts("1.更改");
        puts("2.删除");
        puts("3.返回");
        std::cin>>op;
        switch(op){
            case 1:{change();break;}
            case 2:{remove();break;}
            case 3:{return ;}
            default :{
                menus::error_menu();break;
            }
        }
    }
}

void user_info::remove(){
    std::remove(entry.c_str());
    puts("删除成功");
}
void user_info::change(){
    while(1){
    menus::clear();
    puts("请输入数字来操作");
    puts("1，更改用户名");
    puts("2，重置密码为123456");
    puts("3，更改身份");
    puts("4，退出");
    int op;
    std::cin>>op;
    switch(op){
        case 1:{
            menus::clear();
            std::cout<<"原用户名："<<username<<std::endl;
            puts("请输入更改后用户名");
            std::string tmp;
            std::cin>>tmp;
            username=tmp;
            puts("更改成功");break;
        }
        case 2:{
            menus::clear();
            password=10676640704186402002;
            puts("更改成功");break;
        }
        case 3:{
            menus::clear();
            std::cout<<"原身份："<<level<<std::endl;
            puts("请输入更改后身份");
            bool f=1;
            while(f){
                puts("1.管理员");
                puts("2.用户");
                std::cin>>op;
                switch(op){
                    case 1:{f=0;level="admin";break;}
                    case 2:{f=0;level="user";break;}
                    default:{
                    menus::error_menu();
                    break;
                    }
                }
            }
            puts("更改成功");break;
        }
        case 4:{
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
        out << username <<' '<< password <<' '<<  level <<' '<< book_num <<std::endl;
        for(int i=0;i<book_num;i++){
            out<<borrowed_ISBN[i]<<std::endl;
        }
        out.close();
    }
}

fanc::fanc(func &now){
    sys= &now;
    reload(now);
}

void fanc::reload(func &now){
    users.clear();
    fs::path p="./users";
        if (!exists(p)) {
            create_directory(p);
        }
        int id=0;
    for (const auto& entry : fs::directory_iterator(p)) {
        std::ifstream file(entry.path());
        if (file.is_open()) {
            user_info tmp;tmp.entry=entry.path();
            if(file >> tmp.username >> tmp.password >> tmp.level >> tmp.book_num){
                mp[tmp.username]=++id;
                std::string str;
                for(int i=1;i<=tmp.book_num;i++){
                   file>>str;
                   tmp.borrowed_ISBN.push_back(str);
                   int idx = now.byISBN[str];
                   if(idx) tmp.borrowed_book.push_back(now.book_list[idx-1]);
                }
                users.push_back(tmp);
            }
            file.close();
        }
    }
}

std::pair<bool,user_info*> fanc::cheak(std::string username,std::string password){
    user_info tmp;
    int id=mp[username];
    if(!id)return std::make_pair(0,&tmp);
    size_t a=hash(password);
    //std::cout<<id-1<<std::endl;
    tmp=users[id-1];
    //puts("Tsd");
    if(a!=users[id-1].password)return std::make_pair(0,&tmp);
    else return std::make_pair(1,&users[id-1]);
}

void fanc::look_user_menu(){
    int n=(int)users.size(),now=0;
    char op;
    menus::clear();
    while(1){
        std::cout<<std::left<<std::setw(20)<<"代号"<<
        std::left<<std::setw(20)<<"用户名"<<
        std::left<<std::setw(20)<<"身份"<<
        std::left<<std::setw(20)<<"借阅书籍数"<<std::endl;
            if(now+9>=n){
                for(int i=now;i<n;i++){
                    std::cout<<std::left<<std::setw(20)<<i-now<<
                    std::left<<std::setw(20)<<users[i].username<<
                    std::left<<std::setw(20)<<((users[i].level=="admin")?"管理员":"用户")<<
                    std::left<<std::setw(20)<<users[i].borrowed_book.size()<<std::endl;
                }
                puts("输入代号来查看详情,按e提出");
                bool f1=0;
                if(now){
                    printf("上一页按b ");f1=1;
                }
                std::cin>>op;
                if(isdigit(op)){
                    int idx = now + (op - '0');
                    if(idx >= 0 && idx < n){
                        menus::clear();
                        users[idx].output_menu(*sys);
                    } else {
                        menus::error_menu();
                    }
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
                    std::cout<<std::left<<std::setw(20)<<i-now<<
                    std::left<<std::setw(20)<<users[i].username<<
                    std::left<<std::setw(20)<<((users[i].level=="admin")?"管理员":"用户")<<
                    std::left<<std::setw(20)<<users[i].borrowed_book.size()<<std::endl;    
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
                    int idx = now + (op - '0');
                    if(idx >= 0 && idx < n){
                        menus::clear();
                        users[idx].output_menu(*sys);
                    } else {
                        menus::error_menu();
                    }
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

void fanc::add_user(int mod){
    /*
    0.normal
    1.admin
    */
    user_info tmp;
    int op;bool f=1;
    while(f&&mod){
        puts("请输入数字来选择");
        puts("1.管理员");
        puts("2.用户");
        std::cin>>op;
        switch(op){
            case 1:{f=0;tmp.level="admin";break;}
            case 2:{f=0;tmp.level="user";break;}
            default :{
                menus::error_menu();break;
            }
        }
    }
    while(1){
        puts("请输入用户名");
        std::cin>>tmp.username;
        if(mp[tmp.username]){
            puts("已存在此用户，请重新输入");
        }
        else break;
    }
    puts("请输入密码");
    std::string password;
    std::cin>>password;
    tmp.password=hash(password);
    tmp.book_num=0;
    tmp.entry="./users/"+tmp.username;
    std::ofstream out(tmp.entry);
    if (!out.is_open()) {
        std::cerr << "无法打开文件: " << tmp.entry << std::endl;
        return;
        }
        out << tmp.username <<' '<< tmp.password <<' '<<  tmp.level <<' '<< 0 ;
        out.close();
        users.push_back(tmp);
        mp[tmp.username] = users.size();
        puts("添加成功");
        menus::clear();
}

void fanc::search_user(int mod){
    std::string s;
    while(1){
        menus::clear();
        if(mod==1)puts("请先查找需要更改或删除的用户");
        puts("请输入用户名查找");
        puts("请输入“exit”退出");
        std::cin>>s;
        if(s=="exit")return;
        if(!mp[s]){
            puts("未找到该用户");continue;
        }
        if(mod==1){
            users[mp[s]-1].change_menu();
            reload(*sys);
        }
        else users[mp[s]-1].output_menu(*sys);
    }
}

void fanc::manage_user(){
    int op=0;
    while(1){
        menus::clear();
        puts("请用数字来操作");
        puts("1.搜索用户");
        puts("2.更改或删除用户");
        puts("3.增加用户");
        puts("4.浏览用户");
        puts("5.返回");
        std::cin>>op;
        switch(op){
            case 1:{search_user();break;}
            case 2:{search_user(1);break;}
            case 3:{add_user(1);break;}
            case 4:{look_user_menu();break;}
            case 5:{return;}
            default :{
                menus::error_menu();
            }
        }
    }
}