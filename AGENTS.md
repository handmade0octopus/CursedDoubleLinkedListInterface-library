# CursedDoubleLinkedListInterface (CDLLI) — Technical Reference (Agents & Developers)

Single consolidated technical doc for the CursedDoubleLinkedListInterface
library: API, usage patterns, traps, and conventions. For the project/owner
front page see `README.md`. **Keep it in sync with the code** — the whole
library is one 125-line header, so there is no excuse for drift.

## What This Library Does

CDLLI is an intrusive, header-only, doubly-linked list **interface** for C++11
and Arduino. Your class inherits `CDLLI<YourClass>` (CRTP) and every instance
auto-links into a per-type global list on construction and unlinks on
destruction. No containers, no allocation, no boilerplate — range-based `for`
loops iterate the list directly.

**It does NOT**: own or copy objects, manage memory, or provide thread safety.

## Why "Cursed"

- Objects link **themselves** at construction — including static/global
  instances, which link before `setup()`/`main()` runs.
- `begin()` called on **any** instance starts at the static list `head`, not at
  that instance — there is no "iterate from here" range-for.
- It deliberately breaks C++ style guidance (intrusive links, static state in a
  template, casts through the base). It is also tiny, fast, and zero-allocation.

## Features

- CRTP template — works on any class; stack, heap, or static allocation
- Auto-link on construction (append at tail by default, prepend with `toFront=true`)
- Auto-unlink on destruction (virtual destructor fixes neighbor links + head/tail)
- Range-based `for` via inner `Iterator` (dereferences to `T&`)
- `next()`/`previous()` pointer walking with optional wrap-around
- `swap()` two nodes' list positions in place
- One independent list per instantiated type `T` (static `head`/`tail` per `T`)
- Nesting: inherit several `CDLLI` bases to place one object in several lists

## Cost

- 2 link pointers per object (`_next`, `_prev`) + a vtable pointer (the
  destructor is virtual) + 2 static pointers per list type (`head`, `tail`).
- O(1) insert and remove. No dynamic allocation anywhere.

## Complete API

The entire library is `CursedDoubleLinkedListInterface.hpp`:

```cpp
template <typename T>
class CDLLI {
    static CDLLI<T>* head;              // list head (nullptr when empty)
    static CDLLI<T>* tail;              // list tail

    explicit CDLLI(bool toFront = false);   // auto-link (tail / head)
    virtual ~CDLLI();                       // auto-unlink

    T* next(bool wrap = false);         // nullptr at tail; wrap -> head
    T* previous(bool wrap = false);     // nullptr at head; wrap -> tail

    static T& getHead();                // first object (UB when list empty!)
    static T& getTail();                // last object  (UB when list empty!)

    Iterator begin();                   // ALWAYS at static head
    Iterator end();                     // nullptr

    void swap(CDLLI<T>& other);         // exchange list positions
};
```

`head`/`tail` are `CDLLI<T>*` — cast to `T*` (e.g. `(Sensor*)Sensor::head`)
when you need a typed pointer.

## Usage

### Basic

```cpp
#include <CursedDoubleLinkedListInterface.hpp>

class Sensor : public CDLLI<Sensor> {
public:
    int value;
    Sensor(int v) : value(v) {}    // links on construction
};

Sensor a(1), b(2), c(3);           // list order: a, b, c (construction order)

// Range-for: guard the head, and ALWAYS use auto& (see traps)
if (Sensor::head) {
    for (auto& s : *Sensor::head) {
        Serial.println(s.value);   // 1, 2, 3
    }
}

// Any instance works as the range object too — iteration still
// starts at the static head:
for (auto& s : a) { /* same full list */ }
```

### Pointer walk (start anywhere, wrap-around round-robin)

```cpp
for (Sensor* s = (Sensor*)Sensor::head; s; s = s->next()) { /* ... */ }

Sensor* n = a.next(true);      // at tail wraps to head
Sensor* p = a.previous(true);  // at head wraps to tail
```

### Prepend instead of append

```cpp
class Msg : public CDLLI<Msg> {
public:
    Msg() : CDLLI(true) {}     // insert at head
};
```

### Nesting — one object in several independent lists

```cpp
class A : public CDLLI<A> {};
class B : public CDLLI<B> {};
class AB : public A, public B {};   // appears in BOTH lists

// disambiguate base accessors when needed: obj.A::next(), obj.B::next()
```

## Traps (read before use)

1. **`auto` vs `auto&`** — copying a linked object copies its `_next`/`_prev`
   pointers but does NOT relink; when the copy is destroyed its destructor
   unlinks through the copied pointers and **corrupts the original list**.
   Always iterate with `auto&`; treat CDLLI-derived objects as non-copyable
   (delete the copy ctor in your own class if you can).
2. **Empty list** — `getHead()`/`getTail()` dereference the static pointer:
   undefined behavior when the list is empty. Guard first:
   `if (Sensor::head) { ... }` (same guard covers `*Sensor::head` range-for).
3. **`begin()` ignores `this`** — range-for from any object starts at the list
   head. For "iterate from here", use the pointer walk (`next()`), not a
   range-for.
4. **List order = construction order** (tail append). For static/global objects
   that is translation-unit static-init order — do not rely on cross-TU ordering.
5. **No thread safety** — the application must serialize
   construction/destruction/iteration. CarDataS wraps entry creation in a
   `portMUX_TYPE` spinlock for this reason.
6. **Destroying the last object** resets `head`/`tail` to `nullptr` — an empty
   list is a valid state; all guards must keep holding.

## Used in Gauge.S

- `lib/CarDataS/` — `Holder : public CDLLI<Holder>` (every sensor entry lives in
  one iterable list, cds_holder.hpp) and `OutputPoint : public CDLLI<OutputPoint>`
  (CAN TX points, cds_output.hpp). Guarded iteration pattern from CarDataS:

  ```cpp
  CDS::Holder* head = (CDS::Holder*)CDS::Holder::head;
  if (head) {
      for (auto& h : *head) { /* h is CDS::Holder& */ }
  }
  ```

- Gauge.S firmware built-in Holders are static objects that self-link before
  `setup()` runs, then get registered in the name map at boot.

## Conventions

- Header-only, single file at the library ROOT (no `src/` dir);
  `library.json` `srcFilter` limits the build to that one header.
- No dependencies beyond C++11; Arduino not required.
- Version lives in `library.json` (1.0.4) — keep `library.properties` in sync.
- Demo: `examples/CursedExample/CursedExample.ino` (builds on desktop
  compilers too via its `FOR_CPP` define).

## File Structure

```
lib/CursedDoubleLinkedListInterface-library/
  CursedDoubleLinkedListInterface.hpp        -- the entire library (header-only)
  examples/CursedExample/CursedExample.ino   -- link/unlink/nesting demo
  library.json                               -- PlatformIO manifest (v1.0.4)
  library.properties                         -- Arduino IDE manifest
  keywords.txt                               -- Arduino IDE syntax highlighting
  LICENSE                                    -- MIT
```

## License

MIT — see [LICENSE](LICENSE).
