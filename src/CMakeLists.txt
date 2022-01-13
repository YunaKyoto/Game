cmake_minimum_required(VERSION 3.11)

include(../cmake/raylib.cmake)

project(game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} "main.cpp" "config.h"  "helper.h" "GameScreen.cpp" "GameScreen.h"  "objects/Object.cpp" "objects/Object.h" "objects/Path.cpp" "objects/Path.h" "objects/Water.cpp" "objects/Water.h" "objects/House.cpp" "objects/House.h" "objects/Hafen.cpp" "objects/Hafen.h" "items/ItemBase.cpp" "items/ItemBase.h" "objects/Pit.cpp" "objects/Pit.h" "items/Key.cpp" "items/Key.h"  "items/Sword.cpp" "items/Sword.h" "items/Heart.cpp" "items/Heart.h" "items/Help.cpp" "items/Help.h" "items/Food.cpp" "items/Food.h" "Inventory.cpp" "Inventory.h" "Char.h" "PlayerChar.cpp" "PlayerChar.h" "items/Equipment.cpp" "items/Equipment.h"  "items/Hat.cpp" "items/Hat.h" "NPC.cpp" "NPC.h")

# Add config.h.in file
configure_file(config.h.in config.h)
target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
        )

#  Link to Raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets to assets path
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../cmake/copy-assets.cmake"
        )
add_dependencies(${PROJECT_NAME} copy_assets)

# CPack instructions
# call e.g. cpack -G ZIP -C Debug to create a binary distribution
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_NAME}_Bundle")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets" DESTINATION "${PROJECT_NAME}_Bundle")