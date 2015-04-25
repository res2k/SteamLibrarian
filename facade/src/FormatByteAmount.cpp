/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FormatByteAmount.hpp"

#include <QObject>

namespace facade
{
  namespace
  {
    template<typename T> static inline T my_round(T x, Rounding rounding)
    {
      return (rounding == roundUp) ? std::ceil(x) : std::floor(x);
    }
  }

  QString FormatByteAmount(quint64 size, Rounding rounding)
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
