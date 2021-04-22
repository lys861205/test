#include <stdio.h>
#include <vector>
#include <functional>
#include <memory>


typedef std::function<void(int)> Functor;

void print(int a)
{
  printf("%d\n",a );
}

class Test 
{
public:
  void print()
  {
    printf("Test::print\n");
  }
};

int main()
{
  // std::vector<Functor> Functors;
  // for ( int i=0; i < 4; i++ )
  // {
  //   Functors.emplace_back(std::bind(&print, std::placeholders::_1));
  // }
  //
  // for ( int i=0; i < 4; i++ )
  // {
  //   Functors.emplace_back(std::bind(&print, i));
  // }
  //
  //
  Test t;
  // Functors.emplace_back(std::bind(&Test::print, &t));

  Functor f = std::bind(&Test::print, &t);
  f(2);

  // for ( size_t i=0; i < Functors.size(); i++ )
  // {
  //   Functor& f = Functors[i];
  //   f(2);
  // }
  return 0;
}
