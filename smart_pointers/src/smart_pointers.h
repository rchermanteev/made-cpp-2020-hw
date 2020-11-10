#pragma once

namespace task {

template <class T> class UniquePtr {
public:
  using pointer = T *;
  using element_type = T;

  UniquePtr(pointer p) : ptr(p){};

  UniquePtr(UniquePtr &&another) : ptr(another.ptr) { another.ptr = nullptr; };

  ~UniquePtr() { delete ptr; };

  UniquePtr &operator=(UniquePtr &&another) {
    this->ptr = another.ptr;
    another.ptr = nullptr;

    return *this;
  };

  pointer get() const noexcept { return ptr; };
  pointer operator->() const noexcept { return ptr; }
  element_type &operator*() const { return *ptr; };

  void swap(UniquePtr &r) {
    T *buff = this->ptr;

    this->ptr = r.ptr;
    r.ptr = buff;
  }

  void reset() { this->ptr = nullptr; }

  template <class U> void reset(U *new_ptr) { this->ptr = new_ptr; }

  void release() { this->ptr = nullptr; }

private:
  pointer ptr;
};

template <class T> struct ControlBlock {
  T *ptr;
  size_t ref_shared_cnt = 0;
  size_t ref_weak_cnt = 0;
};

template <class T> class SharedPtr {
public:
  ControlBlock<T> *cb;

  SharedPtr(T *p = nullptr) {
    cb = new ControlBlock<T>;
    cb->ptr = p;
    cb->ref_shared_cnt = 1;
  };

  SharedPtr(ControlBlock<T> *another_cb) : cb(another_cb) {
    cb->ref_shared_cnt += 1;
  };

  SharedPtr(const SharedPtr<T> &another) : cb(another.cb) {
    ++cb->ref_shared_cnt;
  }

  SharedPtr(SharedPtr<T> &&another) : cb(another.cb) {
    ControlBlock<T> *temp_cb = new ControlBlock<T>;
    temp_cb->ptr = nullptr;

    another.cb = temp_cb;
  };

  SharedPtr<T> &operator=(SharedPtr<T> &&another) {
    --cb->ref_shared_cnt;
    cb = another.cb;

    ControlBlock<T> *temp_cb = new ControlBlock<T>;
    temp_cb->ptr = nullptr;
    another.cb = temp_cb;
    ++cb->ref_shared_cnt;

    return *this;
  };

  SharedPtr<T> &operator=(const SharedPtr<T> &another) {
    --cb->ref_shared_cnt;
    cb = another.cb;
    ++cb->ref_shared_cnt;

    return *this;
  };

  ~SharedPtr() {
    if (cb->ref_shared_cnt == 1) {
      delete cb->ptr;
      cb->ref_shared_cnt -= 1;
    } else
      --cb->ref_shared_cnt;

    if (cb->ref_shared_cnt == 0 && cb->ref_weak_cnt == 0)
      delete cb;
  }

  T *get() const { return cb->ptr; }
  T *operator->() const { return cb->ptr; }
  T &operator*() const { return *(cb->ptr); }

  unsigned use_count() const { return cb->ref_shared_cnt; }

  void swap(SharedPtr<T> &r) {
    ControlBlock<T> *buff = cb;

    cb = r.cb;
    r.cb = buff;
  }

  void reset() {

    cb->ref_weak_cnt -= 1;

    ControlBlock<T> *temp_cb = new ControlBlock<T>;
    temp_cb->ptr = nullptr;
    temp_cb->ref_shared_cnt = 1;

    cb = temp_cb;
  }

  template <class U> void reset(U *new_ptr) {
    cb->ref_weak_cnt -= 1;

    ControlBlock<U> *temp_cb = new ControlBlock<U>;
    temp_cb->ptr = new_ptr;
    temp_cb->ref_shared_cnt = 1;

    cb = temp_cb;
  }
};

template <class T> class WeakPtr {
private:
  ControlBlock<T> *cb;

public:
  WeakPtr() {
    cb = new ControlBlock<T>;
    cb->ptr = nullptr;
    cb->ref_weak_cnt += 1;
  }

  WeakPtr(const WeakPtr<T> &r) {
    cb = r.cb;
    cb->ref_weak_cnt += 1;
  }

  WeakPtr(WeakPtr<T> &&r) : cb(r.cb) {
    ControlBlock<T> *temp_cb = new ControlBlock<T>;
    temp_cb->ptr = nullptr;

    r.cb = temp_cb;
  }

  WeakPtr(const SharedPtr<T> &r) {
    cb = r.cb;
    cb->ref_weak_cnt += 1;
  }

  WeakPtr<T> &operator=(const WeakPtr<T> &r) {
    --cb->ref_weak_cnt;
    cb = r.cb;
    ++cb->ref_weak_cnt;

    return *this;
  }

  WeakPtr<T> &operator=(WeakPtr<T> &&r) {
    --cb->ref_weak_cnt;
    cb = reset.cb;

    ControlBlock<T> *temp_cb = new ControlBlock<T>;
    temp_cb->ptr = nullptr;
    r.cb = temp_cb;
    ++cb->ref_weak_cnt;

    return *this;
  }

  template <class U> WeakPtr<U> &operator=(const SharedPtr<U> &r) {
    --cb->ref_weak_cnt;
    cb = r.cb;
    ++cb->ref_weak_cnt;

    return *this;
  }

  ~WeakPtr() { --cb->ref_weak_cnt; };

  long use_count() const { return cb->ref_shared_cnt; }

  bool expired() const { return true ? cb->ref_shared_cnt == 0 : false; }

  SharedPtr<T> lock() const { return SharedPtr<T>(cb); }

  void swap(WeakPtr<T> &r) {
    ControlBlock<T> *buff = cb;

    cb = r.cb;
    r.cb = buff;
  }

  void reset() {
    cb->ref_weak_cnt -= 1;

    ControlBlock<T> *temp_cb = new ControlBlock<T>;
    temp_cb->ptr = nullptr;
    temp_cb->ref_shared_cnt = 1;

    cb = temp_cb;
  }
};

// Your template function definitions may go here...

} // namespace task

// #include "smart_pointers.tpp"
