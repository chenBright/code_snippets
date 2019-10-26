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
           const char delim = ' ') {
    tokens.clear();
    
    std::istringstream iss(str);
    std::string tmp;
    while (std::getline(iss, tmp, delim)) {
        if (tmp != "") {
            // 如果两个分隔符相邻，则 tmp == ""，忽略。
            tokens.emplace_back(std::move(tmp));
        }
    }
}

int main() {
    std::string s("abc:def::ghi");
    std::vector<std::string> sv;

    split(s, sv, ':');

    for (std::vector<std::string>::const_iterator iter = sv.begin();
                                                   iter != sv.end();
                                                             ++iter) {
        std::cout << *iter << std::endl;
    }

    return 0;
}