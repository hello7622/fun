#include "vector.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Allocator& alloc)
    : m_pVec(nullptr), m_size(0), m_capacity(0), m_alloc(alloc)
{
    resize(10);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_t size, const T& value, const Allocator& alloc)
    : m_pVec(nullptr), m_size(0), m_capacity(0), m_alloc(alloc)
{
    resize(size << 1);
    for (size_t i = 0; i < size; ++i) {
        construct(&m_pVec[i], value);
    }
    m_size = size;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& vec)
    : m_pVec(nullptr), m_size(vec.m_size), m_capacity(0), m_alloc(
          std::allocator_traits<Allocator>::select_on_container_copy_construction(vec.m_alloc))
{
    resize(vec.m_capacity);
    for (size_t i = 0; i < m_size; ++i) {
        construct(&m_pVec[i], vec.m_pVec[i]);
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& vec) noexcept
: m_pVec(vec.m_pVec), m_size(vec.m_size), m_capacity(vec.m_capacity), 
m_alloc(std::move(vec.m_alloc))
{
    vec.m_pVec = nullptr;
    vec.m_size = 0;
    vec.m_capacity = 0;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const std::initializer_list<T>& list, const Allocator& alloc)
: m_pVec(nullptr), m_size(0), m_capacity(0), m_alloc(alloc)
{
    resize(list.size() << 1);
    for (const T& value : list) {
        construct(&m_pVec[m_size++], value);
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector()
{
    destroy();
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& vec)
{
    if (this == &vec) {
        return *this;
    }

    // 处理分配器传播
    if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
        m_alloc = vec.m_alloc;
    }

    if (m_capacity < vec.m_size) {
        destroy();
        resize(vec.m_size << 1);
    }

    m_size = vec.m_size;
    for (size_t i = 0; i < m_size; ++i) {
        construct(&m_pVec[i], vec.m_pVec[i]);
    }
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& vec) noexcept
{
    if (this == &vec) {
        return *this;
    }
    
    destroy();
    
    // 处理分配器传播
    if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
        m_alloc = std::move(vec.m_alloc);
    }
    
    m_pVec = vec.m_pVec;
    m_size = vec.m_size;
    m_capacity = vec.m_capacity;
    
    vec.m_pVec = nullptr;
    vec.m_size = 0;
    vec.m_capacity = 0;
    
    return *this;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push(const T& value, size_t index)
{
    if (m_size == m_capacity) {
        resize(m_capacity << 1);
    }
    
    if (index >= m_size) {
        construct(&m_pVec[m_size++], value);
    } else {
        for (size_t i = m_size; i > index; --i) {
            construct(&m_pVec[i], std::move(m_pVec[i - 1]));
            destroy(&m_pVec[i - 1]);
        }
        construct(&m_pVec[index], value);
        ++m_size;
    }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push(T&& value, size_t index)
{
    if (m_size == m_capacity) {
        resize(m_capacity << 1);
    }
    
    if (index >= m_size) {
        construct(&m_pVec[m_size++], std::move(value));
    } else {
        for (size_t i = m_size; i > index; --i) {
            construct(&m_pVec[i], std::move(m_pVec[i - 1]));
            destroy(&m_pVec[i - 1]);
        }
        construct(&m_pVec[index], std::move(value));
        ++m_size;
    }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pop(size_t index)
{
    if (m_size == 0) {
        return;
    }
    if (index >= m_size) {
        index = m_size - 1;
    }
    
    destroy(&m_pVec[index]);
    for (size_t i = index; i < m_size - 1; ++i) {
        construct(&m_pVec[i], std::move(m_pVec[i + 1]));
        destroy(&m_pVec[i + 1]);
    }
    --m_size;
    
    if ((m_size << 2) < m_capacity) {
        resize(m_size << 1);
    }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::clear()
{
    for (size_t i = 0; i < m_size; ++i) {
        destroy(&m_pVec[i]);
    }
    m_size = 0;
    resize(10);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_t new_capacity)
{
    if (new_capacity > m_capacity) {
        resize(new_capacity);
    }
}

template <typename T, typename Allocator>
size_t Vector<T, Allocator>::size() const noexcept
{
    return m_size;
}

template <typename T, typename Allocator>
size_t Vector<T, Allocator>::index(const T &value) const
{
    for (size_t i = 0; i < m_size; ++i) {
        if (m_pVec[i] == value) {
            return i;
        }
    }
    return -1;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::empty() const noexcept
{
    return m_size == 0;
}

template <typename T, typename Allocator>
T &Vector<T, Allocator>::at(size_t index)
{
    if (index >= m_size) {
        throw std::out_of_range("索引越界");
    }
    return m_pVec[index];
}

template <typename T, typename Allocator>
T &Vector<T, Allocator>::operator[](size_t index)
{
    return at(index);
}

template <typename T, typename Allocator>
const T &Vector<T, Allocator>::at(size_t index) const
{
    if (index >= m_size) {
        throw std::out_of_range("索引越界 const");
    }
    return m_pVec[index];
}

template <typename T, typename Allocator>
const T &Vector<T, Allocator>::operator[](size_t index) const
{
    return at(index);
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin() const
{
    return m_pVec;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::end() const
{
    return m_pVec + m_size;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(size_t new_capacity)
{
    if (new_capacity == m_capacity) {
        return;
    }

    pointer new_data = std::allocator_traits<Allocator>::allocate(m_alloc, new_capacity);
    
    if (m_pVec) {
        try {
	 		for (size_t i = 0; i < m_size && i < new_capacity; ++i) {
				construct(&new_data[i], std::move(m_pVec[i]));
        	}
    	} catch (...) {
        	for (size_t i = 0; i < m_size && i < new_capacity; ++i) {
				destroy(&new_data[i]);
        	}
         	std::allocator_traits<Allocator>::deallocate(m_alloc, new_data, new_capacity);
		    throw;
    	}

     	for (size_t i = 0; i < m_size; ++i) {
        		destroy(&m_pVec[i]);
    	}

        std::allocator_traits<Allocator>::deallocate(m_alloc, m_pVec, m_capacity);
    }

    m_pVec = new_data;
    m_capacity = new_capacity;
    if (m_size > m_capacity) {
        m_size = m_capacity;
    }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::destroy()
{
    for (size_t i = 0; i < m_size; ++i) {
        destroy(&m_pVec[i]);
    }
    if (m_pVec) {
        std::allocator_traits<Allocator>::deallocate(m_alloc, m_pVec, m_capacity);
    }
    m_pVec = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::construct(pointer p, const T& value)
{
    std::allocator_traits<Allocator>::construct(m_alloc, p, value);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::construct(pointer p, T&& value)
{
    std::allocator_traits<Allocator>::construct(m_alloc, p, std::move(value));
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::destroy(pointer p)
{
    std::allocator_traits<Allocator>::destroy(m_alloc, p);
}
