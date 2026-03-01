cmake_minimum_required(VERSION 3.14)
project(quill VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# --- Qt5 ---
find_package(Qt5 REQUIRED COMPONENTS Quick Gui Core)

# --- MuPDF ---
# On macOS (Homebrew), mupdf installs to /opt/homebrew/Cellar/mupdf/<version>
# For cross-compilation, override MUPDF_INCLUDE_DIR and MUPDF_LIBRARY
if(NOT MUPDF_INCLUDE_DIR)
    find_path(MUPDF_INCLUDE_DIR mupdf/fitz.h
        HINTS /opt/homebrew/include /usr/local/include /usr/include
    )
endif()

if(NOT MUPDF_LIBRARY)
    find_library(MUPDF_LIBRARY NAMES mupdf
        HINTS /opt/homebrew/lib /usr/local/lib /usr/lib
    )
endif()

if(NOT MUPDF_THIRD_LIBRARY)
    find_library(MUPDF_THIRD_LIBRARY NAMES mupdf-third
        HINTS /opt/homebrew/lib /usr/local/lib /usr/lib
    )
endif()

if(NOT MUPDF_INCLUDE_DIR OR NOT MUPDF_LIBRARY)
    message(FATAL_ERROR "MuPDF not found. Set MUPDF_INCLUDE_DIR and MUPDF_LIBRARY.")
endif()

message(STATUS "MuPDF include: ${MUPDF_INCLUDE_DIR}")
message(STATUS "MuPDF library: ${MUPDF_LIBRARY}")
if(MUPDF_THIRD_LIBRARY)
    message(STATUS "MuPDF third-party library: ${MUPDF_THIRD_LIBRARY}")
endif()

# --- Sources ---
set(SOURCES
    src/main.cpp
    src/pdfrenderer.cpp
    src/pdfrenderer.h
    src/pageimageproviderplugin.cpp
    src/pageimageproviderplugin.h
)

# --- QML Resources ---
qt5_add_resources(QML_RESOURCES qml/qml.qrc)

# --- Executable ---
add_executable(quill ${SOURCES} ${QML_RESOURCES})

target_include_directories(quill PRIVATE
    ${MUPDF_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(quill PRIVATE
    Qt5::Quick
    Qt5::Gui
    Qt5::Core
    ${MUPDF_LIBRARY}
)

if(MUPDF_THIRD_LIBRARY)
    target_link_libraries(quill PRIVATE ${MUPDF_THIRD_LIBRARY})
endif()

# On macOS, link system frameworks needed by MuPDF
if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(SECURITY_LIBRARY Security)
    if(COREFOUNDATION_LIBRARY)
        target_link_libraries(quill PRIVATE ${COREFOUNDATION_LIBRARY})
    endif()
    if(SECURITY_LIBRARY)
        target_link_libraries(quill PRIVATE ${SECURITY_LIBRARY})
    endif()
endif()

# --- Install ---
install(TARGETS quill DESTINATION bin)
