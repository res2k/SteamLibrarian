#include "SteamLibrarian.hpp"

#include "facade/RegisterTypes.hpp"
#include "piping/RegisterTypes.hpp"

void QSteamLibrarianQmlPlugin::registerTypes(const char *uri)
{
  piping::RegisterTypes(uri, 1, 0);
  facade::RegisterTypes(uri, 1, 0);
}
