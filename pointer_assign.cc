#include <pthread.h>
#include <stdio.h>

// 不让编译器自动扩充结构体对齐特征，这个在网络协议以及序列号操作中很常见。
#pragma pack(1)
struct pack {
  // 60字节的padding
  char unsued[60];
  // cacheline仅剩下4字节，仅够装指针p的4字节，剩余4字节跨越到另一个cacheline
  long *p;
};
#pragma pack()

// 内存对齐粒度最小为1字节，我设置64个元素的数组，总有一个元素恰好是在64字节边界的！
struct pack pa[64];

// 保存64字节边界的元素
struct pack *used;

long a = 0x1122334455667788;
long b = 0x8877665544332211;

void *write_value1(void *param)
{
  for (;;) {
    used->p = (long *)a;
    // 不让编译器优化
    asm volatile("" ::: "memory");
  }
  return NULL;
}

void *write_value2(void *param)
{
  for (;;) {
    used->p =(long *)b;
    asm volatile("" ::: "memory");
  }
  return NULL;
}

int main(int argc, char **argv)
{
  int i;
  long *p;
  pthread_t t1, t2;

  // for循环找到那个64字节边界的pack结构体元素
  for (i = 0; i < 64; i++) {
    unsigned long addr;
    addr = (unsigned long)&pa[i];
    if (addr%64 == 0) {
      // 赋值给used
      used = (struct pack *)addr;
      break;
    }
  }

  pthread_create(&t1, NULL, write_value1, NULL);
  pthread_create(&t2, NULL, write_value2, NULL);

  while (1) {
    p = used->p;
    // 我们看能不能找到既不是a，又不是b的p
    if (p != (long *)a && p != (long *)b) {
      printf("%lx\n", (unsigned long)p);
    }
  }

  return 0;
}
