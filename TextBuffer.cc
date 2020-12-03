#include <algorithm>
#include "TextBuffer.h"

namespace gsl
{
const char* const TextBuffer::LITERAL_TRUE = "true";
const char* const TextBuffer::LITERAL_FALSE = "false";
const size_t TextBuffer::LITERAL_LEN_TRUE = strlen(TextBuffer::LITERAL_TRUE);
const size_t TextBuffer::LITERAL_LEN_FALSE = strlen(TextBuffer::LITERAL_FALSE);

TextBuffer::TextBuffer(size_t mCapacity) : _size(0), _capacity(mCapacity),
    _truncated(false), _str(NULL)
{
    if (_capacity > 0)
    {
        _str = static_cast<char*>(malloc(_capacity + 1));
        if (_str != NULL)
        {
            _str[0] = '\0';
        }
        else
        {
            _capacity = 0;
        }
    }
}

TextBuffer::~TextBuffer()
{
    if (_str != NULL)
    {
        free(_str);
    }
}

TextBuffer& TextBuffer::TransferTextBuffer(TextBuffer& other)
{
    if (&other != this)
    {
        if (_str != NULL)
        {
            free(_str);
        }
        _size = other._size;
        _capacity = other._capacity;
        _truncated = other._truncated;
        _str = other._str;
        other._size = 0;
        other._capacity = 0;
        other._truncated = false;
        other._str = NULL;
    }
    return *this;
}

void TextBuffer::swap(TextBuffer &other)
{
    std::swap(_str, other._str);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    std::swap(_truncated, other._truncated);
}

void TextBuffer::clear()
{
    if (_str != NULL && _capacity != 0)
    {
        _str[0] = '\0';
    }
    _size = 0;
}

bool TextBuffer::reserve(size_t mCapacity)
{
    if (mCapacity > _capacity)
    {
        if (mCapacity < _capacity * 2)
        {
            mCapacity *= 2;
        }
        char *p = static_cast<char*>(malloc(mCapacity + 1));
        if (NULL == p)
        {
            return false;
        }
        if (_str != NULL)
        {
            memcpy(p, _str, _size + 1);
            ::free(_str);
        }
        _str = p;
        _capacity = mCapacity;
    }
    return true;
}

TextBuffer& TextBuffer::push(const char *str, size_t len)
{
    if (str != NULL && len != 0)
    {
        if (_str != NULL && (_size + len <= _capacity))
        {
            memcpy(_str + _size, str, len);
            _size += len;
            _str[_size] = '\0';
            _truncated = false;
        }
        else
        {
            if (reserve(_size + len))
            {
                memcpy(_str + _size, str, len);
                _size += len;
                _str[_size] = '\0';
                _truncated = false;
            }
            else if (_size < _capacity)
            {
                memcpy(_str + _size, str, _capacity - _size);
                _size = _capacity;
                _str[_size] = '\0';
                _truncated = true;
            }
            else
            {
                _truncated = true;
            }
        }
    }
    return *this;
}

TextBuffer& TextBuffer::push(const char *str)
{
    if (str != NULL)
    {
        push(str, strlen(str));
    }
    else
    {
        _truncated = false;
    }
    return *this;
}

TextBuffer& TextBuffer::vpushf(const char *fmt, va_list arg)
{
    va_list tmp_arg;
    int len = 0;
    if (fmt == NULL)
    {
        return *this;
    }
    if (_str != NULL)
    {
        va_copy(tmp_arg, arg);
        len = vsnprintf(_str + _size, _capacity - _size + 1, fmt, tmp_arg);
        va_end(tmp_arg);
        if (len >= 0 && _size + len <= _capacity)
        {
            _size += len;
            _truncated = false;
            return *this;
        }
    }
    else
    {
        va_copy(tmp_arg, arg);
        len = vsnprintf(NULL, 0, fmt, tmp_arg);
        va_end(tmp_arg);
    }
    if (len > 0)
    {
        if (reserve(_size + len))
        {
            // 新分配空间成功
            va_copy(tmp_arg, arg);
            vsnprintf(_str + _size, _capacity - _size + 1, fmt, tmp_arg);
            va_end(tmp_arg);
            _size += len;
            _truncated = false;
        }
        else
        {
            // 被截断了
            _size = _capacity;
            _truncated = true;
        }
    }
    return *this;
}

TextBuffer& TextBuffer::pushf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vpushf(fmt, args);
    va_end(args);
    return *this;
}

}

int main()
{
  gsl::TextBuffer echo;
  echo.push("hello world");
  printf("%s\n", echo.c_str());
  return 0;
}

