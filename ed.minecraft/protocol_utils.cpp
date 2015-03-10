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

protocol::packet_buf protocol::SetBigEndianInt(int size, int value)
{
  packet_buf ret;

  for (auto i = 0; i < size; i++)
  {
    ret.push_back(value & 0xFF);
    value >>= 8;
  }

  return{ ret.rbegin(), ret.rend() };
}

// Just moving bytes, according to
// https://github.com/deoxxa/libmcnet/blob/master/src/write.c#L65-L85
protocol::packet_buf protocol::ComposeFloat(float a)
{
  auto *p = (char *)&a;
  packet_buf ret = {p, p + sizeof(a)};
  return{ ret.rbegin(), ret.rend() };
}

protocol::packet_buf protocol::ComposeDouble(double a)
{
  auto *p = (char *)&a;
  packet_buf ret = { p, p + sizeof(a) };
  return{ ret.rbegin(), ret.rend() };
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
  {
    auto it = mes.begin();
    int id = ParseVarInt(it, mes.end());
    cout << "SEND " << id << " SIZE " << mes.size() << endl;
  }
  packet_buf buf = ComposeVarInt(mes.size());
  Append(buf, mes);

  //HexShow(buf);
  answers.push_back(buf);
}