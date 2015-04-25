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

#include "StandardImageProvider.hpp"

#include <QApplication>
#include <QPainter>
#include <QStyleOption>

namespace facade
{
  StandardImageProvider::StandardImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
  {
    m_image_sources[QStringLiteral("MessageBoxInformation")] = QStyle::SP_MessageBoxInformation;
    m_image_sources[QStringLiteral("MessageBoxWarning")] = QStyle::SP_MessageBoxWarning;
    m_image_sources[QStringLiteral("MessageBoxCritical")] = QStyle::SP_MessageBoxCritical;
    m_image_sources[QStringLiteral("MessageBoxQuestion")] = QStyle::SP_MessageBoxQuestion;
    m_image_sources[QStringLiteral("IndicatorArrowDown")] = QStyle::PE_IndicatorArrowDown;
  }

  QPixmap StandardImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
  {
    QStringRef sourceStr (&id);
    int argIndex = id.indexOf('?');
    QStringRef argStr;
    if (argIndex > 0)
    {
      argStr = id.midRef(argIndex + 1);
      sourceStr = id.midRef(0, argIndex);
    }
    image_source_type source = m_image_sources.value(sourceStr.toString());
    {
      QStyle::StandardPixmap* standardPixmap = boost::get<QStyle::StandardPixmap>(&source);
      if (standardPixmap) return requestStandardPixmap(*standardPixmap, argStr.toString(), size, requestedSize);
    }
    {
      QStyle::PrimitiveElement* primElement = boost::get<QStyle::PrimitiveElement>(&source);
      if (primElement) return requestPrimitiveElement(*primElement, argStr.toString(), size, requestedSize);
    }
    return QPixmap();
  }

  QPixmap StandardImageProvider::requestStandardPixmap(QStyle::StandardPixmap id, const QString& arg, QSize* size, const QSize& requestedSize)
  {
    QIcon icon = qApp->style()->standardPixmap(id);
    QPixmap pm = icon.pixmap(requestedSize);
    if (size) *size = pm.size();
    return pm;
  }

  QPixmap StandardImageProvider::requestPrimitiveElement(QStyle::PrimitiveElement id, const QString& arg, QSize* size, const QSize& requestedSize)
  {
    QStyle* style = qApp->style();
    QSize pmSize(requestedSize);
    if (!pmSize.isValid())
    {
      int defaultSize;
      switch (id)
      {
      case QStyle::PE_IndicatorArrowDown:
        defaultSize = style->pixelMetric(QStyle::PM_MenuButtonIndicator) * 0.8;
        break;
      default:
        defaultSize = 16;
      }
      pmSize = QSize(defaultSize, defaultSize);
    }
    QPixmap pm(pmSize.width(), pmSize.height());
    pm.fill(Qt::transparent);
    {
      QPainter painter(&pm);
      QStyleOption styleOpt;
      styleOpt.rect = QRect(0, 0, pm.width(), pm.height());
      {
        styleOpt.state = QStyle::State_Enabled | QStyle::State_AutoRaise;
        QStringList stateStrs = arg.split(',');
        for (const QString& stateStr : stateStrs)
        {
          if (stateStr.compare(QStringLiteral("disabled")) == 0)
            styleOpt.state &= ~QStyle::State_Enabled;
        }
      }
      style->drawPrimitive(id, &styleOpt, &painter);
    }
    if (size) *size = pm.size();
    return pm;
  }
} // namespace facade
