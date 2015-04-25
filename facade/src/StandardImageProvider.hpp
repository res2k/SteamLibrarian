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
 * QML image provider for 'standard' images
 */
#ifndef __FACADE_STANDARDIMAGEPROVIDER_HPP__
#define __FACADE_STANDARDIMAGEPROVIDER_HPP__

#include <QQuickImageProvider>
#include <QStyle>

#include "boost/variant.hpp"

namespace facade
{
  class StandardImageProvider : public QQuickImageProvider
  {
    enum Unknown {};
    /// Type of internal image source we support
    typedef boost::variant<Unknown, QStyle::StandardPixmap, QStyle::PrimitiveElement> image_source_type;
    /// Map of supported standard images
    QHash<QString, image_source_type> m_image_sources;
  public:
    StandardImageProvider();

    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);
  private:
    /// Request a pixmap for a QStyle::StandardPixmap
    QPixmap requestStandardPixmap(QStyle::StandardPixmap id, const QString& arg, QSize* size, const QSize& requestedSize);
    /// Request a pixmap for a QStyle::PrimitiveElement
    QPixmap requestPrimitiveElement(QStyle::PrimitiveElement id, const QString& arg, QSize* size, const QSize& requestedSize);
  };
} // namespace facade

#endif // __FACADE_STANDARDIMAGEPROVIDER_HPP__
