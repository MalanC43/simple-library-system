#include "bookfunc.h"
#include <limits>
#include <sstream>
#include <algorithm>

namespace fs=std::filesystem;

inline int read(std::string ch){
	int x=0,f=0,n=ch.size();
	while(f<n){x=x*10+(ch[f++]^48);}
	return x;
}

static std::string trim(const std::string& s){
    size_t a = s.find_first_not_of(' ');
    size_t b = s.find_last_not_of(' ');
    if(a==std::string::npos) return "";
    return s.substr(a, b-a+1);
}

static std::string serialize_book_line(const book& b){
    return b.book_name + " | " + b.writer + " | " + b.ISBN + " | " + b.data + " | " + (b.borrowed?"1":"0") + " | " + b.master + " | " + std::to_string(b.borrowed_num) + " | " + b.content;
}

static bool deserialize_book_line(const std::string& line, book& out){
    if(line.find('|') != std::string::npos){
        std::vector<size_t> pos;
        for(size_t i=0;i<line.size() && pos.size()<7;i++){
            if(line[i]=='|') pos.push_back(i);
        }
        if(pos.size()<7) return false;
        std::string f1 = trim(line.substr(0, pos[0]));
        std::string f2 = trim(line.substr(pos[0]+1, pos[1]-pos[0]-1));
        std::string f3 = trim(line.substr(pos[1]+1, pos[2]-pos[1]-1));
        std::string f4 = trim(line.substr(pos[2]+1, pos[3]-pos[2]-1));
        std::string f5 = trim(line.substr(pos[3]+1, pos[4]-pos[3]-1));
        std::string f6 = trim(line.substr(pos[4]+1, pos[5]-pos[4]-1));
        std::string f7 = trim(line.substr(pos[5]+1, pos[6]-pos[5]-1));
        std::string f8 = trim(line.substr(pos[6]+1));
        out.book_name = f1;
        out.writer = f2;
        out.ISBN = f3;
        out.data = f4;
        out.borrowed = (f5=="1"||f5=="true") ? 1 : 0;
        out.master = f6;
        out.borrowed_num=read(f7);
        out.content = f8;
        return true;
    }else{
        std::istringstream iss(line);
        if(!(iss >> out.book_name >> out.writer >> out.ISBN >> out.data >> out.borrowed >> out.master)) return false;
        std::string rest;
        std::getline(iss, rest);
        out.content = trim(rest);
        return true;
    }
}

void book::remove(){
    std::remove(entry.c_str());
} 

void book::change_menu(){
    int op;
    while(1){
        menus::clear();
        draw_box("操作",{"1.更改","2.删除","3.退出"});
        std::cin>>op;
        switch(op){
            case 1:{change();break;}
            case 2:{remove();break;}
            case 3:{return;break;}
            default :{
                menus::error_menu();
            }
        }
    }
}

void book::output_menu(func &now){
    std::vector<std::string> info = {
        "书名: " + book_name,
        "作者: " + writer,
        "ISBN: " + ISBN,
        "日期: " + data,
        "借阅情况: " + std::string(borrowed ? "已借阅" : "未借阅"),
        "借阅人: " + master
    };
    draw_box("书籍详情", info);
    char op;
    while(1){
        menus::clear();
        std::vector<std::string> options = {"1.查看内容"};
        if(!borrowed) options.push_back("2.借阅");
        if(master==now.mine->username) options.push_back("2.归还");
        if(now.mine->level=="admin") options.push_back("3.更改");
        options.push_back("e.退出");
        draw_box("操作", options);
        std::cin >> op;
        if(op=='1'){
            menus::clear();
            std::vector<std::string> content_lines;
            std::string line;
            std::istringstream iss(content);
            while(std::getline(iss, line)){
                content_lines.push_back(line);
            }
            if(content_lines.empty() || (content_lines.size() == 1 && content_lines[0].empty())){
                content_lines.clear();
                content_lines.push_back(content.empty() ? "(空)" : content);
            }
            draw_box("内容", content_lines);
            std::cin.clear();
		    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		    std::this_thread::sleep_for(std::chrono::milliseconds(233));
        }
        else if(op=='e'){
            return;
        }
        else if(op=='2'&&master==now.mine->username){
            borrowed=0;master="无";
            std::ofstream out(entry);
            if (!out.is_open()) {
            std::cerr << "无法打开文件: " << entry << std::endl;
            return;
            }
            out << serialize_book_line(*this);
            out.close();
            for(auto i=now.mine->borrowed_ISBN.begin();i!=now.mine->borrowed_ISBN.end();i++){
                if(*i==ISBN){
                    now.mine->borrowed_ISBN.erase(i);break;
                }
            }
            for(auto i=now.mine->borrowed_book.begin();i!=now.mine->borrowed_book.end();i++){
                if(i->ISBN==ISBN){
                    now.mine->borrowed_book.erase(i);break;
                }
            }
            now.mine->book_num--;
            std::ofstream tout(now.mine->entry);
            if (!tout.is_open()) {
            std::cerr << "无法打开文件: " << now.mine->entry << std::endl;
            return;
            }
            tout << now.mine->username << " | " << now.mine->password << " | " << now.mine->level << " | " << now.mine->book_num << std::endl;
            for(std::string s:now.mine->borrowed_ISBN){
                tout<< s << std::endl;
            }
            tout.close();
            menus::clear();
            draw_box("成功", {"归还成功"});
            std::cin.clear();
		    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		    std::this_thread::sleep_for(std::chrono::milliseconds(233));
        }
        else if(op=='2'&&!borrowed){
            borrowed=1;master=now.mine->username;
            std::ofstream out(entry);
            if (!out.is_open()) {
            std::cerr << "无法打开文件: " << entry << std::endl;
            return;
            }
            out << serialize_book_line(*this);
            out.close();
            borrowed_num++;
            now.mine->borrowed_ISBN.push_back(ISBN);
            now.mine->book_num++;
            now.mine->borrowed_book.push_back(*this);
            std::ofstream tout(now.mine->entry);
            if (!tout.is_open()) {
            std::cerr << "无法打开文件: " << now.mine->entry << std::endl;
            return;
            }
            tout << now.mine->username << " | " << now.mine->password << " | " << now.mine->level << " | " << now.mine->book_num << std::endl;
            for(std::string s:now.mine->borrowed_ISBN){
                tout<< s << std::endl;
            }
            tout.close();
            menus::clear();
            draw_box("成功", {"借阅成功"});
            std::cin.clear();
		    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		    std::this_thread::sleep_for(std::chrono::milliseconds(233));
        }
        else if(op=='3'&&now.mine->level=="admin"){
            change();
        }
        else{
            menus::error_menu();
        }
    }
}

