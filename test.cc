#include <iostream>
#include <sstream>
#include <tuple>
#include <exception>
#include <string>
#include <vector>

using namespace std;

class Girl {

  public:
    string name;
    int age;

    Girl() {
      cout << "Girl()" << endl;
    }
    ~Girl() {
      cout << "~Girl()" << endl;
    }

    Girl(string _name, int _age) : name(_name), age(_age) {
      cout << "Girl(string _name, int _age)" << endl;
    }

    Girl(const Girl& b) : name(b.name), age(b.age) {
      cout << "Girl(const Girl&)" << endl;
    }

    Girl(Girl&& b) : name(move(b.name)), age(b.age) {
      cout << "Girl(Girl&&)" << endl;
    }

    Girl& operator=(const Girl& b) {
      cout << "operator=(const Girl&)" << endl;

      this->name = b.name;
      this->age = b.age;

      return *this;
    }

};

int main() {
  vector<Girl> v1;
  v1.emplace_back("girl1", 1);
  vector<Girl> v2;
  v2.reserve(1);
  std::cout << "==================" << std::endl;
  v2 = std::move(v1);
  std::cout << "==================" << std::endl;
  getchar();
  // vector<Girl> bv;
  //
  // cout << bv.size() << endl;
  // cout << bv.capacity() << endl;
  //
  // bv.emplace_back("example1", 1);
  //
  // cout << bv.size() << endl;
  // cout << bv.capacity() << endl;
  //
  // bv.emplace_back("example2", 2);
  //
  // cout << bv.size() << endl;
  // cout << bv.capacity() << endl;
  //
  // bv.emplace_back("example3", 3);
  //
  // cout << bv.size() << endl;
  // cout << bv.capacity() << endl;
  //
  // bv.emplace_back("example4", 4);
  //
  // cout << bv.size() << endl;
  // cout << bv.capacity() << endl;
  //

  return 1;
}
