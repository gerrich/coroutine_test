//
// daytime_client.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <future>
#include <iostream>
#include <thread>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>

using boost::asio::ip::tcp;

struct daytime_session_t {
  boost::asio::io_service& io_service_;
  std::string hostname_;

  daytime_session_t(boost::asio::io_service& io_service, const char* hostname)
    :io_service_(io_service)
    ,hostname_(hostname) {}

  void do_echo(boost::asio::yield_context yield)
  {
    try
    {
      tcp::resolver resolver(io_service_);

      tcp::resolver::iterator iter =  
        resolver.async_resolve(
            {tcp::v4(), hostname_.c_str(), "10013"},
            yield);

      tcp::socket socket(io_service_);
      socket.async_connect(*iter, yield);

      std::array<char, 1> send_buf  = {{ 0 }};
        
      socket.async_send(boost::asio::buffer(send_buf),
            yield);

      std::array<char, 128> recv_buf;
      size_t recv_length = socket.async_receive(
            boost::asio::buffer(recv_buf),
            yield);

      std::cout.write(
          recv_buf.data(),
          recv_length);
    }
    catch (std::system_error& e)
    {
      std::cerr << e.what() << std::endl;
    }
  }
  
  void go() {
    boost::asio::spawn(io_service_, boost::bind(&daytime_session_t::do_echo, this/*shared_from_this()*/, _1));
  } 
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: daytime_client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_service io_service;

    daytime_session_t session(io_service, argv[1]);
    session.go();

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
