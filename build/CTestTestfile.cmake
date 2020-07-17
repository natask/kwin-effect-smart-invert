# CMake generated Testfile for 
# Source directory: /home/savnkk/invert
# Build directory: /home/savnkk/invert/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(appstreamtest "/usr/bin/cmake" "-DAPPSTREAMCLI=/usr/bin/appstreamcli" "-DINSTALL_FILES=/home/savnkk/invert/build/install_manifest.txt" "-P" "/usr/share/ECM/kde-modules/appstreamtest.cmake")
set_tests_properties(appstreamtest PROPERTIES  _BACKTRACE_TRIPLES "/usr/share/ECM/kde-modules/KDECMakeSettings.cmake;184;add_test;/usr/share/ECM/kde-modules/KDECMakeSettings.cmake;202;appstreamtest;/usr/share/ECM/kde-modules/KDECMakeSettings.cmake;0;;/home/savnkk/invert/CMakeLists.txt;29;include;/home/savnkk/invert/CMakeLists.txt;0;")
