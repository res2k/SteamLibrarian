/**\file
 * VDF file handling
 */
#ifndef __PIPING_VDF_HPP__
#define __PIPING_VDF_HPP__

#include <boost/property_tree/ptree.hpp>

class QString;

namespace piping
{
  namespace vdf
  {
    /// VDF contents property tree type
    typedef boost::property_tree::wiptree vdf_ptree;
    /// Read a VDF
    vdf_ptree ReadVDF(const QString& path);
  } // namespace vdf
} // namespace piping

#endif // __PIPING_VDF_HPP__
