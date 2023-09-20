#ifndef TINYSTL_ALLOCATOR_H_
#define TINYSTL_ALLOCATOR_H_

// 包含模板类 allocator，用于管理内存的分配、释放，对象的释放构造

namespace xjintong {
    
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
        typedef ptrdiff_t       difference_type;

    public:
        // 分配内存
        static T*   allocator();
        static T*   allocator(size_type n);

        // 清理内存
        static void deallocate(T* ptr);
        static void deallocate(T* ptr, size_type n);


    };

    template <typename T>
    T* allocator<T>::allocator() {
        return static_cast<T*>(::operator new(sizeof(T)));
    }


}