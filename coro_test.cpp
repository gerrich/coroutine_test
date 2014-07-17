#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <unistd.h>

namespace posix = boost::asio::posix;

struct echo_session_t : public boost::enable_shared_from_this<echo_session_t> {

  echo_session_t(boost::asio::io_service &io_service) 
    :io_service_(io_service)
    ,strand_(io_service)
    ,input_(io_service, ::dup(STDIN_FILENO))
    ,output_(io_service, ::dup(STDOUT_FILENO))
  {}

  //void do_echo(/*posix::stream_descriptor &is, posix::stream_descriptor &os,*/ boost::asio::yield_context yield){
  void do_echo(boost::asio::yield_context yield){
//    boost::asio::error::eof
      char data[128];
      // read asynchronous data from socket
      // execution context will be suspended until
      // some bytes are read from socket
      for(;;) {
      boost::system::error_code error_code;
      size_t n = input_.async_read_some(boost::asio::buffer(data), yield[error_code]);
      if (error_code == boost::asio::error::eof) break;
      // write some bytes asynchronously
      boost::asio::async_write(output_, boost::asio::buffer(data, n), yield);
      }
  }
 
  void go() {
    boost::asio::spawn(strand_, boost::bind(&echo_session_t::do_echo, this/*shared_from_this()*/, _1));
  } 

  boost::asio::io_service &io_service_;
  boost::asio::io_service::strand strand_;
  posix::stream_descriptor input_;
  posix::stream_descriptor output_;
};

int main()
{
  boost::asio::io_service io_service;
  echo_session_t session(io_service);
  session.go();
  io_service.run();
  
  return 0;
}
