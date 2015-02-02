#include "piping/RegisterSingleton.hpp"

#include <QtQml>

#include "piping/Piping.hpp"

namespace piping
{
	static QObject* CreatePiping(QQmlEngine *engine, QJSEngine *scriptEngine)
	{
		return new Piping;
	}

	void RegisterSingleton(const char * uri, int versionMajor, int versionMinor)
	{
		qmlRegisterSingletonType<Piping>(uri, versionMajor, versionMinor, "Piping", &CreatePiping);
	}
} // namespace piping
