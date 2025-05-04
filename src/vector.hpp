#ifndef __LIN_VECTOR_HPP__
#define __LIN_VECTOR_HPP__

#include <cstddef>
#include <initializer_list>
#include <memory>  // 添加allocator_traits支持

template <typename T, typename Allocator = std::allocator<T>>
class Vector 
{
public:
    // 类型定义
    using allocator_type = Allocator;
    using value_type = T;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using Iterator = const T*;

    // 构造函数
    explicit Vector(const Allocator& alloc = Allocator());
    Vector(size_t size, const T& value = T(), const Allocator& alloc = Allocator());
    Vector(const Vector& vec);
    Vector(Vector&& vec) noexcept;
    Vector(const std::initializer_list<T>& list, const Allocator& alloc = Allocator());
    ~Vector();

    // 赋值运算符
    Vector& operator=(const Vector& vec);
    Vector& operator=(Vector&& vec) noexcept;

    // 容量操作
    void push(const T& value, size_t index = -1);
    void push(T&& value, size_t index = -1);
    void pop(size_t index = -1);
    void clear();
    void reserve(size_t new_capacity);

    // 元素访问
    size_t size() const noexcept;
    size_t index(const T& value) const;
    bool empty() const noexcept;
    T& at(size_t index);
    T& operator[](size_t index);
    const T& at(size_t index) const;
    const T& operator[](size_t index) const;

    // 迭代器
    Iterator begin() const;
    Iterator end() const;

    // 分配器访问
    allocator_type get_allocator() const noexcept;

protected:
    void resize(size_t new_capacity);
    void destroy();
    // 辅助函数
    void construct(pointer p, const T& value);
    void construct(pointer p, T&& value);
    void destroy(pointer p);

private:
    T* m_pVec;
    size_t m_size;
    size_t m_capacity;
    Allocator m_alloc;  // 分配器实例

};

#endif // __LIN_VECTOR_HPP__
