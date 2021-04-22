#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__
#include <mutex>
#include <atomic>

namespace userserver 
{
template<class T>
class MemPool {
  struct Metadata 
  {
    std::atomic<uint64_t> _begin;
    std::atomic<uint64_t> _tail;
    std::atomic<uint64_t> _size;
    uint64_t _cap;
    uint64_t _warn_size;
    std::mutex _mu;
    void* _buf;

    void Free()
    {
      T* pitem = (T*)get_head_ptr();
      size_t s = pitem->item_size();
      _begin += s;
      _size -= s;
      pitem->done();
      if (_begin > _warn_size) 
      {
        _begin = 0;
      }
    }
    T* Malloc(size_t size)
    {
      T* pitem;
      pitem = (T*)get_tail_ptr();
      _size += size;
      _tail += size;
      if (_tail > _warn_size)
      {
        _tail = 0;
      }
      return pitem;
    }
    void* get_head_ptr() const
    {
      uint32_t len = _begin;
      return (_buf + len);
    }
    void * get_head_ptr_and_commit(uint32_t offset) 
    {
      _size -= offset;
      _begin += offset;
      uint32_t len = _begin;
      return (_buf + len);
    }
    void* get_tail_ptr() const 
    {
      uint32_t len = _tail;
      return (_buf + len);
    }
    void* get_tail_ptr_and_commit(uint32_t offset)
    {
      _size += offset;
      _tail += offset;
      uint32_t len = _tail;
      return (_buf + len);
    }
  };
public:
  MemPool(uint32_t expired_time = 1800);
  ~MemPool();
  int alloc(uint32_t block=1, uint64_t nmemb=1024*1024*1024*8LL);
  T* get_item(const uint64_t& key, uint64_t timestamp, uint64_t size);
  int store();
  int restore();
private:
  int store_block(int blockid);
  int restore_block(int blockid);
  bool is_expired(void* ptr, uint64_t curstamp);
  bool is_enough_space(void* ptr, uint64_t need_size);
  Metadata* get_max_left_space_block() const;
  uint32_t get_index(const uint64_t& key);

  int _fwrite(FILE* fp, const void* ptr, size_t len);
  int _fread(FILE* fp, void* ptr, size_t len);
  int _rename(const char* oldfile, const char* newfile);
private:
  Metadata* _ptr; 
  uint64_t _cap;
  uint32_t _block;
  uint32_t _expired_time;
  uint32_t _mod;
};
}

#include "MemPool.inl"

#endif // __MEM_POOL_H__
