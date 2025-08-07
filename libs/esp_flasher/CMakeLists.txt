file(GLOB_RECURSE SRC include/*.hpp src/*.cpp)
add_library(ESPFlasher ${SRC})
add_library(Qt::ESPFlasher ALIAS ESPFlasher)

target_compile_features(ESPFlasher PUBLIC cxx_std_20)

target_include_directories(
  ESPFlasher SYSTEM
  INTERFACE include
  PRIVATE include/esp_flasher)

if(NOT TARGET flasher)
  cpmaddpackage(
    NAME
    esp-serial-flasher
    GITHUB_REPOSITORY
    "espressif/esp-serial-flasher"
    VERSION
    1.9.0
    SYSTEM
    ON
    OPTIONS
    "PORT USER_DEFINED"
    # "MD5_ENABLED OFF" # Currently not working?!
    "SERIAL_FLASHER_STUB_PULL_VERSION 1.7.2"
    "SERIAL_FLASHER_STUB_PULL_SOURCE https://github.com/espressif/esptool-legacy-flasher-stub/releases/download"
  )
endif()

if(NOT TARGET magic_enum)
  cpmaddpackage("gh:Neargye/magic_enum@0.9.7")
endif()

if(NOT TARGET Microsoft.GSL::GSL)
  cpmaddpackage("gh:microsoft/GSL@4.2.0")
endif()

target_link_libraries(
  ESPFlasher
  PUBLIC flasher Qt::Core Qt::SerialPort Qt::Widgets
  PRIVATE magic_enum::magic_enum Microsoft.GSL::GSL)
