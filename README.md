# Hermes

**A zero-allocation NASDAQ TotalView-ITCH 5.0 decoder.**

> Hermes, messenger of the gods and patron of interpretation (the root of
> *hermeneutics*), carries messages between worlds and makes them understood.
> This library does the same for market data: raw ITCH bytes in, typed,
> validated C++ messages out.

Hermes is the **decoding layer** of a larger low-latency trading system. It
does one thing: turn the ITCH 5.0 wire format into typed messages, correctly and
fast. Order books, networking (UDP / MoldUDP64), the matching engine and the
strategy live in their own library repositories and consume Hermes as a
dependency.

> **Status:** early development (Phase 0, week 1–2). The API below is the
> target design, not yet implemented.

---

## Goals

- **Correct.** Every field decoded exactly to the ITCH 5.0 spec. Per-type
  message counts on a full NASDAQ sample day match an independent parser.
- **No undefined behavior.** Wire bytes are never `reinterpret_cast` to structs.
  All loads are explicit big-endian reads (`memcpy` / `std::bit_cast` +
  byteswap). Clean under `-fsanitize=address,undefined`.
- **Zero heap allocations** on the decode path, proven by a test with a
  counting allocator.
- **Transport-agnostic.** The decoder takes a span of bytes. It does not care
  whether those bytes came from a BinaryFILE, a raw UDP datagram or a MoldUDP64
  message block. When the transport changes, the decoder does not.
