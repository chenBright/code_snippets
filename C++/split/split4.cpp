#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// str: 要分割的字符串
// tokens: 保存分割结果的字符串数组
// delim: 分隔字符串
void split(const std::string& str, 
           std::vector<std::string>& tokens, 
           const std::string delim = " ") {
    tokens.clear();
    
    auto start = str.find_first_not_of(delim, 0);       // 分割到的字符串的第一个字符
    auto position = str.find_first_of(delim, start);    // 分隔符的位置
    while (position != std::string::npos || start != std::string::npos) {
        // [start, position) 为分割下来的字符串
        tokens.emplace_back(std::move(str.substr(start, position - start)));
        start = str.find_first_not_of(delim, position);
        position = str.find_first_of(delim, start);
    }
}

int main() {
    std::string s("abc:def::ghi");
    std::vector<std::string> sv;

    split(s, sv, ":");

    for (std::vector<std::string>::const_iterator iter = sv.begin();
                                                   iter != sv.end();
                                                             ++iter) {
        std::cout << *iter << std::endl;
    }

    return 0;
}