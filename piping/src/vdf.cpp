#include "vdf.hpp"

#include <QString>

#include <boost/filesystem/path.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <codecvt>

typedef boost::iostreams::stream<boost::iostreams::code_converter <
  boost::iostreams::file_descriptor_source,
  std::codecvt_utf8_utf16<wchar_t> > > utf8_fwistream;

namespace piping
{
  namespace vdf
  {
    vdf_ptree ReadVDF(const QString& path)
    {
      utf8_fwistream vdf_stream(boost::filesystem::path(path.toStdWString()));
      vdf_ptree tree;
      boost::property_tree::read_info(vdf_stream, tree);
      return std::move(tree);
    }
  } // namespace vdf
} // namespace piping
