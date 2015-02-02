qt5_add_resources(FACADE_QRC src/facade.qrc OPTIONS --compress 9 --threshold 0)
add_library(Facade
            src/Start.cpp
            include/facade/Start.hpp
            src/facade.qrc ${FACADE_QRC}
            src/main.qml
           )
target_include_directories(Facade PUBLIC include)
target_link_libraries(Facade Qt5::Core Qt5::Qml)
