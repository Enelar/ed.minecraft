#include "protocol.h"

int protocol::ParseVarInt(packet_buf::const_iterator &it, packet_buf::const_iterator end)
{
  const int step = 7;
  int result = 0, cur_step = 0;

  while (it != end)
  {
    auto ch = *it;
    bool is_last = !(ch >> step);

    int parsed = ch - (!is_last << step);
    result += parsed << (step * cur_step++);

    it++;
    if (is_last)
      return result;
  }

  throw more_bytes{ 1 };
}

protocol::packet_buf protocol::ComposeVarInt(int val)
{
  packet_buf ret;
  const int step = 7;
  const int max_step_value = (1 << step);

  while (true)
  {
    bool is_last = val < max_step_value;

    if (is_last)
    {
      ret.push_back(val);
      break;
    }

    char composed = val & (max_step_value - 1);

    val >>= step;

    ret.push_back(max_step_value + composed);
  }

  return ret;
}

string protocol::ParseString(packet_buf::const_iterator &it, packet_buf::const_iterator end)
{
  int size = ParseVarInt(it, end);
  if (end - it < size)
    throw more_bytes{ size - (end - it) };
  string ret(it, it + size);
  it += size;
  return ret;
}

protocol::packet_buf protocol::ComposeString(string str)
{
  packet_buf ret = ComposeVarInt(str.size());
  return Append(ret, { &str[0], &str[0] + str.size() });
}

unsigned short protocol::ParseUShort(packet_buf::const_iterator &it, packet_buf::const_iterator end)
{
  return (unsigned short)GetBigEndianInt(2, it, end);
}

int protocol::GetBigEndianInt(int size, packet_buf::const_iterator &it, packet_buf::const_iterator end)
{
  int ret = 0;

  if (end - it < size)
    throw more_bytes{ size - (end - it) };

  for (auto i = 0; i < size; i++)
  {
    ret <<= 8;
    ret += *it++;
  }

  return ret;
}

protocol::packet_buf &protocol::Append(protocol::packet_buf &a, const protocol::packet_buf &b)
{
  auto old_size = a.size();
  a.resize(old_size + b.size());
  copy(b.begin(), b.end(), a.begin() + old_size);
  return a;
}

void protocol::Send(const packet_buf &mes)
{
  packet_buf buf = ComposeVarInt(mes.size());
  Append(buf, mes);
  answers.push_back(buf);
}