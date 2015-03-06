#include "piping/Library.hpp"

#include "piping/App.hpp"

namespace piping
{
  Library::Library(const QString& path, QObject* parent) : QObject(parent), m_path(path)
  {
    static int count = 0;
    for (int i = 0; i < 2; i++)
    {
      m_apps.append(new App(this, QString(QStringLiteral("Dummy App %1")).arg(++count)));
    }
  }

  const QString& Library::path() const { return m_path;  }

  int Library::count() const { return m_apps.count(); }
  
  App* Library::get(int index) const { return m_apps[index]; }
} // namespace piping
