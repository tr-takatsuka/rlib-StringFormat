# printf-like formatting, using boost::format

[ENGLISH](/README.md)

## Description

A type-safe string format using boost::format. Implementation in C++17.

+ A string format that can be used like printf using boost::format.
  + You can use it in a function format like printf instead of connecting with "%".
+ Taking advantage of the characteristics of boost :: format, it is implemented so that it is type-safe and exception-free.
  + If an invalid type (non-compliant type) is given as an argument, a compile error will occur.
+ You can specify the stl container etc. as an argument. Expand and output.
  + std::pair std::tuple
    std::unique_ptr std::shared_ptr std::optional
    std::array std::vector std::list std::deque std::initializer_list
    std::set std::multiset std::map std::multimap


## Requirement

Works in the C++17 environment. boost library is required.

We have confirmed the operation in the following environment.
+ linux g++
+ windows VisualStudio 2017,2019

## Usage

It can be used by including StringFormat.h.

## example

```c++
    using std::cout;
    using std::endl;
    using namespace rlib::string;

    // The code below uses only printf-like format, but the content is boost::format.
    // Please refer to that reference for the details of the format.

    {// Normal usage
        cout << format(u8"%s %dSX SR%dDE%s", u8"日産", 180, 20, "T") << endl;   // 日産 180SX SR20DET
    }
    {// std::wstring
        std::wstring r = format(L"BNR%d RB%dDE%s", 32, 26, L"TT");  // L"BNR32 RB26DETT"
        std::wcout << r << endl;    // L"BNR32 RB26DETT"
    }
    {// No exception for unsuitable types
        cout << format("%dSX %s", 240, L"日産") << endl;    // 240SX 00007FF742CC123C(例)
    }
    {// You can write std::string as it is. Convenient because c_str() is not required.
        const std::string a = u8"トヨタ";
        std::string b = "1JZ-GTE";
        cout << format("%s jzx90 %s", a, b) << endl;    // トヨタ jzx90 1JZ-GTE
    }
#ifdef _MSC_VER
    {// CString for VisualStudio
        CString t0(_T("マツダ"));
        const CString t1(_T("13B-T"));
        auto r = format(_T("%s FC3S %s"), t0, t1);  // マツダ FC3S 13B-T
    }
#endif
    {// expands std::tuple std::pair
        std::tuple<std::string, int, double> a{ "RPS13",1998,11.8 };
        const std::pair<std::string, int> b{ "FR", 1848000 };
        cout << format(         // 形式:RPS13 排気量:1998cc 燃費:11.8km/l 駆動方式:FR 価格:1848000円
            u8"形式:%s 排気量:%dcc 燃費:%.1fkm/l 駆動方式:%s 価格:%d円", a, b) << endl;
    }
    {// outputs the contents std::unique_ptr std::shared_ptr std::optional. If it is empty(invalid), "(null)" is output.
        std::unique_ptr<int> u = std::make_unique<int>(1);
        std::shared_ptr<int> s = std::make_shared<int>(2);
        std::optional<int> o(3);
        std::optional<int> e;
        cout << format("%d,%d,%d,%d", u, s, o, e) << endl;  // 1,2,3,(null)
    }
    {// Expand other containers as well. (Maybe it's overkill. You can just give it a compile error.)
        cout << format("%d,%d,%d", std::array<int, 3>{ 1, 2, 3 }) << endl;          // 1,2,3
        cout << format("%d,%d,%d", std::vector<int>{4, 5, 6}) << endl;              // 4,5,6
        cout << format("%d,%d,%d", std::list<int>{7, 8, 9}) << endl;                // 7,8,9
        cout << format("%d,%d,%d", std::deque<int>{10, 11, 12}) << endl;            // 10,11,12
        cout << format("%d,%d,%d", std::set<int>{3, 2, 1}) << endl;                 // 1,2,3 (It is the order that can be taken)
        cout << format("%d,%d,%d", std::map<int, int>{ {1, 2}, { 3,4 }}) << endl;   // 1,2,3
        cout << format("%d,%d,%d", std::multiset<int>{1, 2, 3}) << endl;            // 1,2,3"
        cout << format("%d,%d,%d", std::multimap<int, int>{ {1, 2}, { 3,4 }}) << endl;// 1,2,3
    }
    {// Since the container is also expanded recursively, the following types are also possible.
        std::map<int, std::pair<std::string, std::vector<int>>> m{
            {1, {"a", {5,6,7}}},
            {8, {"b", {9,10}}}
        };
        cout << format("%s,%s,%s,%s,%s,%s", m) << endl; // 1,a,5,6,7,8
    }
    {// Since it is boost::format, unsupported types will result in a compile error.
        std::function<void()> f;
//      cout << format("%s", f) << endl;    // Compile error
    }
```

## Feature and Limitations

- It also contains example using BOOST.TEST. StringFormat_test.cpp
- Code indentation is tabs, not spaces. On github, add "?ts=4" to the end of the URL and refer to it on tab 4. I'm sorry.

## Licence

[LICENSE](/LICENSE)

