#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
namespace posix = boost::asio::posix;

struct echo_session_t : public boost::enable_shared_from_this<echo_session_t> {

  echo_session_t(boost::asio::io_service &io_service) 
    :input_(io_service, ::dup(STDIN_FILENO))
    ,output_(io_service, ::dup(STDOUT_FILENO))
  {}

  //void echo(boost::asio::ip::tcp::socket& socket, boost::asio::yield_context yield){
  void do_echo(/*posix::stream_descriptor &is, posix::stream_descriptor &os,*/ boost::asio::yield_context yield){
      char data[128];
      // read asynchronous data from socket
      // execution context will be suspended until
      // some bytes are read from socket
      size_t n = input_.async_read_some(boost::asio::buffer(data), yield);
      // write some bytes asynchronously
      boost::asio::async_write(output_, boost::asio::buffer(data, n), yield);
  }
 
  void go(boost::asio::io_service &io_service) {
    boost::asio::spawn(io_service, boost::bind(&echo_session_t::do_echo, shared_from_this(), _1));
  } 

  posix::stream_descriptor input_;
  posix::stream_descriptor output_;
};

int main()
{
  boost::asio::io_service io_service;
  echo_session_t session(io_service);
  session.go(io_service);
  
  return 0;
}
