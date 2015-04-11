/**\file
 * Helper function to format a byte amount
 */
#ifndef __FORMATEBYTEAMOUNT_HPP__
#define __FORMATEBYTEAMOUNT_HPP__

#include <QString>

namespace facade
{
  enum Rounding { roundUp, roundDown };

  QString FormatByteAmount(quint64 size, Rounding rounding);
} // namespace facade

#endif // __FORMATEBYTEAMOUNT_HPP__
