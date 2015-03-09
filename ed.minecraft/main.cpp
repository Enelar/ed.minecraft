#include <boost\asio.hpp>
using namespace boost::asio;
using namespace std;

struct active_connection
{
  int id;
  ip::tcp::socket sock;

};

io_service async_io, sync_io;

#include <map>
#include <memory>

auto SyncAccept(ip::tcp::acceptor *accept)
{
  shared_ptr<active_connection> ret;
  ip::tcp::endpoint endpoint;
  accept->accept(ret->sock, endpoint);

  return ret;
}

#include <future>
#include <thread>
#include <iostream>
#include <algorithm>

#include "protocol.h"

auto main()
{
  map<int, shared_ptr<active_connection>> connections;
  ip::tcp::endpoint endpoint(ip::address::from_string("0.0.0.0"), 25565);
  ip::tcp::acceptor accept(async_io, endpoint);

  future<shared_ptr<active_connection>> accepted = 
    async(launch::async, SyncAccept, &accept);

  accepted.wait();

  auto future = accepted.get();
  auto client = future.get(); // get sharedptr

  protocol proto;

  try
  {
    vector<unsigned char> buffer;
    int required = 0;
    while (true)
    {
      this_thread::sleep_for(10ms);
      auto available = client->sock.available();
      if (required > available)
        continue;

      vector<unsigned char> tempbuf(available);
      read(client->sock, boost::asio::buffer(tempbuf));

      if ((required -= available) < 0)
        required = 0;

      int oldsize = buffer.size();
      buffer.reserve(oldsize + available);
      for (auto ch : tempbuf)
        buffer.push_back(ch);

      auto res = proto.OnMessage(buffer);
      if (res < 0)
      {
        required -= res;
        continue;
      }
      vector<unsigned char> tmpswap(buffer.begin() + res, buffer.end());
      tmpswap.swap(buffer);
    }
  }
  catch (boost::system::system_error &e)
  {
    cout << "boost: " << e.what() << endl;
    return;
  }


}