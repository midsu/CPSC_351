//
//  customer.h
//  bankers
//
//

#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

#include "bank.h"
#include "ext_vector.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


class Bank;

extern pthread_mutex_t mutex_;



class Customer {
public:
//  Customer() = default;
  Customer(int index, const ext_vector<int>& allocate, const ext_vector<int>& maximum,
                  Bank* bank_=nullptr)
  : idx(index), alloc(allocate), max_(maximum), need(max_ - alloc), bank(bank_) {
      srand ((unsigned int)time(NULL));
  }
  
  int get_id() const { return idx; }
  pthread_t* get_threadid() { return &thread_id; }
  
  Bank* get_bank() const { return bank; }
  ext_vector<int> get_max() const { return max_; }
  
  const ext_vector<int>& allocated() const { return alloc; }
  
  bool needs_met() const { return alloc >= max_; }
  bool needs_exceeded(const ext_vector<int>& req) const { return alloc + req > max_; }
  
  void alloc_req(  const ext_vector<int>& req) { alloc += req;  need -= req; }
  void dealloc_req(const ext_vector<int>& req) { alloc -= req;  need += req; }
  
  void release_all_resources() {
    ext_vector<int> zero(alloc.size(), 0);
    alloc = max_ = need = zero;
    std::cout << "Customer p#" << idx << " has released all resources and is shutting down\n";
  }

  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "P#" << idx << "    " << alloc << "    " << max_ << "    " << need << "\n";
    pthread_mutex_unlock(&mutex_);
  }

  friend std::ostream& operator<<(std::ostream& os, const Customer& c) {
    c.show();
    return os;
  }
  
  ext_vector<int> create_req() {
    ext_vector<int> req;
    for (size_t i = 0; i < alloc.size(); ++i) {
      if (need[i] == 0) { req.push_back(0);  continue; }
      int val = (rand() % need[i]) + 1;
      req.push_back(val);
    }
    return req;
  }
  
private:
  int idx;
  ext_vector<int> alloc;
  ext_vector<int> max_;
  ext_vector<int> need;
  pthread_t        thread_id;
  
  Bank* bank;
};

#endif /* Customer_h */