void book::change(){
    while(1){
    menus::clear();
    draw_box("更改书籍信息", {"1.更改书名", "2.更改作者", "3.更改ISBN/ISSN", "4.更改借阅情况", "5.更改内容", "6.退出"});
    int op;
    std::cin>>op;
    switch(op){
        case 1:{
            menus::clear();
            draw_box("原书名", {book_name});
            draw_box("输入", {"请输入更改后书名"});
            std::string tmp;
            std::cin>>tmp;
            book_name=tmp;
            draw_box("成功", {"更改成功"});break;
        }
        case 2:{
            menus::clear();
            draw_box("原作者", {writer});
            draw_box("输入", {"请输入更改后作者"});
            std::string tmp;
            std::cin>>tmp;
            writer=tmp;
            draw_box("成功", {"更改成功"});break;
        }
        case 3:{
            menus::clear();
            draw_box("原ISBN/ISSN", {ISBN});
            draw_box("输入", {"请输入更改后ISBN/ISSN"});
            std::string tmp;
            std::cin>>tmp;
            ISBN=tmp;
            draw_box("成功", {"更改成功"});break;
        }
        case 4:{
            menus::clear();
            draw_box("原借阅情况", {borrowed ? "已借阅" : "未借阅"});
            std::string tmp;
            if(!borrowed){
                draw_box("错误", {"无法从未借阅修改成已借阅"});
            }
            else{
                draw_box("确认", {"输入任意键继续"});
                std::cin>>tmp;
            }
            borrowed^=1;
            break;
        }
        case 5:{
            menus::clear();
            draw_box("原内容", {content});
            draw_box("输入", {"请输入更改后内容,输入单个字符串\"end\"结束"});
            std::string line,res;
            std::cin.ignore();
            while(std::getline(std::cin,line) && line!="end"){
                res+=line+'\n';
            }
            content=res;
            draw_box("成功", {"更改成功"});break;
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
        out << serialize_book_line(*this);
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
            std::string line;
            if(std::getline(file, line, '\0')){
                book tmp; tmp.entry = entry.path();
                if(deserialize_book_line(line, tmp)){
                    book_list.push_back(tmp);
                    byISBN[tmp.ISBN]=++id;
                    byname[tmp.book_name].push_back(id);
                    bywriter[tmp.writer].push_back(id);
                }
            }
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
        std::vector<std::string> options = {"1.用书名查询", "2.用作者查询", "3.用ISBN/ISSN查询", "4.返回"};
        if(mod==1) options.insert(options.begin(), "请先找到需要更改的书");
        if(mod==2) options.insert(options.begin(), "请先找到需要删除的书");
        draw_box("搜索书籍", options);
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
    while(1){
        menus::clear();
        draw_box("搜索书名", {"请输入书名进行查询，按'e'返回"});
        std::cin>>name;
        if(name=="e"){
            return;
        }
        if(byname.find(name)==byname.end() || byname[name].empty()){
            menus::error_menu();
            continue;
        }
        std::vector<book> tmp;
        for(int idx : byname[name]){
            if(idx > 0 && idx <= (int)book_list.size()){
                tmp.push_back(book_list[idx-1]);
            }
        }
        if(tmp.empty()){
            menus::error_menu();
            continue;
        }
        if(mod==1){
            look_book(tmp);
        }
        else if(mod==2){
            look_book(tmp);
            reload();
        }
        else{
            look_book(tmp);
        }
    }
}
void func::search_writer_menu(int mod){
    std::string name; 
    while(1){
        menus::clear();
        draw_box("搜索作者", {"请输入作者进行查询，按'e'返回"});
        std::cin>>name;
        if(name=="e"){
            return;
        }
        if(bywriter.find(name)==bywriter.end() || bywriter[name].empty()){
            menus::error_menu();
            continue;
        }
        std::vector<book> tmp;
        for(int idx : bywriter[name]){
            if(idx > 0 && idx <= (int)book_list.size()){
                tmp.push_back(book_list[idx-1]);
            }
        }
        if(tmp.empty()){
            menus::error_menu();
            continue;
        }
        if(mod==1){
            look_book(tmp);
        }
        else if(mod==2){
            look_book(tmp);
            reload();
        }
        else{
            look_book(tmp);
        }   
    }

}
void func::search_ISBN_menu(int mod){
    
    std::string name; 
    int tmpid;
    while(1){
        menus::clear();
        draw_box("搜索ISBN/ISSN", {"请输入ISBN/ISSN进行查询，按'e'返回"});
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
            draw_box("查询结果", {"结果如下:"});
            book_list[tmpid-1].output_menu(*this);
        }
    }

}

void func::manage_book(){
    int op=0;
    while(1){
        menus::clear();
        draw_box("书籍管理", {"1.搜索书籍", "2.更改已有书籍", "3.增加书籍", "4.删除书籍", "5.浏览书籍", "6.返回"});
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
        draw_box("书籍列表", {"代号 | 书名 | 作者 | ISBN | 借阅情况"});
            if(now+9>=n){
                for(int i=now;i<n;i++){
                    std::cout << format_table_cell(std::to_string(i-now), 3) << " | "
                              << format_table_cell(books[i].book_name, 10) << " | "
                              << format_table_cell(books[i].writer, 8) << " | "
                              << format_table_cell(books[i].ISBN, 10) << " | "
                              << format_table_cell(books[i].borrowed ? "已借阅" : "未借阅", 6) << std::endl;
                }
                draw_box("操作", {"输入代号查看详情", "按e退出", (now ? "按b上一页" : "")});
                std::cin>>op;
                if(isdigit(op)){
                    menus::clear();
                    books[now+(op-'0')].output_menu(*this);
                }
                else if(op=='b'&&now){
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
                    std::cout << format_table_cell(std::to_string(i-now), 3) << " | "
                              << format_table_cell(books[i].book_name, 10) << " | "
                              << format_table_cell(books[i].writer, 8) << " | "
                              << format_table_cell(books[i].ISBN, 10) << " | "
                              << format_table_cell(books[i].borrowed ? "已借阅" : "未借阅", 6) << std::endl;
                }
                draw_box("操作", {"输入代号查看详情", "按e退出", ((now)?"按b上一页":""), ((now+9<n)?"按n下一页":"")});
                std::cin>>op;
                if(isdigit(op)){
                    menus::clear();
                    books[now+(op-'0')].output_menu(*this);
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

void func::add_book_menu(){
    menus::clear();
    draw_box("输入书名", {"请输入书名"});
    book tmp;
    while(1){
        std::cin>>tmp.book_name;
        break;
    }
    draw_box("输入作者", {"请输入作者"});
    while(1){
        std::cin>>tmp.writer;
        break;
    }
    draw_box("输入ISBN/ISSN", {"请输入ISBN/ISSN"});
    while(1){
        std::cin>>tmp.ISBN;
        if(byISBN[tmp.ISBN]){
        draw_box("错误", {"已存在此ISBN/ISSN，请重新输入"});
        }
        else break;
    }
    draw_box("输入日期", {"请输入日期,如20070101"});
    std::cin>>tmp.data;
    tmp.borrowed=0;
    tmp.master="无";
    tmp.borrowed_num=0;
    draw_box("输入内容", {"请输入内容,输入单个字符串\"end\"结束"});
    std::string line,res;
    std::cin.ignore();
    while(std::getline(std::cin,line) && line!="end"){
        res+=line+'\n';
    }
    tmp.content=res;
    tmp.entry="./books/"+std::to_string(hash(tmp.book_name+tmp.writer));
    std::ofstream out(tmp.entry);
        if (!out.is_open()) {
        std::cerr << "无法打开文件: " << tmp.entry << std::endl;
        return;
        }
        out << serialize_book_line(tmp);
        out.close();
        int id=book_list.size();
        book_list.push_back(tmp);
        byname[tmp.book_name].push_back(id);
        bywriter[tmp.writer].push_back(id);
        byISBN[tmp.ISBN]=id;
        draw_box("成功", {"添加成功"});
        menus::clear();
}


void func::rank(){
    std::vector<book> tmp;
    tmp=book_list;
    //std::cout<<book_list.size()<<std::endl;
    std::sort(tmp.begin(),tmp.end(), 
    [this](const book& a, const book& b) {
        return a.borrowed > b.borrowed;  
    });
    if(tmp.size()>10)tmp.erase(tmp.begin()+10,tmp.end());
    draw_box("成功",{"稍等，将显示借阅排名前十的书籍"});
    std::this_thread::sleep_for(std::chrono::milliseconds(888));
    look_book(tmp);
    reload();
}