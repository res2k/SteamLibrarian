#include "SteamLibrarian.hpp"

#include "piping/RegisterSingleton.hpp"

void QSteamLibrarianQmlPlugin::registerTypes(const char *uri)
{
  piping::RegisterSingleton(uri, 1, 0);
}
