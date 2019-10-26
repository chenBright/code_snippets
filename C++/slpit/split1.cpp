#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>

// str: 要分割的字符串
// tokens: 保存分割结果的字符串数组
// delim: 分隔字符串
void split(const std::string& str, 
           std::vector<std::string>& tokens, 
           const std::string delim = " ") {
    tokens.clear();
    
    char* buffer = new char[str.size() + 1];
    std::strcpy(buffer, str.c_str());

    char* tmp;
    char* p = strtok_r(buffer, delim.c_str(), &tmp); // 第一次分割
    do {
        tokens.push_back(p); // 如果 p 为 nullptr，则将整个字符串作为结果
    } while ((p = strtok_r(nullptr, delim.c_str(), &tmp)) != nullptr);
    // strtok_r 为 strtok 的线程安全版本。
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
