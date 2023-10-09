## vector中写完的

- vector(10) 初始化10个位置
- vector(10, 1) 初始化10个1


## 目前写到

## 与标准库不一样的地方
标准库
- 1，SFINAE技术，判断Iterator是否有成员类型
```cpp
__void_t<typename _Iterator::iterator_category,
				      typename _Iterator::value_type,
				      typename _Iterator::difference_type,
				      typename _Iterator::pointer,
				      typename _Iterator::reference>
```
完整代码。
```cpp
template<typename _Iterator>
    struct __iterator_traits<_Iterator,
			     __void_t<typename _Iterator::iterator_category,
				      typename _Iterator::value_type,
				      typename _Iterator::difference_type,
				      typename _Iterator::pointer,
				      typename _Iterator::reference>>
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };
// 这个结构体使用了SFINAE（Substitution Failure Is Not An Error）技术，只有在 _Iterator 具有上述这些成员类型别名时才会进行特化。如果 _Iterator 没有这些成员类型别名中的任何一个，那么这个特化就会被忽略，不会引发编译错误。
```


## 以后可以修改的地方

- 1，SFINAE技术，判断Iterator是否有成员类型
换成这样的。
```cpp
template<typename _Iterator>
    struct __iterator_traits<_Iterator,
			     __void_t<typename _Iterator::iterator_category,
				      typename _Iterator::value_type,
				      typename _Iterator::difference_type,
				      typename _Iterator::pointer,
				      typename _Iterator::reference>>
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };
```
而不是。
```cpp
template <class Iterator>
struct iterator_traits
    : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};   // 根据迭代器是否有 iterator_category 选择不同函数

```


## 知识点

### 计算迭代器间的距离

分 input_iterator_tag 和 random_access_iterator_tag 的版本

```cpp
// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last)
  {
    ++first;
    ++n;
  }
  return n;
}

// distance 的 random_access_iterator_tag 的版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last,
                  random_access_iterator_tag)
{
  return last - first;
}
```

### 萃取器萃取过程
如果是原生指针 T* 或者 const T* 就用偏特化版本
```cpp
// 萃取迭代器的特性
template <class Iterator>
struct iterator_traits
    : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};   // 根据迭代器是否有 iterator_category 选择不同函数


// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag             iterator_category;
    typedef T                                      value_type;
    typedef T*                                     pointer;
    typedef T&                                     reference;
    typedef ptrdiff_t                              difference_type;   
};

// 针对 const * 的偏特化
template <class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag          iterator_category;
    typedef T                                   value_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
    typedef ptrdiff_t                           difference_type;                                  
};

```

如果是普通的类的就是，进入``iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {}``,判断是否有iterator_category属性。
```cpp
template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>
    : public iterator_traits_impl<Iterator,
    /*
    如果 Iterator::iterator_category 能转换为 input_iterator_tag 
    或者 Iterator::iterator_category 能转换为 output_iterator_tag
    就继承自 iterator_traits_impl<Iterator, true>
    因为 迭代器的5个属性中，有3个属于继承的，所以只要判断这两个就可以判断是否有 iterator_category 了、
    has_iterator_cat 判断是否有 iterator_category，这个判断 iterator_category 是不是这 5 个
  */
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>       // 如果 Iterator::iterator_category 能转化为 output_iterator_tag 返回一个true
{
};
```

如果有就进入``iterator_traits_impl<Iterator, true>``，返回该迭代器类中定义的属性就好了。
```cpp
template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::difference_type   difference_type;
};
```


## 自己修改的部分

- 初始化空间的时候，end_和cap_设为一样的

源代码。
```cpp
// init_space 函数
template <class T>
void vector<T>::init_space(size_type size, size_type cap)
{
  try
  {
    begin_ = data_allocator::allocate(cap);
    end_ = begin_ + size;
    cap_ = begin_ + cap;
  }
  catch (...)
  {
    begin_ = nullptr;
    end_ = nullptr;
    cap_ = nullptr;
    throw;
  }
}
```

修改后的。
```cpp
// init_space 函数
template <class T>
void vector<T>::init_space(size_type size)
{
    try
    {
        begin_ = data_allocator::allocate(size);
        end_ = begin_ + size;
        cap_ = end_;
    }
    catch(...)
    {
        begin_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
        throw;
    }
    
}
```