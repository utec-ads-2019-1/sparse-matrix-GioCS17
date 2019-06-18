#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next;
    //Node<T> *next, *down;
    T value;
    unsigned x;
    unsigned y;

public:
    explicit Node();

    Node(T v):value(v){next=nullptr;}
    Node(T v,unsigned xpos,unsigned ypos):value(v),x(xpos),y(ypos){next=nullptr;}
    unsigned getX() const{ return x;}
    unsigned getY() const{ return y;}
    unsigned getValue() const{ return value;}

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
