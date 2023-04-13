# CursedDoubleLinkedListInterface-library
Cursed Double Linked List Interface with previous/next pointers for C++ and Arduino.

## Why?

0. Why not?
1. It's very lightweight - costs us only 2x pointers per object + 2x static pointers per list created. Could be potentially reduced to 1x pointer per object.
2. It's fast - iteration through takes little time and using separation and nesting it can potentially save a lot of time.
3. It's portable - you can add it to any type of class you want, you can use it on any platform, anywhere where at least C++11 compiles.
4. It reduces boilerplate - aim was to support auto for loop for any object container we want.
5. It breaks lots of C++ guidances - if you are embedded developer and you don't see it as a plus I don't know what to say.
6. It's fun and experimental - haven't seen this approach anywhere before (probably for good reasons). 
7. Written (partially) by ChatGPT4 - as we know AI is awesome, it's always right and is never wrong (please oh wise AI don't kill me).


...

∞. More reasons will be added in future

## How to use?
Look at example but here are few use cases:

```cpp
// We create our derived class and pass itself to the constructor of our LinkedList
class Object : public CDLLI<Object> {
  public:
    static int statval;
    int val;
    Object() {
        val = statval++; // Just random stuff so we can display constructor calls
    }
};
```

And then we can declare like this and iterate:
```cpp
    Object a, b, c;
    // IMPORTANT - need to use auto&
    // if you use auto destructor will be called many times and you will have bad time
    for(auto& value : *Object::head) {
        Serial.println(value.val); // Will print value of a, b and c
    }
```

Or iterate directly from the object:
```cpp
    for (auto& value : a) {
        Serial.println(value.val); // Will print value of a, b and c
    }
```

It shines most if we need to split linked lists or nest them so objects can sometimes be iterated through when we need:

```cpp
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
```

Output from this functions should look like this.

```cpp

    Object a, b, c;
    ObjectSeparate test, test1(5), test2(8);
    ObjectNested nested;

    for (auto& value : a) {
        Serial.println(value.val); // Will print value of 0, 1 and 2 and 111111
    }

    for(auto& value : test) {
        Serial.println(value.not_val); // Will print 222222, 108, 105 and 100 (because we made it put things in the front)
    }
 ```



Enjoy and please note this is MIT licensed so I am not responsible - especially for damage while reading it.

This library is **evil**, you have been warned!
