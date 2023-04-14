#include <iostream>
#include "CursedDoubleLinkedListInterface.hpp"
    
    
class Object : public CDLLI<Object> {
  public:
    static int statval;
    int val;
    Object() {
        val = statval++;
    }
};

int Object::statval = 0;

class ObjectTwo : public Object {
    public:
    ObjectTwo() {
        val = 2137;
    }
    
};

class ObjectThree : public CDLLI<ObjectThree> {
    public:
    int not_val; // To differentiate from val in object
    ObjectThree(int v = 0) {
        not_val = 100 + v;
    }
};
int main() {
    // Lets test:
    Object a, b, c;
    {
        // Lets see if destructing and linkage works as expected
        Object d;
    }
    // Lets see if ObjectTwo will be included in our linked list
    ObjectTwo abba;
    // And if list works after that
    Object e;
    
    // Now lets create separate linked list and see if its still working.
    ObjectThree test, test1(5), test2(8);
    
    std::cout << "Printing CDLLI<Object>:" << std::endl;
    // IMPORTANT - need to use auto&
    // if you use auto destructor will be called many times and you will have bad time
    for (auto& value : *Object::head()) {
        std::cout << value.val << std::endl;
    }
    
    // Now see if second linked lists is separate and working
    std::cout << std::endl << "Printing CDLLI<ObjectThree>:" << std::endl;
    for (auto& value : test) {
        std::cout << value.not_val << std::endl;
    }

    return 0;
}
