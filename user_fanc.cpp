#include "user_fanc.h"

namespace fs=std::filesystem;

func::func(){
    reload();
}

void reload(){
    fs::path p="./users";
        if (!exists(p)) {
            create_directory(p);
        }
        int id=0;
    for (const auto& entry : fs::directory_iterator(p)) {
        std::ifstream file(entry.path());
        if (file.is_open()) {
            user_info tmp;tmp.entry=entry.path();
            file >> tmp.username >> tmp.password;
           
            file.close();
        }
    }
}
