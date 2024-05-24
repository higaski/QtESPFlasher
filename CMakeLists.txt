cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(CMakePrintHelpers)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/CMakeModules"
  GIT_TAG v0.9.1
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  QtESPFlasher
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(QT_VERSION 6.4.3)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTORCC_OPTIONS "-compress-algo;zlib" "--compress;9")

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(QtESPFlasher WIN32 ${SRC})

target_compile_definitions(QtESPFlasher
                           PRIVATE QTESPFLASHER_VERSION="${PROJECT_VERSION}")

if(CMAKE_BUILD_TYPE STREQUAL Release)
  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    build_qt(
      ${QT_VERSION}
      MODULES
      qt5compat
      qtbase
      qtserialport
      qtsvg
      CMAKE_OPTIONS
      -DCMAKE_BUILD_TYPE=Release
      -DINPUT_optimize_size=ON
      -DBUILD_SHARED_LIBS=ON
      -DBUILD_WITH_PCH=ON
      -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
      # Set C_STANDARD and CXX_STANDARD
      -DFEATURE_c11=ON
      -DFEATURE_cxx20=ON
      # Disable features that are not needed
      -DFEATURE_accessibility=OFF
      -DFEATURE_aesni=OFF
      -DFEATURE_androiddeployqt=OFF
      -DFEATURE_animation=OFF
      -DFEATURE_backtrace=OFF
      -DFEATURE_brotli=OFF
      -DFEATURE_clock_gettime=OFF
      -DFEATURE_clock_monotonic=OFF
      -DFEATURE_colordialog=OFF
      -DFEATURE_colornames=OFF
      -DFEATURE_columnview=OFF
      -DFEATURE_commandlinkbutton=OFF
      -DFEATURE_completer=OFF
      -DFEATURE_concatenatetablesproxymodel=OFF
      -DFEATURE_datawidgetmapper=OFF
      -DFEATURE_datetimeedit=OFF
      -DFEATURE_datetimeparser=OFF
      -DFEATURE_dbus=OFF
      -DFEATURE_desktopservices=OFF
      -DFEATURE_dial=OFF
      -DFEATURE_dnslookup=OFF
      -DFEATURE_dockwidget=OFF
      -DFEATURE_drm_atomic=OFF
      -DFEATURE_dtls=OFF
      -DFEATURE_easingcurve=OFF
      -DFEATURE_effects=OFF
      -DFEATURE_enable_new_dtags=OFF
      -DFEATURE_errormessage=OFF
      -DFEATURE_evdev=OFF
      -DFEATURE_eventfd=OFF
      -DFEATURE_f16c=OFF
      -DFEATURE_fontcombobox=OFF
      -DFEATURE_fontconfig=OFF
      -DFEATURE_fontdialog=OFF
      -DFEATURE_forkfd_pidfd=OFF
      -DFEATURE_formlayout=OFF
      -DFEATURE_futimens=OFF
      -DFEATURE_future=OFF
      -DFEATURE_gbm=OFF
      -DFEATURE_getauxval=OFF
      -DFEATURE_getentropy=OFF
      -DFEATURE_getifaddrs=OFF
      -DFEATURE_gif=OFF
      -DFEATURE_glib=OFF
      -DFEATURE_graphicseffect=OFF
      -DFEATURE_graphicsview=OFF
      -DFEATURE_gssapi=OFF
      -DFEATURE_harfbuzz=OFF
      -DFEATURE_hijricalendar=OFF
      -DFEATURE_http=OFF
      -DFEATURE_ico=OFF
      -DFEATURE_identityproxymodel=OFF
      -DFEATURE_im=OFF
      -DFEATURE_image_heuristic_mask=OFF
      -DFEATURE_image_text=OFF
      -DFEATURE_imageformat_bmp=OFF
      -DFEATURE_imageformat_jpeg=OFF
      -DFEATURE_imageformat_ppm=OFF
      -DFEATURE_imageformat_xbm=OFF
      -DFEATURE_imageio_text_loading=OFF
      -DFEATURE_inotify=OFF
      -DFEATURE_inputdialog=OFF
      -DFEATURE_islamiccivilcalendar=OFF
      -DFEATURE_jalalicalendar=OFF
      -DFEATURE_keysequenceedit=OFF
      -DFEATURE_kms=OFF
      -DFEATURE_largefile=OFF
      -DFEATURE_lcdnumber=OFF
      -DFEATURE_libinput=OFF
      -DFEATURE_libinput_axis_api=OFF
      -DFEATURE_libinput_hires_wheel_support=OFF
      -DFEATURE_libudev=OFF
      -DFEATURE_linkat=OFF
      -DFEATURE_linux_netlink=OFF
      -DFEATURE_localserver=OFF
      -DFEATURE_mdiarea=OFF
      -DFEATURE_mimetype=OFF
      -DFEATURE_mimetype_database=OFF
      -DFEATURE_movie=OFF
      -DFEATURE_mtdev=OFF
      -DFEATURE_network=OFF
      -DFEATURE_networkdiskcache=OFF
      -DFEATURE_networkinterface=OFF
      -DFEATURE_networkproxy=OFF
      -DFEATURE_ocsp=OFF
      -DFEATURE_openssl=OFF
      -DFEATURE_openssl_linked=OFF
      -DFEATURE_openssl_runtime=OFF
      -DFEATURE_opensslv11=OFF
      -DFEATURE_pdf=OFF
      -DFEATURE_picture=OFF
      -DFEATURE_plugin_manifest=OFF
      -DFEATURE_poll_ppoll=OFF
      -DFEATURE_printsupport=OFF
      -DFEATURE_process=OFF
      -DFEATURE_processenvironment=OFF
      -DFEATURE_progressbar=OFF
      -DFEATURE_progressdialog=OFF
      -DFEATURE_publicsuffix_qt=OFF
      -DFEATURE_publicsuffix_system=OFF
      -DFEATURE_raster_64bit=OFF
      -DFEATURE_raster_fp=OFF
      -DFEATURE_rdrnd=OFF
      -DFEATURE_rdseed=OFF
      -DFEATURE_renameat2=OFF
      -DFEATURE_rubberband=OFF
      -DFEATURE_sessionmanager=OFF
      -DFEATURE_sha3_fast=OFF
      -DFEATURE_shani=OFF
      -DFEATURE_signaling_nan=OFF
      -DFEATURE_sizegrip=OFF
      -DFEATURE_socks5=OFF
      -DFEATURE_sortfilterproxymodel=OFF
      -DFEATURE_spinbox=OFF
      -DFEATURE_splashscreen=OFF
      -DFEATURE_sql=OFF
      -DFEATURE_statusbar=OFF
      -DFEATURE_statustip=OFF
      -DFEATURE_statx=OFF
      -DFEATURE_syntaxhighlighter=OFF
      -DFEATURE_system_proxies=OFF
      -DFEATURE_system_doubleconversion=OFF
      -DFEATURE_system_freetype=OFF
      -DFEATURE_system_harfbuzz=OFF
      -DFEATURE_system_libb2=OFF
      -DFEATURE_system_pcre2=OFF
      -DFEATURE_testlib=OFF
      -DFEATURE_textbrowser=OFF
      -DFEATURE_textmarkdownreader=OFF
      -DFEATURE_textmarkdownwriter=OFF
      -DFEATURE_textodfwriter=OFF
      -DFEATURE_timezone=OFF
      -DFEATURE_toolbox=OFF
      -DFEATURE_topleveldomain=OFF
      -DFEATURE_translation=OFF
      -DFEATURE_tslib=OFF
      -DFEATURE_tuiotouch=OFF
      -DFEATURE_udpsocket=OFF
      -DFEATURE_undocommand=OFF
      -DFEATURE_undogroup=OFF
      -DFEATURE_undostack=OFF
      -DFEATURE_undoview=OFF
      -DFEATURE_vaes=OFF
      -DFEATURE_validator=OFF
      -DFEATURE_vulkan=OFF
      -DFEATURE_whatsthis=OFF
      # Enable freetype to get some sort of font support
      -DFEATURE_freetype=ON
      # Default to xcb and disable all other platform plugins
      -DQT_QPA_DEFAULT_PLATFORM=xcb
      -DFEATURE_linuxfb=OFF
      -DFEATURE_vkkhrdisplay=OFF
      -DFEATURE_vnc=OFF
      -DFEATURE_zstd=OFF
      # No FEATURE flags yet
      -DINPUT_libjpeg=no
      -DINPUT_libmd4c=no)
  elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    # Install Qt6 for host using https://github.com/miurahr/aqtinstall
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    execute_process(
      COMMAND
        ${Python3_EXECUTABLE} -m aqt install-qt --outputdir
        ${CMAKE_BINARY_DIR}/qt6-host-build linux desktop ${QT_VERSION} gcc_64
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND_ERROR_IS_FATAL ANY)

    # Cross-compile Qt6 with mingw-w64
    build_qt(
      ${QT_VERSION}
      MODULES
      qt5compat
      qtbase
      qtserialport
      qtsvg
      CMAKE_OPTIONS
      -DQT_HOST_PATH=${CMAKE_BINARY_DIR}/qt6-host-build/${QT_VERSION}/gcc_64
      -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE=Release
      -DINPUT_optimize_size=ON
      -DBUILD_SHARED_LIBS=ON
      -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
      # Set C_STANDARD and CXX_STANDARD
      -DFEATURE_c11=ON
      -DFEATURE_cxx20=ON
      # Disable features that are not needed
      -DFEATURE_accessibility=OFF
      -DFEATURE_aesni=OFF
      -DFEATURE_androiddeployqt=OFF
      -DFEATURE_animation=OFF
      -DFEATURE_backtrace=OFF
      -DFEATURE_brotli=OFF
      -DFEATURE_clock_gettime=OFF
      -DFEATURE_clock_monotonic=OFF
      -DFEATURE_colordialog=OFF
      -DFEATURE_colornames=OFF
      -DFEATURE_columnview=OFF
      -DFEATURE_commandlinkbutton=OFF
      -DFEATURE_completer=OFF
      -DFEATURE_concatenatetablesproxymodel=OFF
      -DFEATURE_datawidgetmapper=OFF
      -DFEATURE_datetimeedit=OFF
      -DFEATURE_datetimeparser=OFF
      -DFEATURE_dbus=OFF
      -DFEATURE_desktopservices=OFF
      -DFEATURE_dial=OFF
      -DFEATURE_dnslookup=OFF
      -DFEATURE_dockwidget=OFF
      -DFEATURE_drm_atomic=OFF
      -DFEATURE_dtls=OFF
      -DFEATURE_easingcurve=OFF
      -DFEATURE_effects=OFF
      -DFEATURE_enable_new_dtags=OFF
      -DFEATURE_errormessage=OFF
      -DFEATURE_evdev=OFF
      -DFEATURE_eventfd=OFF
      -DFEATURE_f16c=OFF
      -DFEATURE_fontcombobox=OFF
      -DFEATURE_fontconfig=OFF
      -DFEATURE_fontdialog=OFF
      -DFEATURE_forkfd_pidfd=OFF
      -DFEATURE_formlayout=OFF
      -DFEATURE_futimens=OFF
      -DFEATURE_future=OFF
      -DFEATURE_gbm=OFF
      -DFEATURE_getauxval=OFF
      -DFEATURE_getentropy=OFF
      -DFEATURE_getifaddrs=OFF
      -DFEATURE_gif=OFF
      -DFEATURE_glib=OFF
      -DFEATURE_graphicseffect=OFF
      -DFEATURE_graphicsview=OFF
      -DFEATURE_gssapi=OFF
      -DFEATURE_harfbuzz=OFF
      -DFEATURE_hijricalendar=OFF
      -DFEATURE_http=OFF
      -DFEATURE_ico=OFF
      -DFEATURE_identityproxymodel=OFF
      -DFEATURE_im=OFF
      -DFEATURE_image_heuristic_mask=OFF
      -DFEATURE_image_text=OFF
      -DFEATURE_imageformat_bmp=OFF
      -DFEATURE_imageformat_jpeg=OFF
      -DFEATURE_imageformat_ppm=OFF
      -DFEATURE_imageformat_xbm=OFF
      -DFEATURE_imageio_text_loading=OFF
      -DFEATURE_inotify=OFF
      -DFEATURE_inputdialog=OFF
      -DFEATURE_islamiccivilcalendar=OFF
      -DFEATURE_jalalicalendar=OFF
      -DFEATURE_keysequenceedit=OFF
      -DFEATURE_kms=OFF
      -DFEATURE_largefile=OFF
      -DFEATURE_lcdnumber=OFF
      -DFEATURE_libinput=OFF
      -DFEATURE_libinput_axis_api=OFF
      -DFEATURE_libinput_hires_wheel_support=OFF
      -DFEATURE_libudev=OFF
      -DFEATURE_linkat=OFF
      -DFEATURE_linux_netlink=OFF
      -DFEATURE_localserver=OFF
      -DFEATURE_mdiarea=OFF
      -DFEATURE_mimetype=OFF
      -DFEATURE_mimetype_database=OFF
      -DFEATURE_movie=OFF
      -DFEATURE_mtdev=OFF
      -DFEATURE_network=OFF
      -DFEATURE_networkdiskcache=OFF
      -DFEATURE_networkinterface=OFF
      -DFEATURE_networkproxy=OFF
      -DFEATURE_ocsp=OFF
      -DFEATURE_openssl=OFF
      -DFEATURE_openssl_linked=OFF
      -DFEATURE_openssl_runtime=OFF
      -DFEATURE_opensslv11=OFF
      -DFEATURE_pdf=OFF
      -DFEATURE_picture=OFF
      -DFEATURE_plugin_manifest=OFF
      -DFEATURE_poll_ppoll=OFF
      -DFEATURE_printsupport=OFF
      -DFEATURE_process=OFF
      -DFEATURE_processenvironment=OFF
      -DFEATURE_progressbar=OFF
      -DFEATURE_progressdialog=OFF
      -DFEATURE_publicsuffix_qt=OFF
      -DFEATURE_publicsuffix_system=OFF
      -DFEATURE_raster_64bit=OFF
      -DFEATURE_raster_fp=OFF
      -DFEATURE_rdrnd=OFF
      -DFEATURE_rdseed=OFF
      -DFEATURE_renameat2=OFF
      -DFEATURE_rubberband=OFF
      -DFEATURE_sessionmanager=OFF
      -DFEATURE_sha3_fast=OFF
      -DFEATURE_shani=OFF
      -DFEATURE_signaling_nan=OFF
      -DFEATURE_sizegrip=OFF
      -DFEATURE_socks5=OFF
      -DFEATURE_sortfilterproxymodel=OFF
      -DFEATURE_spinbox=OFF
      -DFEATURE_splashscreen=OFF
      -DFEATURE_sql=OFF
      -DFEATURE_statusbar=OFF
      -DFEATURE_statustip=OFF
      -DFEATURE_statx=OFF
      -DFEATURE_syntaxhighlighter=OFF
      -DFEATURE_system_proxies=OFF
      -DFEATURE_system_doubleconversion=OFF
      -DFEATURE_system_freetype=OFF
      -DFEATURE_system_harfbuzz=OFF
      -DFEATURE_system_libb2=OFF
      -DFEATURE_system_pcre2=OFF
      -DFEATURE_testlib=OFF
      -DFEATURE_textbrowser=OFF
      -DFEATURE_textmarkdownreader=OFF
      -DFEATURE_textmarkdownwriter=OFF
      -DFEATURE_textodfwriter=OFF
      -DFEATURE_timezone=OFF
      -DFEATURE_toolbox=OFF
      -DFEATURE_topleveldomain=OFF
      -DFEATURE_translation=OFF
      -DFEATURE_tslib=OFF
      -DFEATURE_tuiotouch=OFF
      -DFEATURE_udpsocket=OFF
      -DFEATURE_undocommand=OFF
      -DFEATURE_undogroup=OFF
      -DFEATURE_undostack=OFF
      -DFEATURE_undoview=OFF
      -DFEATURE_vaes=OFF
      -DFEATURE_validator=OFF
      -DFEATURE_vulkan=OFF
      -DFEATURE_whatsthis=OFF
      # Enable freetype to get some sort of font support
      -DFEATURE_freetype=ON
      # No FEATURE flags yet
      -DINPUT_libjpeg=no
      -DINPUT_libmd4c=no)
  endif()
