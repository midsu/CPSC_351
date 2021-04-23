////===========================================================
//// bankers_algorithm.cpp
////===========================================================
//
#include <iostream>
#include <fstream>
#include <cstring>
#include <initializer_list>

#include "ext_vector.h"
#include "bank.h"
#include "customer.h"
#include "utils.h"

//
pthread_mutex_t mutex_;  // prevents intermingled printing by threads (customers)
//
//
void run_customer_bank_tests() {
  ext_vector<int> alloc = { 3, 1, 5 };
  
  ext_vector<int> max  = { 5, 2, 6 };
  ext_vector<int> need = max - alloc;
  ext_vector<int> avail = alloc + need + ext_vector<int>({ 3, 3, 3 });

  std::cout << "alloc: " << alloc << "\n";
  std::cout << "  max: " << max  << "\n";
  std::cout << " need: " << need << "\n";

  Customer c0(0, alloc, max);
  Customer c1(1, max,   max);
  ext_vector<int> maxe = alloc + avail;
  Customer c2(2, alloc, maxe);
  Bank* pBank = new Bank(avail);

  std::cout << "\nCustomer: " << c0;
  std::cout << "needs met? " << Utils::yes_or_no(c0.needs_met()) << "\n\n";

  std::cout << "\nBank: " << *pBank << "\n";
  pBank->add_customer(&c0);
  pBank->add_customer(&c1);
  pBank->add_customer(&c2);
  std::cout << "\nBank: " << *pBank << "\n";


  ext_vector<int> req = c0.create_req();
  std::cout << "random request: " << req << "\n";
  std::cout << "Is this req available? " << Utils::yes_or_no(pBank->is_avail(req)) << "\n";
  std::cout << "     Is this req safe? " << Utils::yes_or_no(pBank->is_safe(0, req)) << "\n";

  std::cout << "Withdrawing request from bank...\n";
  pBank->withdraw_resources(req);
  std::cout << "\nBank: " << *pBank << "\n";
  std::cout << "Depositing request to bank...\n";
  pBank->deposit_resources(req);
  std::cout << "\nBank: " << *pBank << "\n";
  ext_vector<int> toomuch = { 100, 100, 100 };
  pBank->withdraw_resources(toomuch);
  std::cout << "\nBank: " << *pBank << "\n";

  c0.alloc_req(req);
  std::cout << "Customer: " << c0;

  c0.dealloc_req(req);
  std::cout << "Customer: " << c0;

  req = need;
  c0.alloc_req(need);
  std::cout << "Customer: " << c0;

  std::cout << "\nCustomer: " << c0;
  std::cout << "needs met? " << Utils::yes_or_no(c0.needs_met()) << "\n\n";
  c0.release_all_resources();
  std::cout << "Customer: " << c0 << "\n";

  std::cout << "\nBank: " << *pBank << "\n";

  c1.release_all_resources();
  std::cout << "\nBank: " << *pBank << "\n";

  delete pBank;
}
//
//
#define MAX_ITERATIONS  100           //can be removed 

void* runner(void* param) {           // thread runner
  Customer* c = (Customer*)param;

  pthread_mutex_lock(&mutex_);
  std::cout << "<<< Customer thread p#" << c->get_id() << " started... >>>\n";
  pthread_mutex_unlock(&mutex_);

  Bank* b = c->get_bank();
  
  int counter = 0;

  while (!c->needs_met()) {
    ext_vector<int> req = c->create_req();
    int idx = c->get_id();
    std::cout << "REQUEST of " << req << " for customer " << idx << " ";
    bool approved = b->req_approved(idx, req);
    std::cout << "request was approved: " << Utils::yes_or_no(approved) << "\n";

    if (approved) {
      c->alloc_req(req);
      b->withdraw_resources(req);
      b->show();

      if (c->needs_met()) {
        b->deposit_resources(c->get_max());
        c->release_all_resources();
        b->show();
      }
    }
  }
  //pthread_mutex_lock(&mutex_);
  std::cout << ">>>>>>>>>>>>>>> Customer thread p#" << c->get_id() << " shutting down... <<<<<<<<<<<<<<<<<\n\n";
  b->show();
  //pthread_mutex_unlock(&mutex_);

  pthread_exit(0);
}

