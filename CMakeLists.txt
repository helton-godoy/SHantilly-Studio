cmake_minimum_required(VERSION 3.16)

project(showbox-studio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuração Automática do Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Svg)

# Configuração do caminho do Showbox Core
if(NOT SHOWBOX_ROOT)
    get_filename_component(SHOWBOX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../showbox" ABSOLUTE)
endif()
message(STATUS "Using SHOWBOX_ROOT: ${SHOWBOX_ROOT}")

# Localização da Biblioteca Showbox UI
set(SHOWBOX_UI_DIR "${SHOWBOX_ROOT}/libs/showbox-ui")
set(SHOWBOX_UI_INCLUDE "${SHOWBOX_UI_DIR}/include")
set(SHOWBOX_UI_LIB_DIR "${SHOWBOX_ROOT}/build/lib")

# Importar a Biblioteca Showbox UI via subdiretório
add_subdirectory("${SHOWBOX_UI_DIR}" "libs/showbox-ui")

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
    src/core/StudioController.cpp
    src/core/StudioController.h
    src/core/ScriptGenerator.cpp
    src/core/ScriptGenerator.h
    src/core/ProjectSerializer.cpp
    src/core/ProjectSerializer.h
    src/core/StudioWidgetFactory.h
    src/core/IStudioWidgetFactory.h
)

add_executable(showbox-studio 
    ${STUDIO_SOURCES}
)

# Includes
target_include_directories(showbox-studio PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Linkagem
# Agora linkamos o TARGET da biblioteca, o que resolve as dependências do Qt automaticamente
target_link_libraries(showbox-studio PRIVATE 
    Qt6::Widgets 
    Qt6::Charts
    Qt6::Svg
    showbox-ui
)

enable_testing()
add_subdirectory(tests)