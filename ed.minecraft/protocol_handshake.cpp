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

void protocol::DoHandshakeLogin(const packet_buf &buf)
{
  auto it = buf.begin(), end = buf.end();
  string nick = ParseString(it, end);
  cout << "Nick: " << nick << endl;

  // Answering login success

}