#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

// str: 要分割的字符串
// tokens: 保存分割结果的字符串数组
void split(const std::string& str, 
           std::vector<std::string>& tokens) {
    tokens.clear();

    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss), 
              std::istream_iterator<std::string>(),
              std::back_inserter(tokens));
}

int main() {
    std::string s("abc:def::ghi");
    std::vector<std::string> sv;

    split(s, sv);

    for (std::vector<std::string>::const_iterator iter = sv.begin();
                                                   iter != sv.end();
                                                             ++iter) {
        std::cout << *iter << std::endl;
    }

    return 0;
}