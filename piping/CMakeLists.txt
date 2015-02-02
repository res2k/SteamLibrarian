add_library(Piping
            src/Piping.cpp
            src/RegisterSingleton.cpp
            src/RunningStateDetector.cpp
            src/RunningStateDetector.hpp
            include/piping/Piping.hpp
            include/piping/RegisterSingleton.hpp
           )
target_include_directories(Piping PUBLIC include)
target_link_libraries(Piping Qt5::Core Qt5::Concurrent Qt5::Qml)

