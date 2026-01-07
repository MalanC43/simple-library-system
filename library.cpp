#include "library.h"

void library::menu(){
    while(1){
        menus::clear();
        draw_box("欢迎使用此管理系统", {"请输入数字来操作", "1.登陆", "2.注册", "3.退出"});
        int op;
        std::cin>>op;
        switch(op){
            case 1:{login();break;}
            case 2:{user.add_user();break;}
            case 3:{return;break;}
            default :{
                menus::error_menu();
            }
        }
    }
}

void library::login(){
    std::string username,password;
    while(1){
        menus::clear();
        draw_box("登录", {"请输入用户名"});
        std::cin>>username;
        draw_box("登录", {"请输入密码"});
        std::cin>>password;
        auto tmp=user.cheak(username,password);
        if(tmp.first){
            draw_box("成功", {"登录成功"});
            books.mine = tmp.second;
            usernow = tmp.second;
            break;
        }
        else{
            draw_box("错误", {"用户名或密码错误", "输入1退出,其它继续"});
            std::cin>>username;
            if(username=="1")return;
        }
    }
    if(usernow->level=="admin")admin_menu();
    else user_menu();
}

void library::admin_menu(){
    while(1){
        menus::clear();
        draw_box("管理员菜单", {"1.管理书籍", "2.管理用户", "3.退出"});
        int op;
        std::cin>>op;
        switch(op){
            case 1:{books.manage_book();break;}
            case 2:{user.manage_user();break;}
            case 3:{return;break;}
            default :{
                menus::error_menu();
            }
        }
    }
}

void library::user_menu(){
    while(1){
        menus::clear();
        draw_box("用户菜单", {"1.搜索书籍", "2.浏览书籍", "3.查看用户信息", "4.退出"});
        int op;
        std::cin>>op;
        switch(op){
            case 1:{books.search_book_menu(3);user.reload(books);break;}
            case 2:{books.look_book(books.book_list);break;}
            case 3:{usernow->output_menu(books);break;}
            case 4:{return;break;}
            default :{
                menus::error_menu();
            }
        }
    }
}