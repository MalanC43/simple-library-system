#include "menufunc.h"
#include <limits>
#include <vector>
#include <string>
#define testing

// 计算字符串的终端显示宽度（处理中文和ANSI转义码）此函数由ai完成，实在不知道怎么给中文对齐了
size_t get_display_width(const std::string& str) {
    size_t width = 0;
    bool in_escape = false;
    for (size_t i = 0; i < str.length(); ++i) {
        unsigned char c = str[i];
        
        // 处理ANSI转义码
        if (c == '\033') {
            in_escape = true;
            continue;
        }
        if (in_escape) {
            if (c == 'm') {
                in_escape = false;
            }
            continue;
        }
        
        // 判断UTF-8字符的起始（非续字节）
        if ((c & 0xC0) != 0x80) {
            // 这是一个新字符的开始
            // 检查是否是中文（UTF-8 3字节，首字节为111xxxxx，即>=0xE0 且<0xF0）
            if ((c & 0xE0) == 0xE0 && (c & 0xF0) != 0xF0) {
                width += 2; // 中文字符占2个宽度
                i += 2; // 跳过后续的2个字节
            } else {
                width += 1; // 英文和其他字符占1个宽度
            }
        }
    }
    return width;
}

// 格式化表格单元格，以指定宽度左对齐
std::string format_table_cell(const std::string& content, size_t width) {
    size_t content_width = get_display_width(content);
    if (content_width >= width) {
        return content;
    }
    size_t padding = width - content_width;
    return content + std::string(padding, ' ');
}

void draw_box(const std::string& title, const std::vector<std::string>& lines) {
    size_t max_width = get_display_width(title);
    for (const auto& line : lines) {
        size_t w = get_display_width(line);
        if (w > max_width) max_width = w;
    }
    
    std::string color_code;
    if (title == "错误") {
        color_code = "\033[31m"; // 红色
    } else if (title == "成功") {
        color_code = "\033[32m"; // 绿色
    } else {
        color_code = "\033[34m"; // 蓝色
    }
    
    std::string border = "*" + std::string(max_width + 2, '*') + "*";
    
    size_t title_display_width = get_display_width(title);
    size_t title_padding = max_width - title_display_width;
    std::string title_line = "* " + color_code + title + "\033[0m" + std::string(title_padding, ' ') + " *";
    
    std::cout << border << std::endl;
    std::cout << title_line << std::endl;
    std::cout << border << std::endl;
    
    for (const auto& line : lines) {
        size_t line_display_width = get_display_width(line);
        size_t line_padding = max_width - line_display_width;
        std::string content = "* " + line + std::string(line_padding, ' ') + " *";
        std::cout << content << std::endl;
    }
    
    std::cout << border << std::endl;
}

namespace menus{
    void error_menu(){
        draw_box("错误", {"错误输入！请按回车继续。"});
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::this_thread::sleep_for(std::chrono::milliseconds(233));
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

