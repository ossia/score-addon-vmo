#pragma once
#include <score/command/Command.hpp>

namespace vmo
{
inline const CommandGroupKey& CommandFactoryName()
{
  static const CommandGroupKey key{"vmo"};
  return key;
}
}
