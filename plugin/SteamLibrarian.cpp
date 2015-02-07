#include "SteamLibrarian.hpp"

#include "facade/RegisterTypes.hpp"
#include "piping/RegisterSingleton.hpp"

void QSteamLibrarianQmlPlugin::registerTypes(const char *uri)
{
  piping::RegisterSingleton(uri, 1, 0);
  facade::RegisterTypes(uri, 1, 0);
}
