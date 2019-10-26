# C++ 分割字符串函数`split`

在 C++ STL 中，没有线程的分割字符串的函数，但是该函数的又是很常用的。所以，本文介绍三种字符串分割方法。

## 使用 C 中的`strtok`函数

C 中的 `strtok`函数可以对 C 风格的字符串进行分割，`strtok_r`是`strtok`的线程安全版本。

参考[https://linux.die.net/man/3/strtok_r](https://linux.die.net/man/3/strtok_r) 和 [关于函数strtok和strtok_r的使用要点和实现原理](https://www.cnblogs.com/zhouhbing/p/4103916.html)。

```c++
#include <string.h>

// str: 要分割的字符串
// delim: 作为分割点的分隔符
// saveptr: 线程安全版本中，用于保存分割点位置的指针

char* strtok(char* str, const char* delim);

char* strtok_r(char* str, const char* delim, char** saveptr);
```

当`str`不为`NULL`，则从头开始搜索第一个合法的分隔符，然后使用`'\0`'替换分隔符，并使用静态变量（`strtok`版本）或者传入的变量（`strtok_t`版本）保存分隔符的位置，最后返回`str`。因为 C 风格的字符串是以`'\0`'结尾的，所以调用函数后就能得到分割的第一个字符串。

往后，如果需要继续对`str`进行分割，不需要再传入`str`，只需要传入`NULL`即可，其他参数和第一次调用一样。

`split`函数的实现：

```c++
// str: 要分割的字符串
// result: 保存分割结果的字符串数组
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
```

***注意***：`strtok`和`strtok_r`会修改传进去的字符串`str`，所以调用它们之前，需要做好备份。

## C++ 中的流

使用`istringstream`配合`getline`，可以分割字符串。不过有一个限制：`getline`接受的分隔符只能是`char`。

`split`函数的实现：

```c++
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
```

## C++ 流迭代器

如果分隔符是流所定义的空白符，则有更简洁的实现。[流迭代器`istream_iterator`](https://zh.cppreference.com/w/cpp/iterator/istream_iterator)每次迭代都分割出一个字符串（以空白符为分隔符）。

`split`函数的实现：

```c++
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
```

## `find_first_not_of`、`find_first_of`

使用`find_first_not_of`查找到分割字符串的起点`start`，使用`find_first_of`查找到分隔符的位置`position`，则`[start, position)`就是分割到一个字符串。

重复上述过程，知道`position == std::string::npos && start == std::string::npos`，则分割结束。

`split`函数的实现：

```c++
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
```

## 更多

还可以使用 C++ 11 [正则表达式](https://zh.cppreference.com/w/cpp/regex)库或者 C++20 提供了`ranges`，有专门的`split view`，只要写`str | split(delim)`就可以分割字符串。详细见[C++ 的 string 为什么不提供 split 函数？ - 林暗草惊风的回答 - 知乎](https://www.zhihu.com/question/36642771/answer/865135551)。

## 源码

[Github](https://github.com/chenBright/code_-snippets/blob/280f0818dccfc3382f8de84beb6ce92ba961dcb9/C++/split)

## 参考

- [https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string/237280#237280](https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string/237280#237280)
- [https://stackoverflow.com/questions/26328793/how-to-split-string-with-delimiter-using-c](https://stackoverflow.com/questions/26328793/how-to-split-string-with-delimiter-using-c)
- [C++ 的 string 为什么不提供 split 函数？ - 林暗草惊风的回答 - 知乎](https://www.zhihu.com/question/36642771/answer/865135551)
- [Split a string](http://www.cplusplus.com/faq/sequences/strings/split/#basic-algorithm)
