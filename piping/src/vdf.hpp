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
