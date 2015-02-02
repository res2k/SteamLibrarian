/**\file
 * QML singleton registration for Piping object
 */
#ifndef __PIPING_REGISTERSINGLETON_HPP__
#define __PIPING_REGISTERSINGLETON_HPP__

namespace piping
{
	/**
	 * QML singleton registration for Piping object
	 */
	void RegisterSingleton(const char * uri, int versionMajor, int versionMinor);
} // namespace piping

#endif // __PIPING_REGISTERSINGLETON_HPP__
