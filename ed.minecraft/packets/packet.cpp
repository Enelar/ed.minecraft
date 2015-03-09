#include "packet.h"
#include "../protocol.h"

using namespace packets;

construct_stream::operator vector<unsigned char>() const
{
  auto buf = ss.str();
  return{ buf.begin(), buf.end() };
}

construct_stream &construct_stream::AppendVarInt(varint var)
{
  protocol proto;
  auto array = proto.ComposeVarInt(var);
  ss << string(array.begin(), array.end());

  return *this;
}

construct_stream &construct_stream::Append(string var)
{
  ss << (varint)var.size();
  ss << var;
  return *this;
}

construct_stream &construct_stream::AppendBigEndingInt(int size, int value)
{
  protocol proto;
  auto array = proto.SetBigEndianInt(size, value);

  ss << string(array.begin(), array.end());

  return *this;
}