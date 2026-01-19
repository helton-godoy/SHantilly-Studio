cmake_minimum_required(VERSION 3.16)

# Integração automática com vcpkg (Manifest Mode)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE
   AND EXISTS "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(shantilly-studio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Cobertura de Código (Coverage)
# ============================================================================
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                             "Clang")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
  endif()
endif()

# 1. Localizar pacotes do Qt6 (via vcpkg)
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Svg)

# Configuração Automática do Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Configuração do caminho do SHantilly Core
if(NOT SHANTILLY_ROOT)
  get_filename_component(SHANTILLY_ROOT
                         "${CMAKE_CURRENT_SOURCE_DIR}/../SHantilly" ABSOLUTE)
endif()
message(STATUS "Using SHANTILLY_ROOT: ${SHANTILLY_ROOT}")

# Localização da Biblioteca SHantilly UI
set(SHANTILLY_UI_DIR "${SHANTILLY_ROOT}/libs/SHantilly-ui")
set(SHANTILLY_UI_INCLUDE "${SHANTILLY_UI_DIR}/include")
set(SHANTILLY_UI_LIB_DIR "${SHANTILLY_ROOT}/build/lib")

# Importar a Biblioteca SHantilly UI via subdiretório
add_subdirectory("${SHANTILLY_UI_DIR}" "libs/SHantilly-ui")

# Fontes do Studio
set(STUDIO_SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/gui/Canvas.cpp
    src/gui/Canvas.h
    src/gui/ObjectInspector.cpp
    src/gui/ObjectInspector.h
    src/gui/PropertyEditor.cpp
    src/gui/PropertyEditor.h
    src/gui/ActionEditor.cpp
    src/gui/ActionEditor.h
    src/gui/toolbox/AbstractToolbox.cpp
    src/gui/toolbox/AbstractToolbox.h
    src/gui/toolbox/ToolboxClassic.cpp
    src/gui/toolbox/ToolboxClassic.h
    src/gui/toolbox/ToolboxTree.cpp
    src/gui/toolbox/ToolboxTree.h
    src/core/StudioController.cpp
    src/core/StudioController.h
    src/core/ScriptGenerator.cpp
    src/core/ScriptGenerator.h
    src/core/ProjectSerializer.cpp
    src/core/ProjectSerializer.h
    src/core/StudioWidgetFactory.h
    src/core/IStudioWidgetFactory.h
    src/core/PreviewManager.cpp
    src/core/PreviewManager.h)

add_executable(shantilly-studio ${STUDIO_SOURCES})

# Includes
target_include_directories(shantilly-studio
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Linkagem
target_link_libraries(shantilly-studio PRIVATE Qt6::Widgets Qt6::Charts
                                               Qt6::Svg SHantilly-ui)

enable_testing()
add_subdirectory(tests)

# ============================================================================
# Instalação Profissional (GNUInstallDirs e AppImage)
# ============================================================================
include(GNUInstallDirs)

# Instala o binário em usr/bin
install(
  TARGETS shantilly-studio
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  BUNDLE DESTINATION .)

# Instala o ícone SVG (Scalable) para garantir alta definição O comando RENAME
# garante que o ícone tenha o nome esperado pelo .desktop
install(
  FILES packaging/icon/shantilly-studio.svg
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
  RENAME shantilly-studio.svg)

# Instala o arquivo .desktop
install(FILES packaging/appimage/shantilly-studio.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
# ============================================================================
# CPack - Empacotamento DEB
# ============================================================================
set(CPACK_PACKAGE_NAME "shantilly-studio")
set(CPACK_PACKAGE_VENDOR "helton-godoy")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "SHantilly Studio - Visual IDE for SHantilly")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Helton Godoy <helton@example.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libqt6widgets6, libqt6charts6, libqt6svg6, shantilly")
set(CPACK_GENERATOR "DEB")

include(CPack)
