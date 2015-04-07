/**\file
 * Helper function to format a byte amount
 */
#ifndef __FORMATEBYTEAMOUNT_HPP__
#define __FORMATEBYTEAMOUNT_HPP__

#include <QString>

namespace facade
{
  enum Rounding { roundUp, roundDown };
  namespace
  {
    template<typename T> static inline T my_round(T x, Rounding rounding)
    {
      return (rounding == roundUp) ? std::ceil(x) : std::floor(x);
    }
  }

  static QString FormatByteAmount(quint64 size, Rounding rounding)
  {
    const QString units[] =
    {
      QObject::tr("%1 bytes"),
      QObject::tr("%1 KB"),
      QObject::tr("%1 MB"),
      QObject::tr("%1 GB"),
      QObject::tr("%1 TB"),
      QObject::tr("%1 PB")
    };
    const int numUnits = sizeof(units) / sizeof(units[0]);
    // Format size to three digits
    int unit = 0;
    quint64 remainder = 0;
    while ((size >= 1000) && (unit < numUnits - 1))
    {
      remainder = size % 1024;
      size /= 1024;
      unit++;
    }
    double fmt_val;
    int fmt_digits;
    if (unit == 0)
    {
      fmt_val = size;
      fmt_digits = 0;
    }
    else if (size >= 100)
    {
      fmt_val = size;
      if ((rounding == roundUp) && (remainder > 0)) fmt_val += 1;
      fmt_digits = 0;
    }
    else if (size >= 10)
    {
      fmt_val = size + (my_round(remainder / 102.4, rounding)/10);
      fmt_digits = 1;
    }
    else
    {
      fmt_val = size + (my_round(remainder / 10.24, rounding)/100);
      fmt_digits = 2;
    }

    return units[unit].arg(fmt_val, 0, 'f', fmt_digits);
  }
} // namespace facade

#endif // __FORMATEBYTEAMOUNT_HPP__
