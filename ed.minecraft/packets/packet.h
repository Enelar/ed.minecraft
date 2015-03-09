#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
namespace packets
{
  struct construct_stream
  {
    stringstream ss;

    construct_stream &AppendBigEndingInt(int size, int value);
    construct_stream &AppendVarInt(int);
    construct_stream &Append(string);

    operator vector<unsigned char>() const;
  };

  typedef int varint;
  typedef long varlong;

  struct packet
  {
    const int id;
    construct_stream BeginConstruct() const
    {
      construct_stream ret;
      ret.AppendVarInt(id);
      return ret;
    }

    packet(int _id) : id(_id) {}
  };

}
