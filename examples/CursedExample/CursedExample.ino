#include "CursedDoubleLinkedListInterface.hpp"

//#define FOR_CPP // Uncomment for online compiler eg. https://www.onlinegdb.com/online_c_compiler
#ifdef FOR_CPP
#include <iostream>
#else
#include <Arduino.h>
#endif

// We create our derived class and pass itself to the constructor of our LinkedList
class Object : public CDLLI<Object> {
  public:
    static int statval;
    int val;
    Object() {
        val = statval++; // Just random stuff so we can display constructor calls
    }
};

int Object::statval = 0;

// We can derive our class and we still keep pointers type correct
class ObjectDerived : public Object {
    public:
    ObjectDerived() {
        val = 2137; // [*] (*) <*>
    }
};

// Real usefullness if we want to separate our linked lists
class ObjectSeparate : public CDLLI<ObjectSeparate> {
    public:
    int not_val; // To differentiate from val in object
    ObjectSeparate(int v = 0) : CDLLI(true) { // We put it from the front now
        not_val = 100 + v;
    }
};

// Or we can nest them having one type in one list and second in the other
class ObjectNested : public Object, public ObjectSeparate {
    public:
    ObjectNested() {
        val = 111111;
        not_val = 222222;
    }
};

#ifdef FOR_CPP
int main() {
#else
void setup() {
#endif
    sizeof(Object);
    sizeof(CDLLI<Object>);
    // Lets test:
    Object a, b, c;
    {
        // Lets see if destructing and linkage works as expected
        Object d;
    }
    // Lets see if ObjectDerived will be included in our linked list
    ObjectDerived abba;
    // And if list works after that
    Object e;
    
    // Now lets create separate linked list and see if its still working.
    ObjectSeparate test, test1(5), test2(8);
    
    // And test if Nested object works well
    ObjectNested nested;
    
    #ifdef FOR_CPP
    std::cout << "Printing CDLLI<Object>:" << std::endl;
    #else
    Serial.begin(115200);
    Serial.println("Printing CDLLI<Object>:");
    #endif

    // IMPORTANT - need to use auto&
    // if you use auto destructor will be called many times and you will have bad time
    for(auto& value : *Object::head) {
        #ifdef FOR_CPP
        std::cout << value.val << std::endl;
        #else
        Serial.println(value.val);
        #endif
    }
    
    // Now see if second linked lists is separate and working
    #ifdef FOR_CPP
    std::cout << std::endl << "Printing CDLLI<ObjectSeparate>:" << std::endl;
    #else
    Serial.println();
    Serial.println("Printing CDLLI<ObjectSeparate>:");
    #endif
    
    for(auto& value : test) {
        #ifdef FOR_CPP
        std::cout << value.not_val << std::endl;
        #else
        Serial.println(value.not_val);
        #endif
    }
    #ifdef FOR_CPP
    return 0;
    #endif
}

#ifndef FOR_CPP
    void loop() {
        delay(100);
    }
#endif