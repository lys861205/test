#include "Any.h"
#include <iostream>
#include <memory>

using namespace muduo;

template<class T>
void test(T a)
{
  if(std::is_reference<T>::value)
  {
    std::cout << "T is reference " << std::endl;
  }
  typedef typename std::remove_reference<T>::type nonref;
  nonref b = a;
  b = 10;
  /*
  std::add_lvalue_reference<int&>::type b = a;
  b = 20;
  */
}

class A {
public:
  A(int a, int b):a(a),b(b){}
  void print()
  {
    std::cout << "a " << a << " b " << b << std::endl;
  }
private:
  int a;
  int b;
};

typedef std::shared_ptr<A> APtr;


void test_object_ref()
{
  APtr p(new A(111, 222));
  p->print();
  std::cout << "use_count " << p.use_count() << std::endl;
  Any any(p);

  APtr l = any_cast<APtr>(any);
  std::cout << "use_count " << l.use_count() << std::endl;
  l->print();

  APtr& r = any_cast<APtr&>(any);
  std::cout << "use_count " << r.use_count() << std::endl;
  r->print();
}

int main()
{
  int a = 2;
  Any any(a);
  int* p = any_cast<int>(&any);
  std::cout << *p << std::endl;

  int v = any_cast<int>(any);
  std::cout << v << std::endl;

  int& ref = any_cast<int&>(any);
  std::cout << ref << std::endl;
  ref = 100;

  v = any_cast<int>(any);
  std::cout << v << std::endl;

  std::cout << "----------------" << std::endl;


  test<int&>(a);
  std::cout << a << std::endl;

  std::cout << "=====================" << std::endl;
  test_object_ref();

  return 0;
}
