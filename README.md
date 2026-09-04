# CursedDoubleLinkedListInterface

Intrusive doubly-linked list interface for **[Gauge.S](https://sorek.uk)** —
the open automotive gauge / data logger by **[sorek.uk](https://sorek.uk)**
([shop.sorek.uk](https://shop.sorek.uk)).

Inherit `CDLLI<YourClass>` (CRTP) and every instance links itself into a
per-type list at construction and unlinks at destruction — two pointers per
object, zero allocation, and range-based `for` over the whole list with no
container boilerplate. Written for Arduino/embedded but compiles anywhere
C++11 does. It is what [CarDataS](https://github.com/handmade0octopus/CarDataS.git)
uses to keep every sensor Holder and CAN OutputPoint iterable — including
static objects, which self-link before `setup()` runs.

## Highlights

- **Header-only, single file, no dependencies** (C++11; Arduino optional)
- **Auto-link/unlink** in constructor/destructor — O(1) insert and remove
- **Range-based `for`** plus `next()`/`previous()` with wrap-around and `swap()`
- **One independent list per type**; nest bases to put one object in several lists
- **Battle-tested** in CarDataS (Holder / OutputPoint iteration) and the
  Gauge.S firmware + WASM simulator

## Documentation

- **`AGENTS.md`** — the full technical reference: API, usage patterns, traps
  (copy semantics, empty lists, thread safety), conventions. Start there
  whether you are a human or an AI agent.
- **Gauge.S project**: [sorek.uk](https://sorek.uk) · [shop.sorek.uk](https://shop.sorek.uk)
- Repository: [github.com/handmade0octopus/CursedDoubleLinkedListInterface-library](https://github.com/handmade0octopus/CursedDoubleLinkedListInterface-library.git)
- Example: `examples/CursedExample/CursedExample.ino`

## Dependencies

- None (C++11 standard; Arduino optional)

## Author

**sorek** — [sorek.uk](https://sorek.uk) — contact@sorek.uk

## License

MIT — see [LICENSE](LICENSE). Copyright (c) sorek.uk.
