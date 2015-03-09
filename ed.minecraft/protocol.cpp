#include "protocol.h"
#include <iostream>

char HalfByteToHex(unsigned char ch)
{
  char base = ch < 10 ? '0' : ('A' - 0xA);
  return ch + base;
}

void HexShow(const vector<unsigned char> &mes)
{
  auto ShowOne = [](unsigned char ch)
  {
    std::cout << (const char)(HalfByteToHex(ch));
  };

  int i = 0;
  for (auto ch : mes)
  {
    ShowOne(ch >> 4);
    ShowOne(ch & 0xF);
    i++;
    if (i & 7)
      std::cout << ' ';
    else
      std::cout << endl;
  }
  std::cout << std::endl;
}


int protocol::OnMessage(const vector<unsigned char> &buf)
{
  HexShow(buf);
  auto it = buf.begin();
  auto size = ParseVarInt(it, buf.end());

  int readed = it - buf.begin();
  int remain = buf.size() - readed;
  int require = remain - size;

  if (require < 0)
    return require;

  OnChoppedMessage({ it, it + size });

  return readed + size;
}

void protocol::OnChoppedMessage(const packet_buf &buf)
{
  //HexShow(buf);
  auto it = buf.begin();
  int message_id = ParseVarInt(it, buf.end());

  packet_buf message = { it, buf.end() };

  switch (STATE)
  {
  case HANDSHAKE:
    if (message_id == 0)
      DoHandshake(message);
    return;
  case HANDSHAKE_LOGIN:
    if (message_id == 0)
      DoHandshakeLogin(message);
    return;
  }
}

int protocol::OnDecryptedMessage(int packet_id, const vector<unsigned char> &payload)
{
  return 0;
}