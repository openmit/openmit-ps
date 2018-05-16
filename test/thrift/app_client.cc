#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h> 

#include "gen-cpp/Calculator.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace tutorial;
using namespace shared;

int main(int argc, char** argv) {
  std::string hostname("localhost");
  int port = 9090;
  //stdcxx::shared_ptr<TTransport> socket(new TSocket(hostname, port));
  stdcxx::shared_ptr<TSocket> socket(new TSocket(hostname, port));
  stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); 
  CalculatorClient client(protocol);

  try {
    transport->open();
  
    client.ping();
    std::cout << "ping() ..." << std::endl;

    std::cout << "1 + 1 = " << client.add(1, 1) << std::endl; 

    Work work;
    work.op = Operation::DIVIDE;
    work.num1 = 1;
    work.num2 = 0; 

    try {
      client.calculate(1, work);
    } catch (InvalidOperation& io) {
      std::cout << "InvalidOperation: " << io.why << std::endl;
    }

    work.op = Operation::SUBTRACT;
    work.num1 = 15;
    work.num2 = 10;
    int32_t diff = client.calculate(1, work);
    std::cout << "15 - 10 = " << diff << std::endl;

    SharedStruct ss;
    client.getStruct(ss, 1);
    std::cout << "Recieved log: " << ss << std::endl;

    transport->close();
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return 0;
}
