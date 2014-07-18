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
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>

using boost::asio::ip::tcp;

void get_daytime(boost::asio::io_service& io_service, const char* hostname)
{
  try
  {
    tcp::resolver resolver(io_service);

    std::future<tcp::resolver::iterator> iter =
      resolver.async_resolve(
          {tcp::v4(), hostname, "10013"},
          boost::asio::use_future);

    // The async_resolve operation above returns the endpoint iterator as a
    // future value that is not retrieved ...

    tcp::socket socket(io_service);
    std::future<void> connect_future = socket.async_connect(*iter.get(), boost::asio::use_future);

    connect_future.get();
    std::array<char, 1> send_buf  = {{ 0 }};
    std::future<std::size_t> send_length =
      socket.async_send(boost::asio::buffer(send_buf),
          boost::asio::use_future);

    // Do other things here while the send completes.

    send_length.get(); // Blocks until the send is complete. Throws any errors.

    std::array<char, 128> recv_buf;
    std::future<std::size_t> recv_length =
      socket.async_receive(
          boost::asio::buffer(recv_buf),
          boost::asio::use_future);

    // Do other things here while the receive completes.

    std::cout.write(
        recv_buf.data(),
        recv_length.get()); // Blocks until receive is complete.
  }
  catch (std::system_error& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: daytime_client <host>" << std::endl;
      return 1;
    }

    // We run the io_service off in its own thread so that it operates
    // completely asynchronously with respect to the rest of the program.
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread thread([&io_service](){ io_service.run(); });

    get_daytime(io_service, argv[1]);

    io_service.stop();
    thread.join();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
