#include "Version.hpp"

#include "version.h"

namespace piping
{
  Version::Version(QObject* parent) : QObject(parent) {}

  QString Version::number() const
  {
    return QStringLiteral(STMLBN_VERSION_STR);
  }
} // namespace piping
