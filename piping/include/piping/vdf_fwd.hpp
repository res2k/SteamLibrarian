/**\file
 * VDF-related forward typedefs
 */
#ifndef __PIPING_VDF_FWD_HPP__
#define __PIPING_VDF_FWD_HPP__

#include <boost/property_tree/ptree_fwd.hpp>

class QString;

namespace piping
{
  namespace vdf
  {
    /// VDF contents property tree type
    typedef boost::property_tree::wiptree vdf_ptree;
  } // namespace vdf
} // namespace piping

#endif // __PIPING_VDF_FWD_HPP__
