add_library(Piping
            src/Piping.cpp
            src/RunningStateDetector.cpp
            src/RunningStateDetector.hpp
            include/piping/Piping.hpp
           )
target_include_directories(Piping PUBLIC include)
target_link_libraries(Piping Qt5::Core)

