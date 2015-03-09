#include "join_game.h"

using namespace packets::to_client;

join_game::operator vector<unsigned char>() const
{
  auto construct = BeginConstruct();
  construct.AppendBigEndingInt(4, entityId);
  construct.AppendBigEndingInt(1, gameMode);
  construct.AppendBigEndingInt(1, dimension);
  construct.AppendBigEndingInt(1, difficulty);
  construct.AppendBigEndingInt(1, maxPlayers);
  construct.Append(levelType);
  construct.AppendBigEndingInt(1, reducedDebugInfo);

  return construct;
}