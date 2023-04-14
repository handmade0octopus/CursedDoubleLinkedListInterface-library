#ifndef CURSED_DOUBLE_LINKED_LIST_INTERFACE_HPP
#define CURSED_DOUBLE_LINKED_LIST_INTERFACE_HPP
// CursedDoubleLinkedListInterface.hpp
// Library is what it name is - it's a cursed linked list interface for C++
// but I made it for Arduino just to mess with more people heads.
//
// It works great if you want to create objects and have them accesible from anywhere
// Good for stack allocation as well as heap or static allocation
//
// Its supposed to be used as intefrace and templated.
// It will return pointer to the T object on iteration so it's useful to remove boilerplate code
// Enjoy and please note this is MIT licensed so I am not responsible - especially for damage while reading it
//
// This library is **evil**, you have been warned

// Derive it and pass your class type into T
template <typename T>
class CDLLI {
    // _head and _tail are static, this way you can use it easily with T._head for iteration
    static CDLLI<T>* _head;
    static CDLLI<T>* _tail;
    // We only need _next but _prev is useful for handling it faster
    // and accesing nearby nodes faster
    CDLLI<T>* _next;
    CDLLI<T>* _prev;
 public:
    
    
    
    // Initialization - on construction object is automatically added to the linked list
    explicit CDLLI(bool toFront = false) {
        if(toFront) {
            _prev = nullptr;
            if(_head) _head->_prev = this;
            _next = _head;
            _head = this;
        } else {
            _prev = _tail;
            if(!_head) _head = this;
            if(_tail) _tail->_next = this;
            _tail = this;
            _next = nullptr;
        }
    }
    
    virtual ~CDLLI() {
        if(_prev) _prev->_next = _next;
        if(_next) _next->_prev = _prev;
        if(_tail == this) _tail = _prev;
        if(_head == this) _head = _next;
    }
    
    // NOTE: use object.T::next() if object is inherited by different lists
    T* next() { return ((T*) _next); }
    T* previous() { return ((T*) _prev); }

    static T* _head() { return ((T*) _head); }
    static T* _tail() { return ((T*) _tail); }
    
    class Iterator {
        CDLLI<T>* current;

     public:
        explicit Iterator(CDLLI<T>* CDLLI) : current(CDLLI) {}

        Iterator& operator++() {
            current = current->_next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        T& operator*(){
            return *((T*) current);
        }
    };

    Iterator begin() {
        return Iterator(_head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

template <typename T>
CDLLI<T>* CDLLI<T>::_head = nullptr;
template <typename T>
CDLLI<T>* CDLLI<T>::_tail = nullptr;

#endif