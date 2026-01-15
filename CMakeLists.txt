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
# Permite sobrescrever via linha de comando: -DSHOWBOX_ROOT=/path/to/showbox
set(SHOWBOX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../showbox" CACHE PATH "Path to showbox repository root")

# Localização da Biblioteca Showbox UI
set(SHOWBOX_UI_DIR "${SHOWBOX_ROOT}/libs/showbox-ui")
set(SHOWBOX_UI_INCLUDE "${SHOWBOX_UI_DIR}/include")
set(SHOWBOX_UI_LIB_DIR "${SHOWBOX_ROOT}/build/lib") # Onde o CMake do showbox gera a lib

# Importar a Biblioteca Showbox UI via subdiretório
add_subdirectory("${SHOWBOX_UI_DIR}" "libs/showbox-ui")

# Fontes do Studio
file(GLOB_RECURSE STUDIO_SOURCES 
    "src/*.cpp" 
    "src/*.h"
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