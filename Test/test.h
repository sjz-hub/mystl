//
// Created by 孙建钊 on 2023/6/18.
//

#ifndef MYSTL_TEST_H
#define MYSTL_TEST_H


// 一个简单的单元测试框架，定义了两个类 TestCase 和 UnitTest，以及一系列用于测试的宏

#include <iostream>
#include <iomanip>
#include <sstream>

namespace mystl{
    namespace test{
//随机种子
#define seed 0

// 不同情况的测试数量级
#if defined(_DEBUG) || defined(DEBUG)
#define LEN1    10000
#define LEN2    100000
#define LEN3    1000000
#else
#define LEN1    100000
#define LEN2    1000000
#define LEN3    10000000
//#define LEN1    1
//#define LEN2    10
//#define LEN3    100
#endif


#define SCALE_LLL(N) (N * 20)
#define SCALE_LL(N)  (N * 10)
#define SCALE_L(N)   (N * 5)
#define SCALE_M(N)   (N)
#define SCALE_S(N)   (N / 5)
#define SCALE_SS(N)  (N / 10)
#define SCALE_SSS(N) (N / 20)

#define WIDE    14


// 是否开启性能测试
#ifndef PERFORMANCE_TEST_ON
#define PERFORMANCE_TEST_ON 0
#endif // !PERFORMANCE_TEST_ON

// 是否开启大数据量测试
#ifndef LARGER_TEST_DATA_ON
#define LARGER_TEST_DATA_ON 0
#endif // !LARGER_TEST_DATA_ON


//输出通过提示
#define PASSED std::cout<<"[ PASSED ]\n"


//遍历输出到文件
#define WRITEFILE(container, file_name) do{ \
    std::string con_name = #container; \
    std::ofstream file(file_name);          \
    for(auto it = container.begin(); it != container.end(); ++it){ \
        file<<*it<<"\n";                            \
    } \
                                          \
    file.close();\
}while(0)


        //遍历输出容器
#define COUT(container) do{ \
    std::string con_name = #container; \
    std::cout << " " << con_name << " :";\
    for(auto it:container)  \
        std::cout<<" "<<it; \
    std::cout<<"\n";\
}while(0)


//输出容器调用函数后的结果
#define FUN_AFTER(con, fun) do{ \
    std::string fun_name = #fun;\
    std::cout<<" After "<<fun_name<<" :\n"; \
    fun;                        \
    COUT(con);\
}while(0)

//#define FUN_AFTER(con, fun) do {                         \
//  std::string fun_name = #fun;                           \
//  std::cout << " After " << fun_name << " :\n";          \
//  fun;                                                   \
//  COUT(con);                                             \
//} while(0)


//输出容器调用函数返回的值
#define FUN_VALUE(fun)do{ \
    std::string fun_name = #fun; \
    std::cout<<" "<<fun_name<<" : "<<fun<<"\n";\
}while(0)





// 输出测试数量级
        void test_len(size_t len1, size_t len2, size_t len3, size_t wide)
        {
            std::string str1, str2, str3;
            std::stringstream ss;
            ss << len1 << " " << len2 << " " << len3;
            ss >> str1 >> str2 >> str3;
            str1 += "   |";
            std::cout << std::setw(wide) << str1;
            str2 += "   |";
            std::cout << std::setw(wide) << str2;
            str3 += "   |";
            std::cout << std::setw(wide) << str3 << "\n";
        }

#define TEST_LEN(len1, len2, len3, wide) \
  test_len(len1, len2, len3, wide)

#define FUN_TEST_FORMAT1(mode, fun, arg, count,file_name) do {         \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(arg);                                              \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                                   \
   WRITEFILE(c,file_name);\
} while(0)

#define FUN_TEST_FORMAT2(mode, fun, arg1, arg2, count,file_name) do {  \
  srand(seed);                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(c.arg1(), arg2);                                   \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                                   \
  WRITEFILE(c,file_name);\
} while(0)


#define CON_TEST_P1(con, fun, arg, len1, len2, len3, file_name)         \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT1(std::con, fun, arg, len1, file_name+std::string("_std_")+std::string(std::to_string(len1))+".txt");                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len2, file_name+std::string("_std_")+std::string(std::to_string(len2))+".txt");                \
  FUN_TEST_FORMAT1(std::con, fun, arg, len3, file_name+std::string("_std_")+std::string(std::to_string(len3))+".txt");                \
  std::cout << "\n|        mystl        |";                  \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len1, file_name+std::string("_mystl_")+std::string(std::to_string(len1))+".txt");              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len2, file_name+std::string("_mystl_")+std::string(std::to_string(len2))+".txt");              \
  FUN_TEST_FORMAT1(mystl::con, fun, arg, len3, file_name+std::string("_mystl_")+std::string(std::to_string(len3))+".txt");


#define CON_TEST_P2(con, fun, arg1, arg2, len1, len2, len3, file_name)  \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                               \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len1, file_name+std::string("_std_")+std::string(std::to_string(len1))+".txt");         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len2, file_name+std::string("_std_")+std::string(std::to_string(len2))+".txt");         \
  FUN_TEST_FORMAT2(std::con, fun, arg1, arg2, len3, file_name+std::string("_std_")+std::string(std::to_string(len3))+".txt");         \
  std::cout << "\n|        mystl        |";                             \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len1, file_name+std::string("_mystl_")+std::string(std::to_string(len1))+".txt");       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len2, file_name+std::string("_mystl_")+std::string(std::to_string(len2))+".txt");       \
  FUN_TEST_FORMAT2(mystl::con, fun, arg1, arg2, len3, file_name+std::string("_mystl_")+std::string(std::to_string(len3))+".txt");





#define LIST_SORT_DO_TEST(mode, count, file_name) do {                  \
  srand(seed);                                       \
  clock_t start, end;                                        \
  mode::list<int> l;                                         \
  char buf[10];                                              \
  for (size_t i = 0; i < count; ++i)                         \
    l.insert(l.end(), rand());                               \
  start = clock();                                           \
  l.sort();                                                  \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                                    \
  WRITEFILE(l, file_name);\
} while(0)


#define LIST_SORT_TEST(len1, len2, len3,file_name)                     \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                              \
  LIST_SORT_DO_TEST(std, len1, file_name+std::string("_std_")+std::string(std::to_string(len1))+".txt");                              \
  LIST_SORT_DO_TEST(std, len2, file_name+std::string("_std_")+std::string(std::to_string(len2))+".txt");                              \
  LIST_SORT_DO_TEST(std, len3, file_name+std::string("_std_")+std::string(std::to_string(len3))+".txt");                              \
  std::cout << "\n|        mystl        |";                            \
  LIST_SORT_DO_TEST(mystl, len1, file_name+std::string("_mystl_")+std::string(std::to_string(len1))+".txt");                            \
  LIST_SORT_DO_TEST(mystl, len2, file_name+std::string("_mystl_")+std::string(std::to_string(len2))+".txt");                            \
  LIST_SORT_DO_TEST(mystl, len3, file_name+std::string("_mystl_")+std::string(std::to_string(len3))+".txt");





    }
}


#endif //MYSTL_TEST_H
