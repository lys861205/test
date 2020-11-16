#include <iostream>
constexpr int a = 2;

template <int N>
class list
{};

constexpr int sqr1(int p) {
  return p*p*a;
}



int sqr2(int p) {
  return p*p;
}

int main()
{
  const int X = 2; 
  list<sqr1(X)> list1; // OK
  //list<sqr2(X)> list2; // ERROR
  return 0;
}
