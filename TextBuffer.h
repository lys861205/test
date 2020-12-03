#ifndef _TEXTBUFFER_H_
#define _TEXTBUFFER_H_

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

namespace gsl
{
class TextBuffer
{
public:
    explicit TextBuffer(size_t mCapacity = DEFAULT_CAPACITY);
    ~TextBuffer();
    TextBuffer& TransferTextBuffer(TextBuffer &other);
    void swap(TextBuffer& other);
    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }
    bool full() const { return _size == _capacity; }
    bool truncated() const { return _truncated; }
    const char* c_str() const { return (_str != NULL ? _str : ""); }
    void clear();
    bool reserve(size_t mCapacity);
    TextBuffer& pushf(const char* fmt, ...) __attribute__((format (printf, 2, 3)));
    TextBuffer& vpushf(const char* fmt, va_list args);

    TextBuffer& push(const char *str, size_t len);
    TextBuffer& push(const char *str);
    TextBuffer& push(char c)
    {
        if (c == '\0')
        {
            return *this;
        }
        if (_size == _capacity)
        {
            reserve(_size + 1);
        }
        if (_size < _capacity)
        {
            _str[_size++] = c;
            _str[_size] = '\0';
            _truncated = false;
        }
        else
        {
            _truncated = true;
        }
        return *this;
    }

    TextBuffer& operator << (const TextBuffer& other)
    {
        return push(other._str, other._size);
    }
    TextBuffer& operator << (const std::string& str)
    {
        return push(str.c_str(), str.length());
    }
    TextBuffer& operator << (const char* str) { return push(str); }
    TextBuffer& operator << (const bool flag)
    {
        if (flag)
        {
            return push(LITERAL_TRUE, LITERAL_LEN_TRUE);
        }
        else
        {
            return push(LITERAL_FALSE, LITERAL_LEN_FALSE);
        }
    }
    TextBuffer& operator << (char c)
    {
        if (c == '\0')
        {
            return *this;
        }
        if (_size == _capacity)
        {
            reserve(_size + 1);
        }
        if (_size < _capacity)
        {
            _str[_size++] = c;
            _str[_size] = '\0';
            _truncated = false;
        }
        else
        {
            _truncated = true;
        }
        return *this;
    }
    TextBuffer& operator << (signed char sc) { return pushf("%hhd", sc); }
    TextBuffer& operator << (unsigned char uc) { return pushf("%hhu", uc); }
    TextBuffer& operator << (wchar_t wc) { return pushf("%lc", wc); }
    TextBuffer& operator << (const wchar_t *ws)
    {
        if (ws != NULL)
        {
            pushf("%ls", ws);
        }
        else
        {
            _truncated = false;
        }
        return *this;
    }
    TextBuffer& operator << (short s) { return pushf("%hd", s); }
    TextBuffer& operator << (unsigned short s) { return pushf("%hu", s); }
    TextBuffer& operator << (int i) { return pushf("%d", i); }
    TextBuffer& operator << (unsigned int u) { return pushf("%u", u); }
    TextBuffer& operator << (long l) { return pushf("%ld", l); }
    TextBuffer& operator << (unsigned long ul) { return pushf("%lu", ul); }
    TextBuffer& operator << (long long ll) { return pushf("%lld", ll); }
    TextBuffer& operator << (unsigned long long ull) { return pushf("%llu", ull); }
    TextBuffer& operator << (double d) { return pushf("%lg", d); }
    TextBuffer& operator << (long double d) { return pushf("%Lg", d); }
    TextBuffer& operator << (void *p) { return pushf("%p", p); }
    TextBuffer& operator << (float f) { return pushf("%f", f); }
public:
    static const int DEFAULT_CAPACITY = 64;
    static const char* const LITERAL_TRUE;
    static const char* const LITERAL_FALSE;
private:
    TextBuffer(const TextBuffer& other);
    TextBuffer& operator = (const TextBuffer &);

    size_t _size; // 字符串长度
    size_t _capacity; // 容量
    bool _truncated; // 最后一次操作是否发生截断
    char *_str; // 字符串缓冲区

    static const size_t LITERAL_LEN_TRUE;
    static const size_t LITERAL_LEN_FALSE;
};
}

#endif /* _TEXTBUFFER_H_ */
