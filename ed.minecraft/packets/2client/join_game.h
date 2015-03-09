#pragma once

#include "../packet.h"
namespace packets
{
  namespace to_client
  {
    struct join_game : packet
    {
      join_game() : packet(0x1)
      {}

      int entityId;
      unsigned char gameMode;
      char dimension;
      unsigned char difficulty;
      unsigned char maxPlayers;
      string levelType;

      operator vector<unsigned char>() const;
    };
  }
}
