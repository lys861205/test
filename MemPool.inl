#include "MemPool.h"
#include <unistd.h>

namespace userserver 
{

#define MODE_BASE 0xff

template<class T>
MemPool<T>::MemPool(uint32_t expired_time):
  _ptr(nullptr),
  _cap(0),
  _block(0),
  _expired_time(expired_time)
{
}

template<class T>
MemPool<T>::~MemPool()
{
  if (_ptr) 
  {
    free(_ptr);
  }
}

template<class T>
int MemPool<T>::alloc(uint32_t block, uint64_t nmemb)
{
  _block = block;
  _mod = block * MODE_BASE;
  int header_size = block * sizeof(Metadata);
  _cap = header_size + block * nmemb;
  _ptr = (Metadata*)malloc(_cap);
  if (nullptr == _ptr) return -1;

  char* buf = (char*)(_ptr + header_size);
  for (uint32_t i = 0; i < block; ++i)
  {
    ((Metadata*)_ptr+i)->_begin = 0;
    ((Metadata*)_ptr+i)->_tail = 0;
    ((Metadata*)_ptr+i)->_size = 0;
    ((Metadata*)_ptr+i)->_cap = nmemb;
    ((Metadata*)_ptr+i)->_warn_size = nmemb - 1024;
    ((Metadata*)_ptr+i)->_buf = buf + i * nmemb;
  }
  return 0;
}

template<class T>
T* MemPool<T>::get_item(const uint64_t& key, uint64_t timestamp, uint64_t size)
{
  T* pitem;
  //计算i
  uint32_t i = get_index(key);
  printf("%d", i);
  Metadata* ptr = (Metadata*)(_ptr + i);

  ptr->_mu.lock();
  while (is_expired(ptr, timestamp) || not is_enough_space(ptr, size)) 
  {
    ptr->Free(); 
  }
  pitem = ptr->Malloc(size);
  ptr->_mu.unlock();

  return pitem;
}

template<class T>
uint32_t MemPool<T>::get_index(const uint64_t& key)
{
  return key % _block;
  /*
  uint32_t num = key % _mod;
  for (int i = 0; i < _block; ++i) {
    int B = _block - 1 - i;
    if ((num >> (B*8)) & MODE_BASE) {
      return B;
    }
  }
  return 0;
  */
}

template<class T>
bool MemPool<T>::is_expired(void* ptr, uint64_t curstamp)
{
  Metadata* pmeta = (Metadata*)ptr;
  uint64_t begin = pmeta->_begin;
  uint64_t tail  = pmeta->_tail;
  T* pitem = (T*)pmeta->get_head_ptr();
  return (begin != tail && (curstamp > pitem->ts() + _expired_time));
}

template<class T>
bool MemPool<T>::is_enough_space(void* ptr, uint64_t need_size)
{
  Metadata* pmeta = (Metadata*)ptr;
  uint64_t left_size = pmeta->_cap - pmeta->_size;
  uint64_t tail = pmeta->_tail;
  uint64_t warn  = pmeta->_warn_size;
  return (left_size >= need_size && (tail + need_size <= warn));
}

template<class T>
typename MemPool<T>::Metadata* MemPool<T>::get_max_left_space_block() const 
{
  uint32_t block_index = 0;
  uint32_t max_size = 0;
  for (uint32_t i = 0; i < _block; ++i) 
  {
    Metadata* ptr = (Metadata*)(_ptr + i);
    uint64_t left_size = ptr->_cap - ptr->_size;
    if (max_size < left_size) 
    {
      max_size = left_size;
      block_index = i;
    }
  }
  return (Metadata*)(_ptr + block_index);
}

template<class T>
int MemPool<T>::store()
{
  int ret = 0;
//#pragma omp parallel for 
  for (int i = 0; i < _block; ++i) 
  {
    ret += store_block(i); 
  }
  return ret;
}

template<class T>
int MemPool<T>::store_block(int blockid)
{
  Metadata* ptr = (Metadata*)(_ptr + blockid); 
  if (nullptr == ptr) {
    return -1;
  }
  char filename[256] = {0};
  snprintf(filename, sizeof filename, "data/block/cv_%d.dat", blockid);
  char tempfile[256] = {0}; 
  snprintf(tempfile, sizeof tempfile, "data/block/cv_%d.dat.tmp", blockid);
  FILE* fp = fopen(tempfile, "wb");
  if (nullptr == fp) {
    return -1;
  }
  uint64_t begin = ptr->_begin;
  uint64_t tail  = ptr->_tail;
  int ret = -1;
  do {
    ptr->_mu.lock();
    if (tail > begin) {
      if (_fwrite(fp, ptr->_buf + begin, ptr->_size) == -1)  {
        fprintf(stderr, "write block[%d] failed, error: %s\n", blockid, strerror(errno));
        break;
      }
    } else if (tail < begin) {
      uint64_t size = ptr->_size - tail;
      if (_fwrite(fp, ptr->_buf + begin, size) == -1) {
        fprintf(stderr, "write block[%d] failed, error: %s\n", blockid, strerror(errno));
        break;
      }
      if (_fwrite(fp, ptr->_buf, tail) == -1) {
        fprintf(stderr, "write block[%d] failed, error: %s\n", blockid, strerror(errno));
        break;
      }
    }
    if (_rename(tempfile, filename) == -1) {
      break;
    }
    ret = 0;
  } while (0);
  ptr->_mu.unlock();
  fclose(fp);
  return ret;
}

template<class T>
int MemPool<T>::_fwrite(FILE* fp, const void* ptr, size_t len)
{
  assert(!fp);
  if (nullptr == ptr || len == 0) {
    return 0;
  }
  while (len) {
    size_t n = fwrite(ptr, len, 1, fp); 
    len -= n;
  }
  return 0;
}

template<class T>
int MemPool<T>::_rename(const char* oldfile, const char* newfile)
{
  if (rename(oldfile, newfile) == -1) {
    char cwd[256];
    char *cwdp = getcwd(cwd, 256);
    fprintf(stderr,   
        "Error moving temp DB file %s on the final "
        "destination %s (in userserver data/block dir %s): %s",
        oldfile,
        newfile,
        cwdp ? cwdp : "unknown",
        strerror(errno)); 
    return -1;
  }
  return 0;
}

template<class T>
int MemPool<T>::restore()
{
  int ret = 0;
#pragma omp parallel for 
  for (int i = 0; i < _block; ++i) 
  {
    ret += restore_block(i); 
  }
  return ret;
}

template<class T>
int MemPool<T>::restore_block(int blockid)
{
  Metadata* ptr = (Metadata*)(_ptr + blockid); 
  if (nullptr == ptr) {
    return -1;
  }

  char filename[256] = {0};
  snprintf(filename, sizeof filename, "data/block/cv_%d.dat", blockid);
  FILE* fp = fopen(filename, "wb");
  if (nullptr == fp) {
    return -1;
  }
  fseek(fp, 0, SEEK_END); 
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (_fread(fp, ptr->_buf, size) == -1) {
    fprintf(stderr, "read block[%d] failed, error: %s\n", blockid, strerror(errno));
    fclose(fp);
    return -1;
  }
  fclose(fp);

  //修正元数据
  ptr->_tail = size;
  ptr->_size = size;
  return 0;
}

template<class T>
int MemPool<T>::_fread(FILE* fp, void* ptr, size_t len)
{
  assert(!fp);
  if (nullptr == ptr || len == 0) {
    return 0;
  }
  while (len) {
    size_t n = fread(ptr, len, 1, fp);
    len -= n;
  }
  return 0;
}

}



































