#include "user_fanc.h"

namespace fs=std::filesystem;

fanc::fanc(func &now){
    reload(now);
}

void fanc::reload(func &now){
    fs::path p="./users";
        if (!exists(p)) {
            create_directory(p);
        }
        int id=0;
    for (const auto& entry : fs::directory_iterator(p)) {
        std::ifstream file(entry.path());
        if (file.is_open()) {
            user_info tmp;tmp.entry=entry.path();
            file >> tmp.username >> tmp.password >> tmp.level >>tmp.book_num;
            mp[tmp.username]=++id;
            std::string str;
            for(int i=1;i<=tmp.book_num;i++){
               file>>str;
               tmp.borrowed_ISBN.push_back(str);
               tmp.borrowed_book.push_back(now.book_list[now.byISBN[str]]);
            }
            file.close();
        }
    }
}

std::pair<bool,user_info> fanc::cheak(std::string username,std::string password){
    user_info tmp;
    int id=mp[username];
    if(!id)return std::make_pair(0,tmp);
    size_t a=hash(password);
    if(a!=users[id-1].password)return std::make_pair(0,tmp);
    else return std::make_pair(1,users[id-1]);
}

void fanc::look_user_menu(){
    int n=(int)users.size(),now=0;
    char op;
    menus::clear();
    while(1){
        std::cout<<std::left<<std::setw(20)<<"用户名"<<
        std::left<<std::setw(20)<<"身份"<<
        std::left<<std::setw(20)<<"借阅书籍数"<<std::endl;
            if(now+9>=n){
                for(int i=now;i<n;i++){
                    std::cout<<std::left<<std::setw(20)<<i-now<<
                    std::left<<std::setw(20)<<users[i].username<<
                    std::left<<std::setw(20)<<((users[i].level=="admin")?"管理员":"用户")<<
                    std::left<<std::setw(20)<<users.size()<<std::endl;
                }
                puts("输入代号来查看详情,按e提出");
                bool f1=0;
                if(now){
                    printf("上一页按b ");f1=1;
                }
                std::cin>>op;
                if(isdigit(op)){
                    menus::clear();
                    users[now+(op-'0')].output_menu();
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
                    std::left<<std::setw(15)<<books[now+i].book_name<<
                    std::left<<std::setw(15)<<books[now+i].writer<<
                    std::left<<std::setw(15)<<books[now+i].ISBN<<
                    std::left<<std::setw(15)<<((books[now+i].borrowed)?"已借阅":"未借阅")<<std::endl;    
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
                    users[now+(op-'0')].output_menu();
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
