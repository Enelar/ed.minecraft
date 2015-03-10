#include "protocol.h"

#include "packets\bothway\keep_alive.h"
#include "packets\2client\join_game.h"

void protocol::DoAfterLogin()
{
  {
    packets::to_client::join_game gift;
    gift.entityId = 1;
    gift.gameMode = 0; // you join as spectate
    gift.dimension = 0;
    gift.difficulty = 3;
    gift.maxPlayers = 255;
    gift.levelType = "default";

    Send(gift);
  }

  {
    packet_buf spawn_position = ComposeVarInt(0x5);
    packet_buf coordinate = SetBigEndianInt(4, 0);

    Append(spawn_position, coordinate);
    Append(spawn_position, coordinate);
    Append(spawn_position, coordinate);

    Send(spawn_position);
  }

  {
    packet_buf player_abilities = ComposeVarInt(0x39);
    Append(player_abilities, SetBigEndianInt(1, 8 + 4 + 2)); // god flying right now
    Append(player_abilities, ComposeFloat(0));
    Append(player_abilities, ComposeFloat(0));

    Send(player_abilities);
  }

  {
    packet_buf position = ComposeVarInt(0x8);
    Append(position, ComposeDouble(0));
    Append(position, ComposeDouble(0));
    Append(position, ComposeDouble(0));

    Append(position, ComposeFloat(0));
    Append(position, ComposeFloat(0));

    Append(position, SetBigEndianInt(1, true));

    Send(position);
  }
}