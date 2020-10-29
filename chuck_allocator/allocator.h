#pragma once

#include <iostream>
#include <list>

const unsigned SIZE_CHUNK = 1024;

class Chunk {
private:
  unsigned *count_chunk_instance = new unsigned(1);

  unsigned size_chunk = SIZE_CHUNK;
  char *idx_start_chunk = new char[size_chunk];
  unsigned idx_occupancy = 0;

public:
  Chunk(){};
  ~Chunk() {

    if (*count_chunk_instance == 1) {
      delete[](char *) idx_start_chunk;
      delete count_chunk_instance;
    } else
      --(*count_chunk_instance);
  };

  Chunk(const Chunk &another)
      : count_chunk_instance(another.count_chunk_instance),
        size_chunk(another.size_chunk),
        idx_start_chunk(another.idx_start_chunk),
        idx_occupancy(another.idx_occupancy) {
    ++(*count_chunk_instance);
  };

  unsigned check_free_memory() const { return size_chunk - idx_occupancy; }

  char *get_free_memory(const unsigned &num_elements,
                        const unsigned &size_type) {
    char *start_free_part = idx_start_chunk + idx_occupancy;
    idx_occupancy += num_elements * size_type;

    return start_free_part;
  }
};

template <class T> class Allocator {
private:
  std::list<Chunk> list_chunks;

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template <class U> struct rebind { typedef Allocator<U> other; };

  Allocator(){};
  ~Allocator() { list_chunks.clear(); };

  Allocator(const Allocator<T> &other) : list_chunks(other.list_chunks){};

  Allocator<T> &operator=(const Allocator<T> &other) {
    if (this == &other)
      return *this;

    this->list_chunks = other.list_chunks;

    return *this;
  };

  T *allocate(const std::size_t n) {
    if (n * sizeof(T) > SIZE_CHUNK)
      throw std::bad_alloc();
    ;

    for (auto i = list_chunks.begin(); i != list_chunks.end(); ++i) {
      if (i->check_free_memory() >= n * sizeof(T))
        return (T *)(i->get_free_memory(n, sizeof(T)));
    }

    list_chunks.push_front(Chunk());

    return (T *)(list_chunks.front().get_free_memory(n, sizeof(T)));
  };

  void deallocate(T *p, std::size_t n){};

  template <class... Args> void construct(T *p, Args &&... args) {
    new (p) T(args...);
  };

  void destroy(T *p) { p->~T(); };
};
