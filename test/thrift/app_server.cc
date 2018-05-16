#include <iostream>
#include <stdexcept>
#include <sstream> 
#include <map>

#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>
#include <thrift/stdcxx.h>

#include "gen-cpp/Calculator.h"

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace tutorial;
using namespace shared;

class CalculatorHandler : public CalculatorIf {
  public:
    CalculatorHandler() {}
    
    void ping() { std::cout << "server pint() ..." << std::endl; }

    int32_t add(const int32_t n1, const int32_t n2) {
      std::cout << "server add(" << n1 << ", " << n2 << ")" << std::endl;
      return n1 + n2;
    }

    int32_t calculate(const int32_t logid, const Work& work) {
      std::cout << "server calculate(" << logid << ", " << work << ")" << std::endl;
      int32_t val;

      switch (work.op) {
        case Operation::ADD:
          val = work.num1 + work.num2;
          break;
        case Operation::SUBTRACT:
          val = work.num1 - work.num2;
          break;
        case Operation::MULTIPLY:
          val = work.num1 * work.num2;
          break;
        case Operation::DIVIDE:
          if (work.num2 == 0) {
            InvalidOperation io;
            io.whatOp = work.op;
            io.why = "Cannot divide by 0";
            throw io;
          }
          val = work.num1 / work.num2;
          break;
        default:
          InvalidOperation io;
          io.whatOp = work.op;
          io.why = "Invalid Operation";
          throw io;
      }

      SharedStruct ss;
      ss.key = logid;
      ss.value = std::to_string(val);
      log[logid] = ss;
      return val;
    } // calculate 

    void getStruct(SharedStruct& ret, const int32_t logid) {
      std::cout << "getStruct(" << logid << ")" << std::endl;
      ret = log[logid];
    }

    void zip() { std::cout << "zip()" << std::endl; }
  
  protected:
    std::map<int32_t, SharedStruct> log;
}; // CalculatorHandler 

// CalculatorCloneFactory is useful for getting access to the server
class CalculatorCloneFactory : virtual public CalculatorIfFactory {
  public:
    virtual ~CalculatorCloneFactory() {}
    virtual CalculatorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
      stdcxx::shared_ptr<TSocket> sock = stdcxx::dynamic_pointer_cast<TSocket>(connInfo.transport);
      std::cout << "Incoming connection\n";
      std::cout << "\tSocketInfo: " << sock->getSocketInfo() << "\n";
      std::cout << "\tPeerHost: " << sock->getPeerHost() << "\n";
      std::cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
      std::cout << "\tPeerPort: " << sock->getPeerPort() << "\n";
      return new CalculatorHandler;
    }
    
    virtual void releaseHandler(::shared::SharedServiceIf* handler) {
      delete handler;
    }
};

int main(int argc, char** argv) {
  
  stdcxx::shared_ptr<TServerSocket> socket(new TServerSocket(9090));
  stdcxx::shared_ptr<TBufferedTransportFactory> transport(new TBufferedTransportFactory());
  stdcxx::shared_ptr<TBinaryProtocolFactory> protocol(new TBinaryProtocolFactory());

  stdcxx::shared_ptr<CalculatorCloneFactory> calculatorClone(new CalculatorCloneFactory());
  stdcxx::shared_ptr<CalculatorProcessorFactory> processor(new CalculatorProcessorFactory(calculatorClone));
  TThreadedServer server(processor, socket, transport, protocol);

  std::cout << "Starting the server ..." << std::endl;
  server.serve();
  std::cout << "Done." << std::endl;
  return 0;
}