endif()

find_qt(REQUIRED COMPONENTS Core SerialPort Widgets)

qt_add_resources(
  QtESPFlasher
  logo
  PREFIX
  images
  BASE
  data/images
  FILES
  data/images/logo.png)

cpmaddpackage("gh:Neargye/magic_enum@0.9.5")
cpmaddpackage("gh:microsoft/GSL@4.0.0")
cpmaddpackage("gh:ZIMO-Elektronik/QtBreeze@5.115.0")
cpmaddpackage(
  NAME
  esp-serial-flasher
  GITHUB_REPOSITORY
  "higaski/esp-serial-flasher"
  GIT_TAG
  # TODO actual tag once #103 got merged
  # https://github.com/espressif/esp-serial-flasher/pull/103
  3b17882987fe5f84fa160ced937ce15c98bb07e6
  VERSION
  1.3.0
  SYSTEM
  ON
  OPTIONS
  "PORT USER_DEFINED"
  # "MD5_ENABLED OFF" # Currently not working?!
)
cpmaddpackage(
  NAME
  quazip
  GITHUB_REPOSITORY
  "stachenov/quazip"
  VERSION
  1.4
  SYSTEM
  ON
  OPTIONS
  "QUAZIP_QT_MAJOR_VERSION 6"
  "QUAZIP_INSTALL OFF"
  "QUAZIP_BZIP2 OFF")

