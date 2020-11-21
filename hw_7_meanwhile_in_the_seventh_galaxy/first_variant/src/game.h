#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H

#include "binary.h"
#include "xor_sum.h"

template <size_t XorSumRes, size_t IdxHeap, size_t Head, size_t... Tail>
struct MoveHeapIndex;

template <bool Check, size_t XorSumRes, size_t IdxHeap, size_t Head,
          size_t... Tail>
struct GetVicHeap {
  static constexpr size_t value = -1;
  static constexpr size_t value_rocks = -1;
};

template <size_t XorSumRes, size_t IdxHeap, size_t Head, size_t... Tail>
struct GetVicHeap<0, XorSumRes, IdxHeap, Head, Tail...> {
  static constexpr size_t value =
      MoveHeapIndex<XorSumRes, IdxHeap + 1, Tail...>::value;
  static constexpr size_t value_rocks =
      MoveHeapIndex<XorSumRes, IdxHeap + 1, Tail...>::value_rocks;
};

template <size_t XorSumRes, size_t IdxHeap, size_t Head, size_t... Tail>
struct GetVicHeap<1, XorSumRes, IdxHeap, Head, Tail...> {
  static constexpr size_t value = IdxHeap;
  static constexpr size_t value_rocks = Head ^ XorSumRes;
};

template <size_t XorSumRes, size_t IdxHeap, size_t Head>
struct GetVicHeap<0, XorSumRes, IdxHeap, Head> {
  static constexpr size_t value = -1;
  static constexpr size_t value_rocks = -1;
};

template <size_t XorSumRes, size_t IdxHeap, size_t Head>
struct GetVicHeap<1, XorSumRes, IdxHeap, Head> {
  static constexpr size_t value = IdxHeap;
  static constexpr size_t value_rocks = Head ^ XorSumRes;
};

template <size_t XorSumRes, size_t IdxHeap, size_t Head, size_t... Tail>
struct MoveHeapIndex {
  static constexpr size_t value =
      GetVicHeap<((static_cast<ssize_t>((Head ^ XorSumRes) - Head) ^ -1) >= 0),
                 XorSumRes, IdxHeap, Head, Tail...>::value;

  static constexpr size_t value_rocks =
      GetVicHeap<((static_cast<ssize_t>((Head ^ XorSumRes) - Head) ^ -1) >= 0),
                 XorSumRes, IdxHeap, Head, Tail...>::value_rocks;
};

template <size_t IdxHeap, size_t Head, size_t... Tail>
struct MoveHeapIndex<0, IdxHeap, Head, Tail...> {
  static constexpr size_t value = -1;
  static constexpr size_t value_rocks = -1;
};

template <size_t XorSumRes, size_t IdxHeap, size_t Head>
struct MoveHeapIndex<XorSumRes, IdxHeap, Head> {
  static constexpr size_t value =
      GetVicHeap<((((Head ^ XorSumRes) - Head) ^ -1) >= 0), XorSumRes, IdxHeap,
                 Head>::value;
  static constexpr size_t value_rocks =
      GetVicHeap<((((Head ^ XorSumRes) - Head) ^ -1) >= 0), XorSumRes, IdxHeap,
                 Head>::value_rocks;
};

template <size_t... Heaps> struct Game {
  static constexpr size_t xor_sum = XorSum<Heaps...>::value;
  static constexpr size_t move_heap_index =
      MoveHeapIndex<xor_sum, 0, Heaps...>::value;
  static constexpr size_t move_rocks_count =
      MoveHeapIndex<xor_sum, 0, Heaps...>::value_rocks;

  static constexpr bool first_player_wins = xor_sum;
  static constexpr ssize_t first_move_heap_index = move_heap_index;
  static constexpr ssize_t first_move_rocks_count = move_rocks_count;
};

#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H.
