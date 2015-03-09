#pragma once

#include <vector>
#include <list>
#include <iostream>

using namespace std;
struct protocol
{
  enum
  {
    HANDSHAKE,
    RUNTIME
  } STATE = HANDSHAKE;

  typedef vector<unsigned char> packet_buf;

  list<packet_buf> answers;

  int OnMessage(const packet_buf &);

  void OnChoppedMessage(const packet_buf &);
  int OnDecryptedMessage(int packet_id, const packet_buf &);

  int ParseVarInt(packet_buf::const_iterator &, packet_buf::const_iterator end);
  string ParseString(packet_buf::const_iterator &, packet_buf::const_iterator end);
  unsigned short ParseUShort(packet_buf::const_iterator &, packet_buf::const_iterator end);

  int GetBigEndianInt(int size, packet_buf::const_iterator &, packet_buf::const_iterator end);

  struct more_bytes
  {
    int amount;
  };

  void DoHandshake(const packet_buf &);

};
