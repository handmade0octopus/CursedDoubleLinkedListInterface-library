
// CursedDoubleLinkedListInterface.hpp
// Library is what it name is - it's a cursed linked list interface for C++
// but I made it for Arduino just to mess with peoples heads.
//
// It works great if you want to create objects and have them accesible from anywhere
// Good for stack allocation as well as heap or static allocation
//
// Its supposed to be used as intefrace and templated.
// It will return pointer to the T object on iteration so it's useful to remove boilerplate code
// Enjoy and please note this is MIT licensed so I am not responsible - especially for damage while reading it

template <typename T>
class CDLLI {
 public:
    // Head and tail are static, this way you can use it easily with T.head for iteration
    static CDLLI<T>* head;
    static CDLLI<T>* tail;
    // We only need next but previous is useful for handling it faster
    // and accesing nearby nodes faster
    CDLLI<T>* next;
    CDLLI<T>* previous;
    
    // Initialization - on construction object is automatically added to the linked list at the back
    explicit CDLLI(bool toFront = false) : next(nullptr) {
        if(toFront) {
            
        } else {
            previous = tail;
            if(!head) head = this;
            if(tail) tail->next = this;
            tail = this;
        }
        
        std::cout << sizeof(*this) << std::endl;
    }
    
    virtual ~CDLLI() {
        if(previous) {
            previous->next = next;
            if(next) next->previous = previous;
            else tail = previous;
        } else head = nullptr;
    }
    
    
    class Iterator {
        CDLLI<T>* current;

     public:
        explicit Iterator(CDLLI<T>* CDLLI) : current(CDLLI) {}

        Iterator& operator++() {
            current = current->next;
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
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

template <typename T>
CDLLI<T>* CDLLI<T>::head = nullptr;
template <typename T>
CDLLI<T>* CDLLI<T>::tail = nullptr;