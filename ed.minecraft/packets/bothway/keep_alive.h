#pragma once

#include "../packet.h"
namespace packets
{
  namespace bothway
  {
    struct keep_alive : packet
    {
      keep_alive() : packet(0x0)
      {}

      int rand_id;

      operator vector<unsigned char>() const
      {
        auto construct = BeginConstruct();
        return construct.AppendBigEndingInt(4, rand_id);
      }
    };
  }
}
