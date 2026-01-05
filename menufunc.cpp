#include "menufunc.h"
#define testing

namespace menus{
    void error_menu(){
        std::cerr << "错误输入！\n";
		std::cin.clear();
		std::cin.sync();
		std::this_thread::sleep_for(std::chrono::milliseconds(233));//为了能在两边都能跑也是没谁了（
    }
    void clear(){
    #ifdef testing 
    system("clear");
    #else 
    system("cls");
    #endif
}
};

