#include "piping/Libraries.hpp"

#include "piping/Library.hpp"

namespace piping
{
  Libraries::Libraries(const QString& steamPath, QObject* parent) : QObject(parent)
  {
    m_libraries.append(new Library(QStringLiteral("DummyLibrary1"), this));
    m_libraries.append(new Library(QStringLiteral("DummyLibrary2"), this));
  }

  int Libraries::count() const
  {
    return m_libraries.count();
  }

  Library* Libraries::get(int index) const
  {
    return m_libraries[index];
  }
} // namespace piping