target_link_libraries(
  QtESPFlasher
  PRIVATE flasher
          magic_enum::magic_enum
          Microsoft.GSL::GSL
          Qt::BreezeStyleSheets
          Qt::Core
          Qt::SerialPort
          Qt::Widgets
          QuaZip::QuaZip
          -Wl,--gc-sections,-Map=QtESPFlasher.map)

# Deploy release builds with CQtDeployer
if(CMAKE_BUILD_TYPE STREQUAL Release)
  # Output directory of .zip files
  set(TARGET_DIR ${CMAKE_BINARY_DIR}/DistributionKit)

  # Qt6 does not ship with fonts and we have built without fontconfig, so get a
  # free font and copy it
  cpmaddpackage(
    NAME
    dejavu-fonts
    URL
    "https://github.com/dejavu-fonts/dejavu-fonts/releases/download/version_2_37/dejavu-fonts-ttf-2.37.zip"
    VERSION
    2.37
    DOWNLOAD_ONLY
    TRUE)
  add_custom_command(
    TARGET QtESPFlasher
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy
      ${dejavu-fonts_SOURCE_DIR}/ttf/DejaVuSansMono.ttf
      ${TARGET_DIR}/lib/fonts/DejaVuSansMono.ttf)

  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    # Get an old version of CQtDeployer compatible with GLIB 2.31
    find_package(CQtDeployer 1.5.4.17 COMPONENTS Linux)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake6)

    # Qt libs path
    execute_process(
      COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_LIBS
      OUTPUT_VARIABLE LIB_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Qt plugins path
    execute_process(
      COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_PLUGINS
      OUTPUT_VARIABLE QT_INSTALL_PLUGINS
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    file(GLOB EXTRA_PLUGIN ${QT_INSTALL_PLUGINS}/*)
    list(JOIN EXTRA_PLUGIN "," EXTRA_PLUGIN)

    add_custom_command(
      TARGET QtESPFlasher
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        clear #
        noTranslations #
        -bin $<TARGET_FILE:QtESPFlasher> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -extraLibs xcb-cursor #
        -extraPlugin ${EXTRA_PLUGIN} #
        -recursiveDepth 2 #
        -targetDir ${TARGET_DIR} #
        -verbose 3 #
        -platform linux_x86_64 #
    )

  elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    # Don't care about GLIB here, get a modern version
    find_package(CQtDeployer 1.6.2345 COMPONENTS Linux)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake)

    # For Windows we need Qt and system libs, so use CMAKE_FIND_ROOT_PATH
    list(JOIN CMAKE_FIND_ROOT_PATH "," LIB_DIR)

    add_custom_command(
      TARGET QtESPFlasher
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        clear #
        noTranslations #
        -bin $<TARGET_FILE:QtESPFlasher> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -recursiveDepth 2 #
        -targetDir ${TARGET_DIR} #
        -verbose 3 #
        -platform win_x86_64 #
    )
  endif()

  add_custom_command(
    TARGET QtESPFlasher
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E tar "cf"
      QtESPFlasher-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-x64.zip --format=zip
      *
    WORKING_DIRECTORY ${TARGET_DIR})
endif()

if(PROJECT_IS_TOP_LEVEL)
  file(
    DOWNLOAD
    https://raw.githubusercontent.com/ZIMO-Elektronik/.clang-format/master/.clang-format
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC src/*.*pp tests/*.*pp)
  add_clang_format_target(QtESPFlasherFormat OPTIONS -i FILES ${SRC})
endif()
