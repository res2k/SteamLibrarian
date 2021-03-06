qt5_add_resources(FACADE_QRC src/facade.qrc OPTIONS --compress 9 --threshold 0)
add_library(Facade
            src/AppsModel.cpp
            src/AppsModel.hpp
            src/FormatByteAmount.cpp
            src/FormatByteAmount.hpp
            src/LibrariesModel.cpp
            src/LibrariesModel.hpp
            src/RedBlackTree.hpp
            src/RegisterTypes.cpp
            src/sortfilterproxymodel.cpp
            src/sortfilterproxymodel.h
            src/StandardImageProvider.cpp
            src/StandardImageProvider.hpp
            src/Start.cpp
            src/UI.cpp
            src/UI.hpp
            include/facade/RegisterTypes.hpp
            include/facade/Start.hpp
            src/facade.qrc ${FACADE_QRC}
            src/main.qml
           )
target_include_directories(Facade PUBLIC include)
target_link_libraries(Facade PRIVATE Piping)
target_link_libraries(Facade PUBLIC Qt5::Core Qt5::Qml Qt5::Quick)

if(CXXTEST_FOUND)
    CXXTEST_ADD_TEST(test_Facade test_Facade.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/test_RedBlackTree.hpp)
    target_include_directories(test_Facade PRIVATE ${Boost_INCLUDE_DIR})
endif()
