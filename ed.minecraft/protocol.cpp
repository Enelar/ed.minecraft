#include "protocol.h"
#include <iostream>

void HexShow(const vector<unsigned char> &mes)
{
  auto ShowOne = [](unsigned char ch)
  {
    char base = ch < 10 ? '0' : ('A' - 0xA);
    std::cout << (const char)(ch + base);
  };

  int i = 0;
  for (auto ch : mes)
  {
    ShowOne(ch >> 4);
    ShowOne(ch & 0xF);
    i++;
    if (i & 3)
      std::cout << ' ';
    else
      std::cout << endl;
  }
  std::cout << std::endl;
}


int protocol::OnMessage(const vector<unsigned char> &buf)
{
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
  HexShow(buf);
  auto it = buf.begin();
  int message_id = ParseVarInt(it, buf.end());

  if (STATE == HANDSHAKE && message_id == 0)
    DoHandshake({ it, buf.end() });
}

int protocol::OnDecryptedMessage(int packet_id, const vector<unsigned char> &payload)
{
  return 0;
}