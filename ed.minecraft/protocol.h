#pragma once

#include <vector>

struct protocol
{
  int OnMessage(const std::vector<unsigned char> &);
};
