#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HLL_P 14
#define HLL_Q (64-HLL_P)

#define HLL_REGISTER 1<<HLL_P
#define HLL_P_MASK (HLL_REGISTER - 1)
#define HLL_BITS 6
#define HLL_REGISTER_MAX ((1<<HLL_BITS)-1)


#define HLL_DENSE_GET_REGISTER(target, p, regnum) do {\
    uint8_t* __p = (uint8_t*) p; \
    unsigned long __byte = regnum*HLL_BITS/8;\
    unsigned long __fb = regnum*HLL_BITS&7;\
    unsigned long __fb8 = 8 - __fb;\
    unsigned long __b0 = __p[__byte];\
    unsigned long __b1 = __p[__byte+1];\
    target = ((__b0 >> __fb) | (__b1 << __fb8)) & HLL_REGISTER_MAX;\
  }while(0)

#define HLL_DENSE_SET_REGISTER(p,regnum,val) do { \
  uint8_t *_p = (uint8_t*) p; \
  unsigned long _byte = regnum*HLL_BITS/8; \
  unsigned long _fb = regnum*HLL_BITS&7; \
  unsigned long _fb8 = 8 - _fb; \
  unsigned long _v = val; \
  _p[_byte] &= ~(HLL_REGISTER_MAX << _fb); \
  _p[_byte] |= _v << _fb; \
  _p[_byte+1] &= ~(HLL_REGISTER_MAX >> _fb8); \
  _p[_byte+1] |= _v >> _fb8; \
} while(0)


uint64_t MurmurHash64A(const void* key, int len, unsigned int seed)
{
  const uint64_t m = 0xc6a4a7935bd1e995;
  const int r = 47;
  uint64_t h = seed ^ (len * m);
  const uint8_t* data = (const uint8_t*)key;
  const uint8_t* end  = data + (len-(len&7));

  while (data != end) {
    uint64_t k;

#if (BYTE_ORDER == LITTLE_ENDIAN)
  #ifdef USE_ALIGNED_ACCESS
    memcpy(&k, data, sizeof(uint64_t);
  #else
    k = *((uint64_t*)data);
  #endif
#else
    k = (uint64_t) data[0];
    k |= (uint64_t) data[1] << 8;
    k |= (uint64_t) data[2] << 16;
    k |= (uint64_t) data[3] << 24;
    k |= (uint64_t) data[4] << 32;
    k |= (uint64_t) data[5] << 40;
    k |= (uint64_t) data[6] << 48;
    k |= (uint64_t) data[7] << 56;
#endif
    k *= m;
    k ^= k >> r;
    k *= m;
    h ^= k;
    h *= m;
    data += 8;
  }    

  switch(len & 7) { 
    case 7: h ^= (uint64_t)data[6] << 48; /* fall-thru */
    case 6: h ^= (uint64_t)data[5] << 40; /* fall-thru */
    case 5: h ^= (uint64_t)data[4] << 32; /* fall-thru */
    case 4: h ^= (uint64_t)data[3] << 24; /* fall-thru */
    case 3: h ^= (uint64_t)data[2] << 16; /* fall-thru */
    case 2: h ^= (uint64_t)data[1] << 8; /* fall-thru */
    case 1: h ^= (uint64_t)data[0];
            h *= m; /* fall-thru */
  };   
  h ^= h >> r;
  h *= m;
  h ^= h >> r;
  return h;
}


int hllPatLen(unsigned char* ele, size_t size, long* regp)
{
  uint64_t hash, bit, index;
  int count;

  hash = MurmurHash64A(ele, size, 0xadc83b19ULL);
  index = hash & HLL_P_MASK;
  hash >>= HLL_P;
  hash |= ((uint64_t)1<HLL_Q);

  bit = 1;
  count = 1;
  while ((hash & bit) == 0) {
    count++;
    bit <<= 1;
  }
  *regp = index;
  return count;
}


int hllDenseSet(uint8_t* registers, long index, uint8_t count)
{
  uint8_t oldcount;
  HLL_DENSE_GET_REGISTER(oldcount, registers, index);
  if (count > oldcount) {
    HLL_DENSE_SET_REGISTER(registers, index, count); 
    return 1;
  }
  return 0;
}

int main()
{
  unsigned char ele[] = "Hello HLL";
  long re;
  printf("%d, %lu\n", hllPatLen(ele, 8, &re), re);
  return 0;
}
