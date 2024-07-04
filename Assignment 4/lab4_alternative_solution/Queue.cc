/*
 * Queue implementation.
 *
 * Circular queue using a integer array with capacity Q_CAPACITY.
 *
 * Created by Gustaf on 10/5/2019.
*/

#include "Queue.h"

bool Queue::enqueue(const int e) {
    if (full())
        return false;

    arr[m_tail] = e;
    m_tail = (m_tail + 1) % Q_CAPACITY;
    ++m_len;

    return true;
}

bool Queue::dequeue(int &e) {
    if (empty())
        return false;

    e = arr[m_head];

    m_head = (m_head + 1) % Q_CAPACITY;
    --m_len;

    return true;
}

bool Queue::full() {
    return m_len == Q_CAPACITY;
}

bool Queue::empty() {
    return m_len == 0;
}

int Queue::length() {
    return m_len;
}
