//
//  extended_vector.h
//  bankers
//

#ifndef __ext_vector_h__
#define __ext_vector_h__

#include <iostream>
#include <iomanip>
#include <vector>


template <typename T>
class ext_vector {
public:
  ext_vector() : v(std::vector<T>()) { }
  ext_vector(size_t size, const T& k) : ext_vector() {
    for (size_t i = 0; i < size; ++i) { v.push_back(k); }
  }
  ext_vector(const std::vector<T>& other) : v(other) { }
  ext_vector(const std::string& name, const std::initializer_list<T>& li) : ext_vector<T>() {
    for (const T& el : li) {
      v.push_back(el);
    }
  }
  ext_vector(const std::initializer_list<T>& li) : ext_vector<T>() {
    for (const T& el : li) {
      v.push_back(el);
    }
  }
//------------------------------------------------------------------
  bool empty()  const { return v.empty(); }
  size_t size() const { return v.size(); }
//------------------------------------------------------------------
  void add(T& val) { v.push_back(val); }
  void push_back(T& val) { v.push_back(val); }
  void clear() {
    while (!empty()) { v.pop_back(); }
  }
//------------------------------------------------------------------
  T  operator[](size_t i) const { return v[i]; }
  T& operator[](size_t i)       { return v[i]; }
  
  typename std::vector<T>::const_iterator begin() const { return v.begin(); }
  typename std::vector<T>::const_iterator end()   const { return v.end();   }
//------------------------------------------------------------------
  ext_vector& operator+=(const ext_vector<T>& other) {    // alloc += req;
    for (int i = 0; i < other.v.size(); ++i) {
      v[i] += other.v[i];
    }
    return *this;
  }
  ext_vector& operator-=(const ext_vector<T>& other) {    // avail -= req;
    for (int i = 0; i < other.v.size(); ++i) {
      v[i] -= other.v[i];
    }
    return *this;
  }
//------------------------------------------------------------------
  ext_vector operator+(const ext_vector<T>& other) const {   // max + need == alloc;
    ext_vector<T> temp(v);
    temp += other;
    return temp;
  }
  ext_vector operator-(const ext_vector<T>& other) const {   // need = max - alloc;
    ext_vector<T> temp(v);
    temp -= other;
    return temp;
  }
  ext_vector operator*(const T& k) const {
    ext_vector<T> temp(v);
    for (int i = 0; i < v.size(); ++i) { temp.v[i] *= k; }            // returns v * k
    return temp;
  }
  friend ext_vector operator*(const T& k, const ext_vector<T>& v) {
    ext_vector<T> temp(v);
    for (int i = 0; i < v.size(); ++i) { temp.v[i] *= k; }            // returns v * k
    return temp;
  }
//------------------------------------------------------------------
  bool operator< (const ext_vector<T>& other) const { return v < other.v;  }     // alloc < max;
  bool operator<=(const ext_vector<T>& other) const { return v <= other.v; }     // alloc + req <= max;   // should occur
  bool operator> (const ext_vector<T>& other) const { return v > other.v;  }     // alloc + req > max;    // should not occur
  bool operator>=(const ext_vector<T>& other) const { return v >= other.v; }

  bool operator==(const ext_vector<T>& other) const { return v == other.v; }     // alloc == max;
  bool operator!=(const ext_vector<T>& other)       { return !operator==(other); }
  bool is_zero() const {
    for (size_t i = 0; i < v.size(); ++i) {
      if (v[i] != 0) { return false; }
    }
    return true;
  }
  bool is_k(const T& constant) {
    for (size_t i = 0; i < v.size(); ++i) {
      if (v[i] != constant) { return false; }
    }
    return true;
  }
//------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& os, const ext_vector& vect) {
    if (vect.empty()) { return os << "ext_vector<T> is empty]\n"; }
    size_t i = 0;
    for (const T& el : vect.v) {
      os << std::setw(2) << el;
      if (i++ < vect.v.size() - 1) { os << " "; }
    }
    return os;
  }
//------------------------------------------------------------------
  static void run_tests() {
    ext_vector v = { 1, 2, 3, 4, 5 };
    ext_vector v2 = { 1, 0, 2, 0, 3 };
    std::cout << "v  is: " << v << "\n";
    std::cout << "v2 is: " << v2 << "\n";

    v += v2;
    std::cout << "v += v2 is: " << v << "\n";
    std::cout << "v +  v2 is: " << v + v2 << "\n";
    std::cout << "v -  v2 is: " << v - v2 << "\n";
    std::cout << "v -  v  is: " << v - v << "\n";

    std::cout << "3 * v is: " << 3 * v << "\n";
    std::cout << "v * 2 is: " << v * 2 << "\n";
  }

private:
  std::vector<T> v;
};


#endif /* __ext_vector_h__ */
