/**\file
 * Singleton for UI-related helper functions
 */
#ifndef __FACADE_UI_HPP__
#define __FACADE_UI_HPP__

#include <QObject>

#include "FormatByteAmount.hpp"

namespace facade
{
  class UI : public QObject
  {
    Q_OBJECT
  public:
    UI(QObject* parent = nullptr);
    
    enum Rounding
    {
      roundUp = facade::roundUp,
      roundDown = facade::roundDown
    };
    Q_ENUMS(Rounding);
    Q_INVOKABLE QString FormatByteAmount(quint64 bytes, Rounding rounding);
  };
} // namespace facade

#endif // __FACADE_UI_HPP__