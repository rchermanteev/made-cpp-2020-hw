#pragma once
#include <iostream>
#include <iterator>
#include <string>

namespace task {

template <class T> class Node {
public:
  Node(){};
  Node(T value) : val(value){};
  Node(T &&value, Node<T> *next, Node<T> *prev)
      : val(std::move(value)), next(next), prev(prev){};
  Node(const Node<T> &other)
      : val(other.val), next(other.next), prev(other.prev){};
  ~Node(){};

  Node<T> *next;
  Node<T> *prev;
  T val;
};

template <class T, class Alloc = std::allocator<T>> class list {
private:
  typedef std::allocator_traits<Alloc> _traits;

public:
  typedef T value_type;
  typedef Alloc allocator_type;
  typedef typename _traits::size_type size_type;
  typedef typename _traits::difference_type difference_type;
  typedef typename _traits::pointer pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename _traits::const_pointer const_pointer;
  using node_type = Node<T>;
  using node_pointer = Node<T> *;

public:
  class iterator {
  public:
    friend class list;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;
    using node_type = Node<T>;
    using node_pointer = Node<T> *;

  private:
    node_pointer node;

  public:
    iterator() : node(0){};

    iterator(node_pointer node) : node(node){};

    iterator(const iterator &it) : node(it.node){};

    iterator &operator=(const iterator &it) {
      node = it.node;
      return *this;
    };
    iterator &operator++() {
      node = node->next;
      return *this;
    }
    iterator operator++(int) {
      iterator t(*this);
      t = t->next;
      return t;
    };
    reference operator*() const { return node->val; };
    pointer operator->() const { return &(node->val); };
    iterator &operator--() {
      node = node.prev;
      return *this;
    };
    iterator operator--(int) {
      iterator t(*this);
      t = t.prev;
      return t;
    };
    bool operator==(iterator other) const { return (node == other.node); };
    bool operator!=(iterator other) const { return !(*this == other); };
  };

  class const_iterator {
  public:
    friend class list;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using const_pointer = const T *;
    using const_reference = const T &;
    using iterator_category = std::bidirectional_iterator_tag;
    using const_node_type = const Node<T>;
    using const_node_pointer = const Node<T> *;

  private:
    const_node_pointer node;

  public:
    const_iterator() : node(0){};
    const_iterator(const_node_pointer node) : node(node){};
    const_iterator(const_iterator &it) : node(it.node){};

    const_iterator &operator=(const_iterator &it) {
      node = it.node;
      return *this;
    };
    const_iterator &operator++() {
      node = node->next;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator t(*this);
      t = t->next;
      return t;
    };
    const_reference operator*() const { return node->val; };
    const_pointer operator->() const { return &(node->val); };
    const_iterator &operator--() {
      node = node.prev;
      return *this;
    };
    const_iterator operator--(int) {
      const_iterator t(*this);
      t = t.prev;
      return t;
    };
    bool operator==(const_iterator other) const {
      return (node == other.node);
    };
    bool operator!=(const_iterator other) const { return !(*this == other); };
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static_assert(std::is_same<typename Alloc::value_type, value_type>::value,
                "Allocator::value_type must be same type as value_type");

private:
  allocator_type _allocator;

  std::allocator<node_type> allocator_for_node;

  node_pointer _head;
  node_pointer _tail;

  size_t _size = 0;

  iterator head_iterator;
  iterator tail_iterator;

public:
  list() {
    _head = _tail = allocator_for_node.allocate(1);
    _tail = nullptr;
    _head = nullptr;

    head_iterator = iterator(_head);
    tail_iterator = iterator(_tail);
  };

  explicit list(const Alloc &alloc){};

  list(size_t count, const T &value, const Alloc &alloc = Alloc());

  list(size_t count, const Alloc &alloc = Alloc()) {
    while (this->size() < count)
      this->push_back();
  }

  ~list() {
    if (_size) {
      node_pointer tmp;
      while (_head) {
        tmp = _head;
        _head = _head->next;
        allocator_for_node.deallocate(tmp, 1);
      }
    }
  };

  list(const list &other);

  list(list &&other);

  list &operator=(const list &other);
  list &operator=(list &&other);

  Alloc get_allocator() const;

  T &front() {
    if (_head == nullptr)
      throw std::runtime_error("Invalid Action!");

    return _head->val;
  };

  const T &front() const {
    if (_head == nullptr)
      throw std::runtime_error("Invalid Action!");

    return _head->val;
  }

  T &back() {
    if (_tail == nullptr)
      throw std::runtime_error("Invalid Action!");

    return _tail->val;
  };

  const T &back() const {
    if (_tail == nullptr)
      throw std::runtime_error("Invalid Action!");

    return _tail->val;
  };

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  bool empty() const { return (_size == 0); };

  size_t size() const { return _size; };
  size_t max_size() const { return _size; };

  void clear() {
    if (_size) {
      node_pointer tmp;
      while (_head) {
        tmp = _head;
        _head = _head->next;
        allocator_for_node.deallocate(tmp, 1);
      }
    }
    _head = _tail = nullptr;
    _size = 0;
  };

  iterator insert(const_iterator pos, const T &value);
  iterator insert(const_iterator pos, T &&value);
  iterator insert(const_iterator pos, size_t count, const T &value);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void push_back(const T &value) {
    node_pointer new_node = allocator_for_node.allocate(1);
    new_node->next = nullptr;
    new_node->prev = _tail;
    new_node->val = value;

    if (_head == nullptr)
      _head = new_node;

    if (_tail != nullptr)
      _tail->next = new_node;

    _tail = new_node;
    ++_size;
  };

  void push_back(T &&value = T()) {
    node_pointer new_node = allocator_for_node.allocate(1);
    allocator_for_node.construct(new_node, std::move(value), nullptr, _tail);

    if (_head == nullptr)
      _head = new_node;

    if (_tail != nullptr)
      _tail->next = new_node;

    _tail = new_node;
    ++_size;
  };

  void pop_back() {
    if (_tail == nullptr)
      throw std::runtime_error("Invalid Action");

    node_pointer tmp = _tail;

    _tail = _tail->prev;
    if (_tail != nullptr)
      _tail->next = nullptr;

    --_size;

    allocator_for_node.deallocate(tmp, 1);
  };

  void push_front(const T &value) {
    node_pointer new_node = allocator_for_node.allocate(1);
    new_node->next = _head;
    new_node->prev = nullptr;
    new_node->val = value;

    if (_tail == nullptr)
      _tail = new_node;

    if (_head != nullptr)
      _head->prev = new_node;

    _head = new_node;
    ++_size;
  };

  void push_front(T &&value = T()) {
    node_pointer new_node = allocator_for_node.allocate(1);
    allocator_for_node.construct(new_node, std::move(value), _head, nullptr);

    if (_tail == nullptr)
      _tail = new_node;

    if (_head != nullptr)
      _head->prev = new_node;

    _head = new_node;
    ++_size;
  }

  void pop_front() {
    if (_head == nullptr)
      throw std::runtime_error("Invalid Action");

    node_pointer tmp = _head;

    _head = _head->next;
    if (_head != nullptr)
      _head->prev = nullptr;

    --_size;
    allocator_for_node.deallocate(tmp, 1);
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args);

  template <class... Args> void emplace_back(Args &&... args);

  template <class... Args> void emplace_front(Args &&... args);

  void resize(size_t count) {
    if (count > _size) {
      while (_size != count)
        this->push_back();
    } else if (count < _size) {
      while (_size != count)
        this->pop_back();
    }
  };

  void swap(list &other);

  void merge(list &other);
  void splice(const_iterator pos, list &other);
  void remove(const T &value);
  void reverse();
  void unique();
  void sort();

  // Your code goes here?..
};

} // namespace task
