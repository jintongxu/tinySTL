# tinySTL

tinySTL项目是实现的简单的STL。本来想都写的，但是由于学业紧张，只实现了vector。

## 一、notes

里面记录了做这个项目过程中的一些笔记。比如，一些知识点、代码的实现和标准库不一样的地方...

## 二、source

这个文件夹里面放的是STL具体的实现代码。

- algo.h

这个头文件包含了 mystl 的一系列算法，比如说将某个区间的元素反转的算法。

- algobase.h

这个头文件包含了 mystl 的基本算法，比如说``fill_n``从first位置开始填充n个值，``copy_backward``，``move_backward``等等。

- allocator.h

**分配器**，这个头文件包含模板类 allocator，分配器就用于处理容器对[内存的分配与释放](https://zh.wikipedia.org/wiki/内存管理)请求。

```cpp
// 模板类：allocator
// 模板函数代表数据类型
template <typename T>
class allocator {
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t    difference_type;
public:
    // 分配内存
    static T*   allocate();
    static T*   allocate(size_type n);

    // 清理内存
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);

    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);

    template <class... Args>
    static void construct(T* ptr, Args&& ...args);


    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
};
```

- construct.h

这个头文件包含两个函数 construct, destroy

- exceptdef.h

这个是Debug的头文件。

- iterator.h

**迭代器**，这个是迭代器的头文件。包含了迭代器模板结构体与一些全局函数，还有萃取器。

例如五种迭代器类型，源码如下：

```cpp
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
```

- memory.h

这个头文件负责更高级的动态内存管理。包含一些基本函数、空间适配器、未初始化的存储空间管理，以及一个模板类 auto_ptr。

- type_traits.h

这个头文件用于提取类型信息。

- uninitialized.h

- util.h

这个文件包含一些通用工具，包括 move，forward，swap 等函数，以及 pair 等。

- vector.h

这个不用多说，包含vector模板类。



