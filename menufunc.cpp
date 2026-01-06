#include "menufunc.h"
#include <limits>
#define testing

namespace menus{
    void error_menu(){
        std::cerr << "错误输入！\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::this_thread::sleep_for(std::chrono::milliseconds(233));//为了能在两边都能跑也是没谁了（
    }
    void clear(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::this_thread::sleep_for(std::chrono::milliseconds(233));    
    #ifdef testing 
    system("clear");
    #else 
    system("cls");
    #endif
    }
};

