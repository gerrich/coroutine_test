CC=g++

INCLUDE_FLAGS=-I/opt/local/include
#INCLUDE_FLAGS=-I../../boost_1_55_0
BOOST_SFX=-mt
LINK_FLAGS=-L/opt/local/lib -lboost_context$(BOOST_SFX) -lboost_coroutine$(BOOST_SFX) -lboost_system$(BOOST_SFX)
#LINK_FLAGS=-L/Users/gerich/coding/boost_1_55_0/stage/lib -lboost_context -lboost_coroutine -lboost_system
ASIO_LINK_FLAGS=-lboost_system$(BOOST_SFX) -lboost_thread$(BOOST_SFX) -lboost_date_time$(BOOST_SFX) -lboost_regex$(BOOST_SFX) -lboost_serialization$(BOOST_SFX)
CXXFLAGS=$(INCLUDE_FLAGS) $(LINK_FLAGS) -g -O0

BIN=bin

all:
	mkdir -p $(BIN)
	$(CC) $(CXXFLAGS) -o $(BIN)/xrange xrange.cpp
	$(CC) $(CXXFLAGS) $(ASIO_LINK_FLAGS) -o $(BIN)/coro_test coro_test.cpp
	$(CC) $(CXXFLAGS) $(ASIO_LINK_FLAGS) -o $(BIN)/server server.cpp
#	$(CC) $(CXXFLAGS) $(ASIO_LINK_FLAGS) -o $(BIN)/echo_server echo_server.cpp

clear:
	rm -rf $(BIN)/xrange $(BIN)/coro_test $(BIN)/server
