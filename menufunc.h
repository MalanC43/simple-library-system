#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <iomanip>

size_t get_display_width(const std::string& str);
std::string format_table_cell(const std::string& content, size_t width);
void draw_box(const std::string& title, const std::vector<std::string>& lines);

namespace menus{
    void error_menu();
    void clear();
};

