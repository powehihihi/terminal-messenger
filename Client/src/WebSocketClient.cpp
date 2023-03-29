#include "WebSocketClient.hpp"
#include "MessagingStructure.hpp"
#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/role.hpp>
#include <boost/beast/core/stream_traits.hpp>
#include <boost/core/ignore_unused.hpp>
#include <chrono>
#include <cstddef>
#include <stdexcept>

void WebSocketClient::Run(const char * host_, const char * port_) {
  host = host_;
  resolver.async_resolve(
      host_,
      port_,
      beast::bind_front_handler(
        &WebSocketClient::on_resolve, 
        shared_from_this())
      );
}

void WebSocketClient::on_resolve(
        beast::error_code ec,
        tcp::resolver::results_type results) {
  if (ec)
    return Error(ec, "Resolve failure");
  beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));
  beast::get_lowest_layer(ws).async_connect(
      results,
      beast::bind_front_handler(
        &WebSocketClient::on_connect, 
        shared_from_this())
      );
}

void WebSocketClient::on_connect(
    beast::error_code ec,
    tcp::resolver::results_type::endpoint_type ep) {
  if (ec)
    return Error(ec, "Connect failure");
  beast::get_lowest_layer(ws).expires_never();
  ws.set_option(
      websocket::stream_base::timeout::suggested(
          beast::role_type::client
        )
      );
  ws.set_option(websocket::stream_base::decorator(
      [](websocket::request_type& req)
      {
        req.set(http::field::user_agent,
          std::string(BOOST_BEAST_VERSION_STRING) +
              " terminal-chat-websocket-client");
      }));
  host += ':' + std::to_string(ep.port());

  ws.async_handshake(host, "/",
      beast::bind_front_handler(
        &WebSocketClient::on_handshake, 
        shared_from_this())
      );
}

void WebSocketClient::on_handshake(beast::error_code ec) {
  if (ec)
    return Error(ec, "Handshake failure");

  handler();
}

void WebSocketClient::handler() {
  if (state->PopOutcome(msg)) {
    ws.async_write(net::buffer(Message::MessageToString(msg)),
        [self = shared_from_this()](
          beast::error_code ec,
          std::size_t bytes_transferred) {
            boost::ignore_unused(ec);
            boost::ignore_unused(bytes_transferred);
        });
  }
  ws.async_read(ReadBuffer, 
      [self = shared_from_this()](
        beast::error_code ec,
        std::size_t bytes_transferred) {
          if (ec==websocket::error::closed)
            return;
          if (ec)
            return self->Error(ec, "Read error");
          self->state->PushIncome(Message::StringToMessage(beast::buffers_to_string(self->ReadBuffer.data())));
          self->ReadBuffer.consume(bytes_transferred);
          self->handler();
      });
}

void WebSocketClient::Error(beast::error_code ec, char const* what) {
  state->PushError(ec, what);
}
