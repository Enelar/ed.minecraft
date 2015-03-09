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
}