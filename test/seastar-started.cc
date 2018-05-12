#include "core/app-template.hh"
#include "core/reactor.hh"
#include <iostream>

using namespace seastar;

int main(int argc, char** argv) {
  app_template app;
  app.run(argc, argv, [] {
    std::cout << "Hello SeaStar!!! thread number: " << smp::count << "\n";
    return make_ready_future<>();
  });
  return 0;
}
