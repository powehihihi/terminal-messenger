#ifndef WEBSOCKET_CLIENT_HPP
#define WEBSOCKET_CLIENT_HPP
#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "MessagingStructure.hpp"
#include "SharedState.hpp"
#include "ThreadSafeQueue.hpp"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebSocketClient: 
  public std::enable_shared_from_this<WebSocketClient>
{
 private:
  tcp::resolver resolver;
  websocket::stream<beast::tcp_stream> ws;
  std::shared_ptr<SharedState> state;
  // ThreadSafe::Queue<std::string> income, outcome;
  // beast::flat_buffer buffer;
  std::string host;
  beast::flat_buffer ReadBuffer;
  Message msg;                  // message from outcome queue

 public:
  // owner of WebSocketClient (WSC) object creates IOC of io_context type,
  // creates shared ptr of WSC, immediatelly calls Run on it.
  // In the end it calls IOC.run();
  explicit WebSocketClient(net::io_context& ioc, std::shared_ptr<SharedState> state):
    resolver(net::make_strand(ioc)),
    ws(net::make_strand(ioc)),
    state(state) {};

  void Run(const char * host, const char * port);   // calls async_resolve --> on_resolve

 private:
  void on_resolve(beast::error_code,
      tcp::resolver::results_type);                 // calls async_connect --> on_connect
  void on_connect(beast::error_code,
      tcp::resolver::results_type::endpoint_type);  // calls async_handshake --> on_handshake
  void on_handshake(beast::error_code);             // calls handler --> handler --> ... 

  void handler();
  void Error(beast::error_code ec, char const* what);

  /*    should be in Message class
  static std::string MessageToString(const Message& msg);
  static Message StringToMessage(const std::string& str);
  */
};



#endif
