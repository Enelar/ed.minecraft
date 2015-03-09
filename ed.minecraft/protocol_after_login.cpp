#include "protocol.h"

#include "packets\2client\join_game.h"

void protocol::DoAfterLogin()
{
  packets::to_client::join_game gift;
  gift.entityId = 1;
  gift.gameMode = 3; // you join as spectate
  gift.dimension = 0;
  gift.difficulty = 3;
  gift.maxPlayers = 255;
  gift.levelType = "default";
  gift.reducedDebugInfo = false;

  Send(gift);
}