#include "protocol.h"

void protocol::DoHandshake(const packet_buf &buf)
{
  auto it = buf.begin(), end = buf.end();

  {
    int proto_version = ParseVarInt(it, end);
    string server_address = ParseString(it, end);
    int port = ParseUShort(it, end);
    int next_state = ParseVarInt(it, end);

    if (next_state != 2)
      throw "we proxying only players, sorry";
    cout << "Handshake: " << proto_version << endl;
    STATE = HANDSHAKE_LOGIN;
  };
}

#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_generators.hpp>
#include <boost\uuid\uuid_io.hpp>
#include <boost\lexical_cast.hpp>

void protocol::DoHandshakeLogin(const packet_buf &buf)
{
  auto it = buf.begin(), end = buf.end();
  string nick = ParseString(it, end);
  cout << "Nick: " << nick << endl;

  // Answering kick
  {
    protocol::packet_buf ret = ComposeVarInt(0);
    Append(ret, ComposeString("{\"text\": \"Hello world\"}"));
    Send(ret);
    return;
  }

  // Answering login success
  protocol::packet_buf ret = ComposeVarInt(0x2); // Login success packet
  boost::uuids::uuid uuid = boost::uuids::random_generator()();

  { // Compose UUID
    Append(ret, ComposeVarInt(32));
    for (auto ch : uuid)
    {
      ret.push_back(HalfByteToHex(ch >> 4));
      ret.push_back(HalfByteToHex(ch & 15));
    }
    
  }
  Append(ret, ComposeString(nick)); // zero string
  Send(ret);

  STATE = RUNTIME;
  //DoAfterLogin();
}