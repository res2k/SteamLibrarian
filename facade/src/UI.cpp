#include "UI.hpp"

namespace facade
{
  UI::UI(QObject* parent) : QObject(parent) {}

  QString UI::FormatByteAmount(quint64 bytes, Rounding rounding)
  {
    return facade::FormatByteAmount(bytes, static_cast<facade::Rounding> (rounding));
  }
} // namespace facade