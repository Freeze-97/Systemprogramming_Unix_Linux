/*
 * Queue implementation.
 *
 * Circular queue using a integer array with capacity Q_CAPACITY.
 *
 * Created by Gustaf on 10/5/2019.
*/

#ifndef LAB4_QUEUE_H
#define LAB4_QUEUE_H

#define Q_CAPACITY 10

class Queue {
private:
    int arr[Q_CAPACITY];
    int m_head, m_tail;
    int m_len;
public:
    Queue()
        :m_head(0), m_tail(0), m_len(0) {};

    bool enqueue(const int e);
    bool dequeue(int &e);
    int length();
    bool full();
    bool empty();

};


#endif //LAB4_QUEUE_H
