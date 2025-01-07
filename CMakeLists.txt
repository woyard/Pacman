cmake_minimum_required(VERSION 3.28)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the path to your Qt5 installation
set(CMAKE_PREFIX_PATH "usr/lib/x86_64-linux-gnu/cmake/Qt5/Qt5Config.cmake")

find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Pacman main.cpp
        Entities/Entity.h
        Entities/Entity.h
        LevelUtilities/LevelFileManager.cpp
        LevelUtilities/LevelFileManager.h
        LevelUtilities/tile_definitions.h
        Entities/MovableEntity.cpp
        Entities/MovableEntity.h
        Entities/MovableEntities/Player.cpp
        Entities/MovableEntities/Player.h
        Entities/MovableEntities/Ghost.cpp
        Entities/MovableEntities/Ghost.h
        Entities/StaticEntities/Pellet.h
        LevelUtilities/LevelData.h
        Entities/StaticEntities/MazeWalls.h
        GameHandler.cpp
        GameHandler.h
        Entities/StaticEntities/Door.h
        Entities/Scoreboard.h
        PathFinder/PathFinder.cpp
        PathFinder/PathFinder.h
        Entities/StaticEntities/Powerup.h
        Entities/MovableEntities/Ghosts/Blinky.h
        Entities/MovableEntities/Ghosts/Clyde.h
        Entities/MovableEntities/Ghosts/Pinky.h
        Entities/MovableEntities/Ghosts/Inky.h
)
target_link_libraries(Pacman
        Qt5::Core
        Qt5::Widgets
)


# Add a custom command to copy all .txt files from levels to the build directory
file(GLOB LEVEL_FILES "${CMAKE_SOURCE_DIR}/levels/*.txt")
foreach (LEVEL_FILE ${LEVEL_FILES})
    add_custom_command(TARGET Pacman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${LEVEL_FILE}
            $<TARGET_FILE_DIR:Pacman>)
endforeach ()


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()