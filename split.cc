#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

template <int __size>
class SplitStr {
  public:
    SplitStr(char *str, char split) {
      if (str == NULL)
        abort();
      field_num_ = 0;
      do {
        arr_[field_num_] = str;
        str = strchr(str, split);
        if (str != NULL) {
          *str = '\0';
          ++str;
        }   
        ++field_num_;
      }while (str != NULL && field_num_ < __size);
    }   
    const char *operator[](uint32_t i) {
      if (i < field_num_) {
        return arr_[i];
      } else if (i < __size) {
        return ""; 
      } else {
        throw "idx error";
        return ""; 
      }   
    }   
    bool bad() {return field_num_ != __size;}
    uint32_t field_num(){return field_num_;}
  private:
    uint32_t field_num_;
    char * arr_[__size];
};

int main()
{
  char str[] = "hello world china";
  SplitStr<5> split(str, ' ');
  printf("%s\n", str);
  return 0;
}

