#include "user_fanc.h"
#include <sstream>

namespace fs=std::filesystem;

static std::string trim_u(const std::string& s){
    size_t a = s.find_first_not_of(' ');
    size_t b = s.find_last_not_of(' ');
    if(a==std::string::npos) return "";
    return s.substr(a, b-a+1);
}

static std::string serialize_user_line(const user_info& u){
    return u.username + " | " + std::to_string(u.password) + " | " + u.level + " | " + std::to_string(u.book_num);
}

static bool deserialize_user_line(const std::string& line, user_info& out){
    if(line.find('|') != std::string::npos){
        std::vector<size_t> pos;
        for(size_t i=0;i<line.size() && pos.size()<3;i++){
            if(line[i]=='|') pos.push_back(i);
        }
        if(pos.size()<3) return false;
        std::string f1 = trim_u(line.substr(0, pos[0]));
        std::string f2 = trim_u(line.substr(pos[0]+1, pos[1]-pos[0]-1));
        std::string f3 = trim_u(line.substr(pos[1]+1, pos[2]-pos[1]-1));
        std::string f4 = trim_u(line.substr(pos[2]+1));
        out.username = f1;
        try { out.password = (size_t)std::stoull(f2); } catch(...) { out.password = 0; }
        out.level = f3;
        try { out.book_num = std::stoi(f4); } catch(...) { out.book_num = 0; }
        return true;
    } else {
        std::istringstream iss(line);
        if(!(iss >> out.username >> out.password >> out.level >> out.book_num)) return false;
        return true;
    }
}

void user_info::output_menu(func &now){
    std::vector<std::string> info = {
        "用户名: " + username,
        "身份: " + std::string((level=="admin")?"管理员":"用户"),
        "借阅书籍数: " + std::to_string(borrowed_book.size())
    };
    draw_box("用户信息", info);
    int op;
    while(1){
        menus::clear();
        draw_box("操作",{"1.查看借阅详情","2.更改密码","3.返回"});
        std::cin>>op;
        switch(op){
            case 1:{now.look_book(borrowed_book);break;}
            case 2:{change_password();break;}
            case 3:{menus::clear();return;break;}
            default :{
                menus::error_menu();
            }
        }
    }
}

void user_info::change_password(){
    std::string ps1,ps2;
    size_t p1,p2;
    while(1){
        menus::clear();
        draw_box("输入新密码", {"请输入新密码"});
        std::cin>>ps1;
        p1=hash(ps1);
        draw_box("确认密码", {"请再次输入新密码"});
        std::cin>>ps2;
        p2=hash(ps2);
        if(p1!=p2){
            draw_box("错误",{"两次输入不一样，输入\"e\"以外字符来继续"});
            std::cin>>ps1;
            if(ps1=="e"){
                return;
            }
        }
        else{
            password=p1;
            std::ofstream out(entry);
            if (!out.is_open()) {
            std::cerr << "无法打开文件: " << entry << std::endl;
            return;
            }
            out << serialize_user_line(*this) << std::endl;
            for(int i=0;i<book_num;i++){
                out<<borrowed_ISBN[i]<<std::endl;
            }
            out.close();
            draw_box("成功",{"更改成功"});
            return;
        }
    }
}

