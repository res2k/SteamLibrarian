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
