#include "piping/App.hpp"

#include "piping/Library.hpp"

#include "vdf.hpp"

namespace piping
{
  App::App(Library* lib, const QString& acf) : QObject(lib), m_acf(acf) {}

  App::~App()
  {}

  void App::SetACF(vdf::vdf_ptree&& acfData)
  {
    m_acfData.reset (new vdf::vdf_ptree(acfData));
  }

  Library* App::library() const
  {
    return static_cast<Library*> (parent());
  }

  const QString& App::acfName() const
  {
    return m_acf;
  }

  QString App::name() const 
  {
    if (m_acfData)
    {
      boost::optional<vdf::vdf_ptree> name_child(m_acfData->get_child_optional(L"AppState.name"));
      if (name_child)
        return QString::fromStdWString(name_child->data());
    }
    return m_acf;
  }

} // namespace piping
