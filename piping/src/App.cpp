#include "piping/App.hpp"

#include "piping/Library.hpp"

namespace piping
{
  App::App(Library* lib, const QString& acf) : QObject(lib), m_acf(acf) {}

  Library* App::library() const
  {
    return static_cast<Library*> (parent());
  }

  const QString& App::acfName() const
  {
    return m_acf;
  }

  const QString& App::name() const 
  {
    return m_acf; // FIXME: for now
  }

} // namespace piping
