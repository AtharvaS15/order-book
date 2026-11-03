# Electronic Exchange Simulator — Limit Order Matching

A command-line C++ simulation of a simplified trading venue's order book. It accepts user-entered buy and sell orders and distinguishes market orders from limit orders.

## Implemented features

- Market and limit buy/sell order entry
- Ordered price levels stored in a C++ `std::map`
- FIFO handling for resting orders at the same price level
- Partial fills and quantity updates as orders execute
- Removal of empty price levels and reporting of unfilled quantity

## Implementation

The simulation stores separate buy and sell quantity queues at each price level. Buy orders attempt to match resting sells; sell orders match resting buys. Remaining quantity for unmatched limit orders is added to the book. The project is intentionally a learning-focused CLI simulator, not a production exchange engine.

## Build and run

Requires a C++20-compatible compiler because the code uses `std::ranges::reverse_view`.

```bash
g++ -std=c++20 -O2 -o exchange main.cpp
./exchange
```
