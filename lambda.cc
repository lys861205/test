#include <stdio.h>
#include <string>

/*
 * lambda 表达式格式
 * [capture list] (params list) -> return type { function body }
 * lambda 表达式值传递有2种方式
 * 1 通过捕获的方式
 * 2 通过形参传递
 *
*/
/*
   声明lambda表达式
   [capture list](param list) mutable exception -> return type { function body }
   capture list 捕获外部变量列表
   param list 形参列表
   mutable指示符：是否可以修改捕获的变量
   expection： 异常设定
   return type： 返回类型
   function body： 函数体

   可以省略部分 如下：
   [capture list] (params list) -> return type {function body}
   [capture list] (params list) {function body}
   [capture list] {function body}

   捕获形式	说明
   []	不捕获任何外部变量
   [变量名, …]	默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符）
   [this]	以值的形式捕获this指针
   [=]	以值的形式捕获所有外部变量
   [&]	以引用形式捕获所有外部变量
   [=, &x]	变量x以引用形式捕获，其余变量以传值形式捕获
   [&, x]	变量x以值的形式捕获，其余变量以引用形式捕获
*/

int main()
{
  // 捕获
  {
    // 捕获（通过值传递）
    int value = 100;
    auto f = [value]{printf("%d\n", value);};
    value = 200;
    f();
  }

  {
    // 捕获（通过引用传递）
    int value = 100;
    auto f = [&value]{printf("%d\n", value);};
    value = 200;
    f();
  }

  {
    // 捕获（隐式捕获）
    int value = 100;
    auto f = [&]{printf("%d\n", value);}; //隐式 引用捕获
    value = 200;
    f();

  }
  {
    // 捕获（隐式捕获）
    int value = 100;
    auto f = [=]{printf("%d\n", value);}; //隐式 值捕获
    value = 200;
    f();
  }


  //参数列表形式
  {
    int value = 200;
    auto f = [](int a) {printf("%d\n", a);};
    f(value);
  }

  return 0;
}
