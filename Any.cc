#include <typeinfo> 
#include <algorithm>

class Any {
public:
  Any():content_(0){}

  template <typename TypeValue>
  Any(const TypeValue& value):content_(new Holder<TypeValue>(value)){}

  Any(const Any& other):content_(other.content_ ? other.content_->Clone() : 0) {}

  template <typename TypeValue>
  Any& operator=(const TypeValue& value) 
  {
    Any(value).Swap(*this);
    return *this;
  }

  Any& operator=(const Any& rh)
  {
    Any(rh).Swap(*this);
    return *this;
  }

  Any& Swap(Any& rh)
  {
    std::swap(this->content_, rh.content_);
    return *this;
  }
  ~Any() {
    if (content_) 
    {
      printf("~Any\n");
      delete content_;
    }
  }

private:
  class PlaceHolder {
  public:
    virtual ~PlaceHolder() {}
    virtual const std::type_info& type() const  = 0;
    virtual PlaceHolder* Clone() = 0;
  };

  template <typename TypeValue>
  class Holder : public PlaceHolder {
  public:
    Holder(const TypeValue& value) : value_(value) {}

    virtual const std::type_info& type() const {
      return typeid(value_);
    }
    virtual PlaceHolder* Clone() {
      return new Holder(value_);  
    }
    TypeValue value_;
  };
private:
  PlaceHolder* content_;

  template <typename ValueType>
  friend ValueType* AnyCast(Any* any);
};

template <typename ValueType>
ValueType* AnyCast(Any* any)
{
  return any->content_ && any->content_->type() == typeid(ValueType) ? &static_cast<Any::Holder<ValueType>*>(any->content_)->value_ : 0;
}

int main()
{
  Any any;
  any = 2;
  any = (const char*)"ssss";
  Any any2;
  std::swap(any, any2);
  const char** p = AnyCast<const char*>(&any2);
  printf("%s\n", *p);
  return 0;
}
