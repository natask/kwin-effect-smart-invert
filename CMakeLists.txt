cmake_minimum_required(VERSION 3.0)

set(SMARTINVERT kwin_effect_smartInvert)

project(${SMARTINVERT})

set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.9.0")


cmake_policy(SET CMP0054 NEW)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Gui
    Core
    DBus
    UiTools
    Widgets
    X11Extras
    OpenGL
    Network
    Xml
)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5X11Extras_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

# required frameworks by Core
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Crash
    GlobalAccel
    I18n
    KIO
    Service
    Init
    Notifications
    Service
    WidgetsAddons
    WindowSystem
    GuiAddons
)



execute_process(COMMAND kf5-config --install module OUTPUT_VARIABLE MODULEPATH OUTPUT_STRIP_TRAILING_WHITESPACE)

find_path(EFFECTS_H kwineffects.h PATH_SUFFIXES kf5)

if (EFFECTS_H)
    include_directories(${EFFECTS_H})
else (EFFECTS_H)
    message(STATUS "didnt find kwineffects.h, not building effects")
endif (EFFECTS_H)

find_library(KWIN_EFFECTS NAMES kwineffects PATH_SUFFIXES kf5)
find_library(KWIN_GLUTILS NAMES kwinglutils PATH_SUFFIXES kf5)
find_library(OPENGL NAMES GL)

if (NOT KWIN_EFFECTS)
    message(STATUS "didnt find kwineffects lib, not building effects")
endif (NOT KWIN_EFFECTS)

if (NOT KWIN_GLUTILS)
    message(STATUS "didnt find kwin glutils lib, not building effects")
endif (NOT KWIN_GLUTILS)

if (NOT OPENGL)
    message(STATUS "didnt find opengl, not building effects")
endif (NOT OPENGL)

if (NOT EFFECTS_H OR NOT KWIN_GLUTILS OR NOT KWIN_EFFECTS OR NOT OPENGL)
    message(FATAL_ERROR "cant continue")
endif (NOT EFFECTS_H OR NOT KWIN_GLUTILS OR NOT KWIN_EFFECTS OR NOT OPENGL)

if (XCB_FOUND)
    target_link_libraries(${SMARTINVERT} PUBLIC Qt5::X11Extras XCB::XCB)
else (XCB_FOUND)
    if (X11_FOUND)
        target_link_libraries(${SMARTINVERT} PUBLIC ${X11_X11_LIB} ${X11_LIBRARIES})
    endif (X11_FOUND)
endif (XCB_FOUND)


#######################################
# Effect
set(kwin_smartInvert_SRCS invert.cpp)

add_library(${SMARTINVERT} MODULE ${kwin_smartInvert_SRCS})

qt5_add_resources(${SMARTINVERT} shaders.qrc) # will need to add shader.qrc later will see how kwin does this

kcoreaddons_desktop_to_json(${SMARTINVERT} smartInvert.desktop SERVICE_TYPES kwineffect.desktop)

target_link_libraries(${SMARTINVERT}
    PUBLIC
        Qt5::Core
        Qt5::Gui
        ${KWIN_EFFECTS}
        ${KWIN_GLUTILS}
        epoxy
        GL

   PRIVATE
        KF5::ConfigCore
        KF5::CoreAddons
        KF5::ConfigWidgets
        KF5::GuiAddons
        KF5::WindowSystem

        KF5::ConfigWidgets
        KF5::GlobalAccel
        KF5::I18n
        KF5::XmlGui
        KF5::Service
        #KWinEffects::KWinEffects
        )

install(
    TARGETS
    ${SMARTINVERT}
    DESTINATION
        ${PLUGIN_INSTALL_DIR}/kwin/effects/plugins/
)

#######################################
# Config
find_package (Qt5 REQUIRED COMPONENTS
    Core
    DBus
)

find_package (KF5 REQUIRED COMPONENTS
    ConfigWidgets
    GlobalAccel
    I18n
    Service
    XmlGui
)

set(SMARTINVERTCONFIG kwin_smartInvert_config)
set(kwin_smartInvert_config_SRCS invert_config.cpp)

qt5_add_dbus_interface(kwin_smartInvert_config_SRCS dbus.xml kwineffects_interface)

add_library(kwin_smartInvert_config MODULE ${kwin_invert_config_SRCS})

target_link_libraries(${SMARTINVERTCONFIG}
    Qt5::Core
    Qt5::DBus
    KF5::ConfigWidgets
    KF5::GlobalAccel
    KF5::I18n
    KF5::Service
    KF5::XmlGui
    KF5::ConfigWidgets
    KF5::GlobalAccel
    KF5::I18n
    KF5::XmlGui
)

kcoreaddons_desktop_to_json(${SMARTINVERTCONFIG}  smartInvert_config.desktop SERVICE_TYPES kcmodule.desktop)

install(
    TARGETS
        ${SMARTINVERTCONFIG}
    DESTINATION
        ${PLUGIN_INSTALL_DIR}/kwin/effects/configs
)

#######################################
# Effect
# Gray
#######################################
# Config
# add_subdirectory( kcm )
# set(kwin_effect_grayscale_SRCS plugin.cpp GrayscaleEffect.cpp)
# qt5_add_resources(kwin_effect_grayscale_SRCS shaders.qrc)
# add_library(kwin_effect_grayscale SHARED ${kwin_effect_grayscale_SRCS})

#  kcoreaddons_desktop_to_json(kwin_effect_grayscale grayscale.desktop SERVICE_TYPES kwineffect.desktop)



# for other file, smart invert
# add_library(${SMARTINVERT} MODULE smartInvert.cpp)


# install(TARGETS ${SMARTINVERT} DESTINATION ${MODULEPATH}/kwin/effects/plugins)
# # execute_process(COMMAND kf5-config --install services OUTPUT_VARIABLE SERVICEPATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# #install(FILES smartInvert.desktop DESTINATION ${SERVICEPATH}/kwin)

# execute_process(COMMAND kf5-config --install data OUTPUT_VARIABLE DATAPATH OUTPUT_STRIP_TRAILING_WHITESPACE)

# install(FILES shaders_140/smartInvert.frag DESTINATION ${DATAPATH}/kwin/shaders/1.40)
# install(FILES shaders_110/smartInvert.frag DESTINATION ${DATAPATH}/kwin/shaders/1.10)



