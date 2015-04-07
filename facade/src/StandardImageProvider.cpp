#include "StandardImageProvider.hpp"

#include <QApplication>

namespace facade
{
  StandardImageProvider::StandardImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
  {
    m_image_sources[QStringLiteral("MessageBoxInformation")] = QStyle::SP_MessageBoxInformation;
    m_image_sources[QStringLiteral("MessageBoxWarning")] = QStyle::SP_MessageBoxWarning;
    m_image_sources[QStringLiteral("MessageBoxCritical")] = QStyle::SP_MessageBoxCritical;
    m_image_sources[QStringLiteral("MessageBoxQuestion")] = QStyle::SP_MessageBoxQuestion;
  }

  QPixmap StandardImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
  {
    image_source_type source = m_image_sources.value(id);
    {
      QStyle::StandardPixmap* standardPixmap = boost::get<QStyle::StandardPixmap>(&source);
      if (standardPixmap) return requestStandardPixmap(*standardPixmap, size, requestedSize);
    }
    return QPixmap();
  }

  QPixmap StandardImageProvider::requestStandardPixmap(QStyle::StandardPixmap id, QSize* size, const QSize& requestedSize)
  {
    QIcon icon = qApp->style()->standardPixmap(id);
    QPixmap pm = icon.pixmap(requestedSize);
    if (size) *size = pm.size();
    return pm;
  }
} // namespace facade
