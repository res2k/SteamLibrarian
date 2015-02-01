add_library(Piping
            src/Piping.cpp
            include/piping/Piping.hpp
           )
target_include_directories(Piping PUBLIC include)
target_link_libraries(Piping Qt5::Core)