- **Decoding is separate from presentation.** Bytes → structs → (somebody
  else's) formatter. Hermes never prints on the hot path.
- **Measured.** Throughput (messages/sec) and, later, per-message latency
  percentiles are recorded in Release builds with hardware listed.

## Non-goals

These belong to other repositories in the system:

| Concern | Lives in |
|---|---|
| Order book reconstruction | order-book library |
| UDP / multicast receive, MoldUDP64 sequencing, gap recovery | feed-transport library |
| Exchange simulator / matching engine / ITCH encoding | exchange library |
| Strategy, risk, PnL | strategy library |

## ITCH 5.0 essentials

- All integers are **big-endian**; there are no floats on the wire.
- Every message begins with an 11-byte header:
  `type (1) | stock locate (2) | tracking number (2) | timestamp (6)`.
  The timestamp is nanoseconds since midnight, a 48-bit value stored in a
  `uint64_t`.
- `Price(4)` has **4 implied decimals**: $189.50 is encoded as `1,895,000`.
  Prices stay integers end to end.
- `Stock` is 8 ASCII bytes, left-justified and space-padded. It is mapped to
  the stock locate once (from the Stock Directory message); everything
  downstream keys on the locate integer.
- NASDAQ sample files use **BinaryFILE** framing: each message is prefixed by a
  2-byte big-endian length. MoldUDP64 message blocks use the same framing, so
  the framer is reused unchanged.

### Phase 0 message set

| Type | Message | Bytes |
|:---:|---|---:|
| `S` | System Event | 12 |
| `R` | Stock Directory | 39 |
| `A` | Add Order | 36 |
| `F` | Add Order with MPID | 40 |
| `E` | Order Executed | 31 |
| `C` | Order Executed with Price | 36 |
| `X` | Order Cancel | 23 |
| `D` | Order Delete | 19 |
| `U` | Order Replace | 35 |
| `P` | Trade (non-cross) | 44 |

Every other type is **length-checked and skipped, not rejected**, because real
files contain them. The remaining types are added after Phase 0.

## Design

```
 bytes (file / UDP / MoldUDP64 block)
            │
            ▼
 ┌──────────────────────┐
 │ framer               │  reads the 2-byte length, yields std::span per message,
 │                      │  detects truncation
 └──────────┬───────────┘
            ▼
 ┌──────────────────────┐
 │ decoder              │  dispatch on type byte → length check → field reads
 │  wire.hpp            │  read_be16/32/48/64, no aliasing UB
 └──────────┬───────────┘
            ▼
   typed message structs ──► handler / consumer (printer, logger, order book…)
```

- **`wire.hpp`**: portable big-endian loads, including the 48-bit timestamp.
- **`messages.hpp`**: one struct per message type. A `constexpr` table of
  wire lengths is checked with `static_assert`.
- **`decoder.hpp`**: `decode(std::span<const std::byte>) → result`. It
  validates the length and enum fields (for example, side must be `B` or
  `S`).
- **Handler interface** (Phase 1): compile-time dispatch, so adding a consumer
  requires no decoder changes and the hot path carries no virtual calls.

### Target layout

```
hermes/
  CMakeLists.txt
  include/hermes/   wire.hpp, messages.hpp, decoder.hpp, framer.hpp
  src/              decoder.cpp, framer.cpp
  app/              main.cpp, format.cpp   (printing lives here, not in the library)
  tests/            per-type golden tests, malformed-input tests, framer tests
  benchmarks/       decode throughput / latency benches
  data/             tiny hand-built fixtures (never the multi-GB sample files)
  notes/            weekly notes, assembly snippets, measurements
```

Build targets: `libhermes` (static library), `hermes_tests`, and the `hermes`
CLI.

### CLI (planned)

```
hermes dump  <file> [--limit N] [--type A]   # human-readable messages, $ prices, HH:MM:SS.nnnnnnnnn
hermes stats <file>                          # per-type counts, bytes, wall time, msgs/sec
hermes hex   <file> --offset N               # raw hex view for checking against the spec
```

## Getting the data

NASDAQ publishes free sample ITCH 5.0 trading days at
[emi.nasdaq.com/ITCH](https://emi.nasdaq.com/ITCH/). Each is several GB
uncompressed. Download one, decompress it, and keep it **outside** the repo.

Reference specs to keep open:
- NASDAQ TotalView-ITCH 5.0 specification
- MoldUDP64 specification

## Building

*CMake setup is part of the Week 4 milestone. Until then, sources compile
directly:*

```bash
clang++ -std=c++20 -Wall -Wextra -Wconversion -Wsign-conversion -Iinclude src/*.cpp -o hermes
```

## Roadmap

Hermes covers Phase 0 of the overall plan in full, then picks up the
decoder-specific work from later phases.

### Phase 0: Decoder fundamentals (8 weeks, current)

- [ ] **Week 1: Bits and bytes.** Hand-decode Add Orders from a hex dump.
      `read_be16/32/48/64` with unit tests (all-`0x00`, all-`0xFF`).
      Fixed-point price and timestamp formatters.
- [ ] **Week 2: Buffers.** Load a file into one buffer. Framer walks the
      2-byte length prefixes and yields `std::span`s. Per-type counts. Total
      bytes consumed equals file size. Clean error on a truncated final
      message.
- [ ] **Week 3: Layout and endianness.** Decode all ten Phase 0 types into
      structs with no aliasing UB. `static_assert`ed length table. Skip
      unknown types. Validate the side byte. One golden test per type.
- [ ] **Week 4: Compilation and linking.** CMake split into `libhermes`,
      tests and app. Inspect symbols with `nm -C`. Sanitizer build type.
- [ ] **Week 5: Generated code.** Find `bswap`/`movbe` for field reads.
      Compare `-O0/-O2/-O3`. Annotated assembly in `notes/`.
- [ ] **Week 6: Performance.** Zero-allocation decode loop, enforced by a
      counting-allocator test. Messages/sec for a full day at each opt level.
- [ ] **Weeks 7–8: Hardening.** Malformed-input tests. Full-day ASan/UBSan
      run. Per-type counts cross-checked against an independent parser
      (bbalouki/itch or RITCH). Fuzzing (stretch). `dump` / `stats` / `hex`
      commands.

**Phase 0 acceptance bar**

- [ ] A full sample day decodes end to end with zero sanitizer errors.
- [ ] Per-type counts match an independent parser exactly.
- [ ] Steady-state decoding makes zero heap allocations, proven by a test.
- [ ] Messages/sec measured and recorded in a Release build.
- [ ] Assembly for one field read can be walked through instruction by
      instruction.
- [ ] Decoding and printing are separate layers.

### Later phases (decoder-specific work)

- **Phase 1: Modern C++.** Typed message layer, compile-time dispatch table
  on the type byte, and a handler interface supporting multiple consumers
  without decoder changes. Remaining ITCH 5.0 message types.
- **Phase 5: CPU architecture.** Cache-aware decode. Branch and layout
  tuning, only where benchmarks prove a win.
- **Phase 6: Profiling.** Benchmark suite (Google Benchmark) reporting mean,
  p50, p95, p99, p99.9 and max per decode stage.
- **Phase 7: Memory.** Decode directly from the receive buffer into views.
  Zero allocations in steady state.
- **Phase 9–10: Integration.** Consumed unchanged by the UDP/MoldUDP64
  transport and fed by the exchange simulator's ITCH output.

## Results

*To be filled in as measurements land: messages/sec and latency percentiles,
with hardware, compiler and flags listed.*

| Build | Hardware | Messages/sec | p50 | p99 | p99.9 |
|---|---|---:|---:|---:|---:|
| — | — | — | — | — | — |

## License

Released under the [MIT License](LICENSE).
