#include "piping/App.hpp"

#include "piping/Library.hpp"

namespace piping
{
  App::App(Library* lib, const QString& name) : QObject(lib), m_name(name) {}

  Library* App::library() const
  {
    return static_cast<Library*> (parent());
  }

  const QString& App::name() const 
  {
    return m_name; 
  }

} // namespace piping
