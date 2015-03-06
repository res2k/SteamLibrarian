add_library(Piping
            src/App.cpp
            src/Libraries.cpp
            src/Library.cpp
            src/Piping.cpp
            src/RegisterTypes.cpp
            src/RunningStateDetector.cpp
            src/RunningStateDetector.hpp
            include/piping/App.hpp
            include/piping/Libraries.hpp
            include/piping/Library.hpp
            include/piping/Piping.hpp
            include/piping/RegisterTypes.hpp
           )
target_include_directories(Piping PUBLIC include)
target_link_libraries(Piping Qt5::Core Qt5::Concurrent Qt5::Qml)

