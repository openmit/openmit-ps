#include "core/app-template.hh"
#include "core/reactor.hh"
#include "core/sleep.hh"
#include <iostream>
#include <stdexcept>

using namespace seastar;

// future and then
future<> f() {
  std::cout << "Hello SeaStar!!! thread number: " << smp::count << "\n";
  std::cout << "Sleeping .... " << std::flush;
  using namespace std::chrono_literals;
  sleep(500ms).then([] { std::cout << "500ms " << std::flush; });
  sleep(200ms).then([] { std::cout << "200ms " << std::flush; });
  sleep(100ms).then([] { std::cout << "100ms " << std::flush; });
  return sleep(1s).then([] { std::cout << "Done.\n"; });
}

// future<type>
future<int> slow() {
  using namespace std::chrono_literals;
  return sleep(1s).then([] { std::cout << "1s done.\n"; return 3; });
}

future<int> fast() {
  return make_ready_future<int>(3);
}

future<> f_type() {
  //return slow().then([] (int val) {
  return fast().then([] (int val) {
    std::cout << "Got " << val << std::endl;
  });
}

// Capture state in continuations
future<int> incr(int i) {
  using namespace std::chrono_literals;
  //return sleep(100ms).then([&i] { return i + 1; });   // &i reference is wrong. address not value
  return sleep(100ms).then([&i] { return i + 1; });
}

future<> f_state() {
  return incr(3).then([](int val) {
    std::cout << "Capture state. val: " << val << std::endl;
  });
}

// Obj's move-assignment 
int do_something(std::unique_ptr<T> obj) {
  // do some computation based on the contents of obje. 
  return 17;
}

future<int> slow_do_something(std::unique_ptr<T> obj) {
  using namespace std::chrono_literals;
  return sleep(100ms).then([obj = std::move(obj)] {
    return do_something(std::move(obj));
  });
}

int main(int argc, char** argv) {
  app_template app;
  try {
    //app.run(argc, argv, f);
    //app.run(argc, argv, f_type);
    app.run(argc, argv, f_state);
  } catch(std::runtime_error& e) {
    std::cerr << "Could not start application: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
