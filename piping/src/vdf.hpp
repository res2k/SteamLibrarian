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
 * VDF file handling
 */
#ifndef __PIPING_VDF_HPP__
#define __PIPING_VDF_HPP__

#include "piping/vdf_fwd.hpp"

#include <boost/property_tree/ptree.hpp>

namespace piping
{
  namespace vdf
  {
    /// Read a VDF
    vdf_ptree ReadVDF(const QString& path);
  } // namespace vdf
} // namespace piping

#endif // __PIPING_VDF_HPP__