void user_info::change_menu(){
    menus::clear();
    int op;
    while(1){
        draw_box("更改选项", {"1.更改", "2.删除", "3.返回"});
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
    draw_box("成功", {"删除成功"});
}
void user_info::change(){
    while(1){
    menus::clear();
    draw_box("更改用户信息", {"1.更改用户名", "2.重置密码为123456", "3.更改身份", "4.退出"});
    int op;
    std::cin>>op;
    switch(op){
        case 1:{
            menus::clear();
            draw_box("原用户名", {username});
            draw_box("输入", {"请输入更改后用户名"});
            std::string tmp;
            std::cin>>tmp;
            username=tmp;
            draw_box("成功", {"更改成功"});break;
        }
        case 2:{
            menus::clear();
            password=10676640704186402002;
            draw_box("成功", {"更改成功"});break;
        }
        case 3:{
            menus::clear();
            draw_box("原身份", {level});
            draw_box("选择身份", {"1.管理员", "2.用户"});
            bool f=1;
            while(f){
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
            draw_box("成功", {"更改成功"});break;
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
        out << serialize_user_line(*this) << std::endl;
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
            std::string header;
            if(std::getline(file, header)){
                if(deserialize_user_line(header, tmp)){
                    mp[tmp.username]=++id;
                    std::string str;
                    for(int i=0;i<tmp.book_num;i++){
                        if(!std::getline(file, str)) break;
                        str = trim_u(str);
                        if(str.empty()) continue;
                        tmp.borrowed_ISBN.push_back(str);
                        int idx = now.byISBN[str];
                        if(idx > 0 && idx <= (int)now.book_list.size()) tmp.borrowed_book.push_back(now.book_list[idx-1]);
                    }
                    users.push_back(tmp);
                }
            }
            file.close();
        }
    }
}

std::pair<bool,user_info*> fanc::cheak(std::string username,std::string password){
    user_info tmp;
    int id=mp[username];
    if(!id)return std::make_pair(false, nullptr);
    size_t a=hash(password);
    //std::cout<<id-1<<std::endl;
    tmp=users[id-1];
    //puts("Tsd");
    if(a!=users[id-1].password)return std::make_pair(false, nullptr);
    else return std::make_pair(true, &users[id-1]);
}

void fanc::look_user_menu(){
    int n=(int)users.size(),now=0;
    char op;
    while(1){
        menus::clear();
        draw_box("用户列表", {"代号 | 用户名 | 身份 | 借阅书籍数"});
        std::cout << std::string(50, '-') << std::endl;
            if(now+9>=n){
                for(int i=now;i<n;i++){
                    std::cout << std::left << std::setw(5) << (i-now) << " | "
                              << std::left << std::setw(15) << users[i].username << " | "
                              << std::left << std::setw(10) << ((users[i].level=="admin")?"管理员":"用户") << " | "
                              << users[i].borrowed_book.size() << std::endl;
                }
                draw_box("操作", {"输入代号查看详情", "按e退出", (now ? "按b上一页" : "")});
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
                else if(op=='b'&&now>0){
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
                for(int i=now;i<now+9;i++){
                    std::cout << std::left << std::setw(5) << (i-now) << " | "
                              << std::left << std::setw(15) << users[i].username << " | "
                              << std::left << std::setw(10) << ((users[i].level=="admin")?"管理员":"用户") << " | "
                              << users[i].borrowed_book.size() << std::endl;
                }
                draw_box("操作", {"输入代号查看详情", "按e退出", ((now)?"按b上一页":""), ((now+9<n)?"按n下一页":"")});
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
                else if(op=='b'&&now>0){
                    now-=10;
                }
                else if(op=='e'){
                    return;
                }
                else if(op=='n'&&now+9<n){
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
        draw_box("选择身份", {"1.管理员", "2.用户"});
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
        draw_box("输入用户名", {"请输入用户名"});
        std::cin>>tmp.username;
        if(mp[tmp.username]){
            draw_box("错误", {"已存在此用户，请重新输入"});
        }
        else break;
    }
    draw_box("输入密码", {"请输入密码"});
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
        out << serialize_user_line(tmp) << std::endl;
        out.close();
        users.push_back(tmp);
        mp[tmp.username] = users.size();
        draw_box("成功", {"添加成功"});
        menus::clear();
}

void fanc::search_user(int mod){
    std::string s;
    while(1){
        menus::clear();
        if(mod==1) draw_box("提示", {"请先查找需要更改或删除的用户"});
        draw_box("搜索用户", {"请输入用户名查找", "请输入\"exit\"退出"});
        std::cin>>s;
        if(s=="exit")return;
        if(!mp[s]){
            draw_box("错误", {"未找到该用户"});
            continue;
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
        draw_box("用户管理", {"1.搜索用户", "2.更改或删除用户", "3.增加用户", "4.浏览用户", "5.返回"});
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