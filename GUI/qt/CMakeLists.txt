cmake_minimum_required(VERSION 3.16)

project(CppLLMTranslate VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS Network)




set(PROJECT_SOURCES
        main.cpp
        CommonUtils.cpp


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CppLLMTranslate
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        CustomTabStyle.h

        MessageManager.h MessageManager.cpp
        cJSON.h cJSON.cpp
        agreement.h agreement.cpp
        StateManager.h StateManager.cpp
        start_page.h start_page.cpp
        simple_page.h simple_page.cpp
        ConfigManager.h ConfigManager.cpp
        intricate_page.h intricate_page.cpp
        HttpManager.h HttpManager.cpp
        FileManager.h FileManager.cpp
        FileTranslation_page.h FileTranslation_page.cpp
        ThreadSafeString.h ThreadSafeString.cpp
        ModelsInfo.h ModelsInfo.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CppLLMTranslate APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CppLLMTranslate SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CppLLMTranslate
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CppLLMTranslate PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CppLLMTranslate PRIVATE Qt6::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt)
endif()
set_target_properties(CppLLMTranslate PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CppLLMTranslate
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CppLLMTranslate)
endif()
