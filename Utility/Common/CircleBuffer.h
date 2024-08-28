#ifndef CIRCLEBUFFER_H
#define CIRCLEBUFFER_H

#include <vector>
#include <cstdint>
#include <stdexcept>


template<typename T>
class CircleBuffer
{
public:
    CircleBuffer()
    {
        m_buffer.resize(m_capacity + 1);
    }

    explicit CircleBuffer(size_t capacity) : m_capacity(capacity + 1)
    {
        m_buffer.resize(capacity + 1);
    }

    // Inserts an item into the back of the buffer
    void push_back(T item)
    {
        m_buffer[m_head] = item;
        m_head = (m_head + 1) % m_capacity;

        // Update the tail when the m_head reaches m_tail that is when the buffer gets full
        // and would override m_tail.
        if (m_head == m_tail)
        {
            m_tail = (m_tail + 1) % m_capacity;
        }
        else
        {
            m_size++;
        }
    }

    // Removes an item from the back of the buffer and returns it
    T pop_back()
    {
        if (empty())
            throw std::out_of_range("Buffer is empty");

        T item = m_buffer[m_head - 1];
        m_head--;

        if (m_head == 0)
            m_head = m_capacity;

        m_size--;
        return item;
    }

    // Removes an item from the front of the buffer and returns it
    T pop_front()
    {
        if (empty())
            throw std::out_of_range("Buffer is empty");

        T item = m_buffer[m_tail];
        m_tail = (m_tail + 1) % m_capacity;
        m_size--;
        return item;
    }

    // Returns the element at the front of the buffer (the tail)
    T front()
    {
        return m_buffer[m_tail];
    }

    // Returns the element at the back of the buffer (the head)
    T back()
    {
        return m_buffer[m_head - 1];
    }

    // Returns if the buffer is empty or not
    bool empty() const
    {
        return m_head == m_tail;
    }

    // Returns if the buffer is full or not
    bool full() const
    {
        return (m_head + 1) % m_capacity == m_tail;
    }

    // Returns the size of the buffer
    size_t size() const
    {
        return m_size;
    }

private:
    // Internal buffer
    std::vector<T>  m_buffer;
    // Head index
    std::uint32_t   m_head{ 0 };
    // Tail index
    std::uint32_t   m_tail{ 0 };
    // Total capacity
    size_t          m_capacity{ 128 };
    // Current size    
    size_t          m_size{ 0 };
};

#endif