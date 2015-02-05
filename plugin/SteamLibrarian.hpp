/**\file 
 * QML plugin
 */
#ifndef __PLUGIN_STEAMLIBRARIAN_HPP__
#define __PLUGIN_STEAMLIBRARIAN_HPP__

#include <QQmlExtensionPlugin>

class QSteamLibrarianQmlPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
  void registerTypes(const char *uri);
};

#endif // __PLUGIN_STEAMLIBRARIAN_HPP__
