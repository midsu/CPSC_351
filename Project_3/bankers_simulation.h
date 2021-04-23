//
//  bankers_simulation.h
//  bankers
//
//  Created by William McCarthy on 0811//20.
//

#ifndef __bankers_simulation_h__
#define __bankers_simulation_h__


#include <iostream>
#include "ext_vector.h"

#include "customer.h"
#include "bank.h"


class BankersSimulation {
public:
  BankersSimulation() {
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<>(0, 1);
    
    ext_vector<int> alloc = { 3, 1, 5 };
    ext_vector<int> max  = { 5, 2, 6 };
    ext_vector<int> need = max - alloc;
    
    std::cout << "alloc: " << alloc << "\n";
    std::cout << "  max: " << max  << "\n";
    std::cout << " need: " << need << "\n";
    
    ext_vector<int> alloc1 = { 1, 2, 1 };
    ext_vector<int> max1 = 2 * alloc1;
    
    ext_vector<int> alloc2 = { 2, 0, 1 };
    ext_vector<int> max2 = 3 * alloc2;

    ext_vector<int> avail = alloc + alloc1 + alloc2 + ext_vector<int>({ 3, 3, 3 });

    bank = new Bank(avail);
    Customer* c = new Customer(0, alloc,  max,  bank);
    Customer* d = new Customer(1, alloc1, max1, bank);
    Customer* e = new Customer(2, alloc2, max2, bank);
    
    add(c);
    add(d);
    add(e);
  }
  BankersSimulation(Bank* bank_, ext_vector<Customer*>& customers_) : bank(bank_), customers(customers_) { }
  ~BankersSimulation() {
    for (Customer* c : customers) { delete c; }
  }
  void add(Customer* customer) {
    customers.add(customer);
    bank->add_customer(customer);
  }
  
  int random_cust_idx() {
    int idx = dis(gen) * customers.size();    // gen a random customer idx
    if (idx >= customers.size()) { std::cerr << "WARNING: idx out of range\n";  return -1; }
    return idx;
  }
  
  bool all_done() const {
    for (Customer* c : customers) {
      if (!c->needs_met()) { return false; }
    }
    return true;
  }
  
  void run() {
    std::cout << "\nBanker's algorithm simulation beginning...\n--------------------------------------------\n";
    bank->show("\n");
    while (!all_done()) {             // e.g., 11011, 01001, 01000 are not done, but 11111 is done
      int idx = random_cust_idx();
      if (idx < 0) { continue; }
      
      Customer* c = customers[idx];
      if (c->needs_met()) { continue; }
      Bank* b = c->get_bank();
      ext_vector<int> req = c->create_req();
      bool approved = b->req_approved(idx, req);
      if (approved) {
        c->alloc_req(req);
        b->withdraw_resources(req);
        b->show("\n");
        if (c->needs_met()) {  b->deposit_resources(c->get_max());  c->release_all_resources();  b->show("\n"); }
      }
    }
    std::cout << "\nBanker's algorithm simulation completed...\n\n";
  }
  
  
private:
  ext_vector< Customer*> customers;
  Bank* bank;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<> dis;     //uniform dist[0, 1]
};

#endif /* __bankers_simulation_h__ */
