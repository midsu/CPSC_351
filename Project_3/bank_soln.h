//
//  bank.h
//
//

#ifndef __BANK_H__
#define __BANK_H__

#include "customer.h"


class Bank {
public:
  Bank() = default;
  Bank(const ext_vector<int>& available) : avail(available), customers() { }
  
  ext_vector<int> get_avail() const { return avail; }
  bool is_avail(const ext_vector<int>& req) const { return req < avail; }
  
  bool is_safe(int id, const ext_vector<int>& req) {   // TODO:  implement check for request not leading to deadlock
    // grant request (temporarily) to customer
    // go through list of customers, are there enough resources for at least one customer to get to its max?
    // roll back request
    // return anaswer;
    
    return true;   // TODO: calculate if it is really safe
  }

  bool req_approved(int id, const ext_vector<int>& req) {
    if (req > avail) { return false; }

    Customer* c = customers[id];
    if (c->needs_exceeded(req)) { return false; }
    
    return is_safe(id, req);
  }
  
  void add_customer(Customer* c) {
    customers.push_back(c);
    avail -= c->allocated();
  }
  
  void withdraw_resources(const ext_vector<int>& req) {
    if (!is_avail(req)) {
      pthread_mutex_lock(&mutex_);
      std::cerr << "WARNING: req: " << req << " is not available for withdrawing\n";
      pthread_mutex_unlock(&mutex_);
      return;
    }
    if (is_avail(req)) { avail -= req; }
  }
  void deposit_resources(const ext_vector<int>& req) { avail += req; }


  ext_vector<Customer*> get_customers() const { return customers; }
  
  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "avail: [" << avail << "]\n";
    pthread_mutex_unlock(&mutex_);
    
    for (Customer* c : customers) {
      c->show();
    }
    std::cout << "\n";
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Bank& be) {
    be.show();
    return os;
  }

private:
  ext_vector<int> avail;
  ext_vector<Customer*> customers;
};

#endif /* Bank_h */
