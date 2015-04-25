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