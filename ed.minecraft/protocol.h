#pragma once

#include <vector>
#include <list>
#include <string>
#include <iostream>

using namespace std;
struct protocol
{
  enum
  {
    HANDSHAKE,
    HANDSHAKE_LOGIN,
    RUNTIME
  } STATE = HANDSHAKE;

  typedef vector<unsigned char> packet_buf;

  list<packet_buf> answers;

  int OnMessage(const packet_buf &);

  void OnChoppedMessage(const packet_buf &);
  int OnDecryptedMessage(int packet_id, const packet_buf &);

  int             ParseVarInt(packet_buf::const_iterator &, packet_buf::const_iterator end);
  string          ParseString(packet_buf::const_iterator &, packet_buf::const_iterator end);
  unsigned short  ParseUShort(packet_buf::const_iterator &, packet_buf::const_iterator end);

  packet_buf ComposeVarInt(int);
  packet_buf ComposeString(string);

  int GetBigEndianInt(int size, packet_buf::const_iterator &, packet_buf::const_iterator end);
  packet_buf SetBigEndianInt(int size, int value);

  packet_buf &Append(packet_buf &, const packet_buf &);

  struct more_bytes
  {
    int amount;
  };

  void DoHandshake(const packet_buf &);
  void DoHandshakeLogin(const packet_buf &);
  void DoAfterLogin();

  void Send(const packet_buf &);

};