void run_simulation(Bank* bank) {
  std::cout << "\nBanker's algorithm simulation beginning...\n--------------------------------------------\n";
  bank->show();

// start threads
  pthread_attr_t attr;
  pthread_mutexattr_t mutex_attr;
  pthread_attr_init(&attr);    /* get the default attributes */
  pthread_mutexattr_init(&mutex_attr);

  pthread_mutex_init(&mutex_, &mutex_attr);

  ext_vector<Customer*> customers = bank->get_customers();
  for (Customer* c : customers) {
    pthread_create(c->get_threadid(), &attr, runner, c);
  }

// join threads
  for (Customer* c : customers) {
    pthread_join(*c->get_threadid(), NULL);
  }

  std::cout << "\nBanker's algorithm simulation completed...\n\n";
}

////============================================== confirms argc > 1
void verify(int argc, const char* argv[]) {
  if (argc <= 1) {
    std::cerr << "Usage: ./bankers filename1 [filename2 filename3 ...]\n";
    exit(1);
  }
}

void process_line(char* buf, ext_vector<int>& values) {    // gets values from one line
  int i = 0;

  Utils::strstrip(buf);  // strip punctuation
  values.clear();

  char* p = strtok(buf, " ");   // split into tokens
  while (p != nullptr) {
    int val = atoi(p);
    values.push_back(val);      // convert to int, add to int array
    p = strtok(nullptr, " ");
    ++i;
  }
}

void process_file(const char* filename, Bank*& bank) {    // extracts avail for Bank, customers' alloc and max
  char buf[BUFSIZ];
  ext_vector<int> res;  // resources

  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "\n\nWarning, could not open file: '" << filename << "'\n";
    return;
  }

  std::cout << "\n\nProcessing file: '" << filename << "'...\n";
  bool finished = false;
  bool first_line = true;
  int idx = 0;
  while (!finished) {
    ifs.getline(buf, BUFSIZ, '\n');
    if (strlen(buf) == 0) { finished = true;  break; }

    process_line(buf, res);
    if (first_line) {    // first line has bank's resources
      bank = new Bank(res);
      first_line = false;
    } else {
      ext_vector<int> alloc;
      ext_vector<int> max;
      size_t size = bank->get_avail().size();
      for (size_t i = 0; i < size; ++i) {
        alloc.add(res[i]);            // e.g., for size = 2,  0, 1
        max.add(res[i + size]);       // ditto,               2, 3
      }
      Customer* c = new Customer(idx++, alloc, max, bank);
      bank->add_customer(c);
    }
  }
  ifs.close();
}

void process_files(int argc, const char* argv[], Bank*& bank) {    // processes all files in command line
  while (--argc > 0) {  // skip over program name
    const char* filename = *++argv;
    process_file(filename, bank);

    if (bank->get_customers().empty()) { std::cerr << "\t\tNo customers found... exiting...\n\n";  exit(1); }
    //else { bank->show(); }   // TODO: remove this line
    
    run_simulation(bank);
    std::cout << "\n\n\n";
  }
}



int main(int argc, const char * argv[]) {
//  ext_vector<int>::run_tests();
  
//  run_customer_bank_tests();

  Bank* bank = nullptr;

  verify(argc, argv);
  process_files(argc, argv, bank);

  run_simulation(bank);
  std::cout << "\n\n\n";

  std::cout << "\n\t\t...done.\n";
  return 0;
}


// To run it in new terminal:
// g++ bankers_algorithm.cpp -lpthread -std=c++11

//Then try your files, ex:
// ./a.out bankers_tinier.txt

 