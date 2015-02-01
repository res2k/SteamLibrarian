/**\file
 * Piping - interface to Steam.
 */
#ifndef __PIPING_PIPING_HPP__
#define __PIPING_PIPING_HPP__

#include <QObject>
#include <QString>

namespace piping
{
    class Piping : public QObject
    {
      Q_OBJECT
    public:
      Piping(QObject* parent = nullptr);

      /// Steam install path (or an empty string, if not found)
      QString steamInstallPath() const { return m_steamInstallPath; }

      Q_PROPERTY(QString steamInstallPath READ steamInstallPath CONSTANT)
    private:
      /// Steam install path (or empty, if not found)
      QString m_steamInstallPath;

      /// Fetch Steam install path
      void FetchInstallPath();
    };
} // namespace piping

#endif // __PIPING_PIPING_HPP__
