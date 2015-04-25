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

#include "vdf.hpp"

#include <QString>

#include <boost/filesystem/path.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <codecvt>

typedef boost::iostreams::stream<boost::iostreams::code_converter <
  boost::iostreams::file_descriptor_source,
  std::codecvt_utf8_utf16<wchar_t> > > utf8_fwistream;

namespace piping
{
  namespace vdf
  {
    vdf_ptree ReadVDF(const QString& path)
    {
      utf8_fwistream vdf_stream(boost::filesystem::path(path.toStdWString()));
      vdf_ptree tree;
      boost::property_tree::read_info(vdf_stream, tree);
      return std::move(tree);
    }
  } // namespace vdf
} // namespace piping
