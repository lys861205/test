#include <stdio.h>
#include <stack>


int calc(const char* str, int& offset)
{
  if (NULL == str || str[0] == '\0') return 0;
  std::stack<int> s;
  const char* p = str;
  int sign = 1;
  int num = 0;
  for (; *p != '\0'; p += offset) {
    ++offset;
    if (*p == ' ') {
      continue;
    }
    if (*p == '-' || *p == '+') {
      if (num != 0) {
        s.push(num*sign);
        num = 0;
      }
      if (*p == '-')
        sign = -1; //负数 
      else if (*p == '+') 
        sign = 1; //正数
    }
    else if (*p >= '0' && *p <= '9') {
      num = num * 10 + (*p - '0');
    }
    else if (*p == '(') {
      num = calc(p+1, offset);
      s.push(num*sign);
      sign = 1;
      num = 0;
    }
    else if (*p == ')') {
      break;
    }
    else {
      return -1;
    }
  }
  s.push(sign*num);
  int sum = 0;
  while (!s.empty()) {
    sum += s.top();s.pop(); 
  }
  return sum;
}

int main()
{
  const char* str = "4 + (5 - 2)";
  int offset = 0;
  printf("%d\n", calc(str, offset));
  return 0;
}
