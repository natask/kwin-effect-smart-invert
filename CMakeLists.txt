cmake_minimum_required(VERSION 3.16)

set(SMARTINVERT kwin_effect_smart_invert)

project(${SMARTINVERT})

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.91")


cmake_policy(SET CMP0054 NEW)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include (FeatureSummary)

add_definitions (-Wall -Werror)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

find_package (ECM REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${ECM_MODULE_PATH}
    ${ECM_KDE_MODULE_DIR}
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/Modules
)

include(KDEInstallDirs)
include(KDECMakeSettings)

feature_summary (WHAT ALL)

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

include(ECMSetupQtPluginMacroNames)
ecm_setup_qtplugin_macro_names(
    JSON_ARG2
    "KWIN_EFFECT_FACTORY"
    JSON_ARG3
    "KWIN_EFFECT_FACTORY_ENABLED"
    "KWIN_EFFECT_FACTORY_SUPPORTED"
    JSON_ARG4
    "KWIN_EFFECT_FACTORY_SUPPORTED_ENABLED"
    CONFIG_CODE_VARIABLE
    PACKAGE_SETUP_AUTOMOC_VARIABLES
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

function(kwin_add_effect_config name)
    list(REMOVE_ITEM ARGV ${name})
    kcoreaddons_add_plugin(${name} INSTALL_NAMESPACE "kwin/effects/configs" SOURCES ${ARGV})
endfunction()

macro(KWIN4_ADD_EFFECT_MODULE name)
    kcoreaddons_add_plugin(${name} SOURCES ${ARGN} INSTALL_NAMESPACE "kwin/effects/plugins")
endmacro()
#######################################
# Effect
find_package (Qt5 REQUIRED COMPONENTS
    Core
)

find_package (KF5 REQUIRED COMPONENTS
    Service
    WindowSystem
    GlobalAccel
    I18n
)

set(SMART_INVERT_SRCS plugin.cpp invert.cpp)
kconfig_add_kcfg_files(SMART_INVERT_SRCS invertconfig.kcfgc)

kwin4_add_effect_module(${SMARTINVERT} ${SMART_INVERT_SRCS})

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
        KF5::ConfigGui
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


install(FILES data/1.40/invert.frag DESTINATION ${DATAPATH}kwin/shaders/1.40)
install(FILES data/1.10/invert.frag DESTINATION ${DATAPATH}kwin/shaders/1.10)

install(DIRECTORY  data/1.40 DESTINATION ${DATAPATH}kwin/shaders FILES_MATCHING PATTERN "*.cpp")
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

set(SMARTINVERTCONFIG kwin_effect_smart_invert_config)
set(SMARTINVERTCONFIG_SRCS invert_config.cpp)
ki18n_wrap_ui(SMARTINVERTCONFIG_SRCS invert_config.ui)
kconfig_add_kcfg_files(SMARTINVERTCONFIG_SRCS invertconfig.kcfgc)

qt5_add_dbus_interface(SMARTINVERTCONFIG_SRCS dbus.xml kwineffects_interface)
add_library(${SMARTINVERTCONFIG} MODULE ${SMARTINVERTCONFIG_SRCS})

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

kcoreaddons_desktop_to_json(${SMARTINVERTCONFIG} smartInvertConfig.desktop SERVICE_TYPES kcmodule.desktop)

install(
    TARGETS
        ${SMARTINVERTCONFIG}
    DESTINATION
        ${MODULEPATH}/kwin/effects/configs
)
