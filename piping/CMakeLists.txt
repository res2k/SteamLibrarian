add_library(Piping
            src/App.cpp
            src/AppMover.cpp
            src/AppMoverWorker.cpp
            src/AppMoverWorker.hpp
            src/Libraries.cpp
            src/Library.cpp
            src/Piping.cpp
            src/RegisterTypes.cpp
            src/RunningStateDetector.cpp
            src/RunningStateDetector.hpp
            src/vdf.cpp
            src/vdf.hpp
            include/piping/App.hpp
            include/piping/AppMover.hpp
            include/piping/Libraries.hpp
            include/piping/Library.hpp
            include/piping/Piping.hpp
            include/piping/RegisterTypes.hpp
            include/piping/vdf_fwd.hpp
           )
target_include_directories(Piping PUBLIC include)
target_include_directories(Piping PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(Piping Qt5::Core Qt5::Concurrent Qt5::Qml Qt5::Widgets Qt5::WinExtras
                      ${Boost_FILESYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_SYSTEM_LIBRARY})
