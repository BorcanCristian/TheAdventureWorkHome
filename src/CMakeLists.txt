add_executable(
    ${PROJECT_ID}
    animated_sprite.h
    animated_sprite.cpp
    codes.h
    events.h
    game.h
    game.cpp
    hero.h
    hero.cpp
    i_aggravatable.cpp
    i_aggravatable.h
    i_attacker.h
    i_collidable.h
    i_collidable.cpp
    i_destroyable.h
    i_input_handler.h
    i_renderable.h
    i_thing.h
    i_thing.cpp
    main.cpp
    map.h
    map.cpp
    object.h
    object.cpp
    renderer.h
    renderer.cpp
    slime.cpp
    slime.h
    sound.h
    sound.cpp
    sprite.h
    sprite.cpp
)

set_property (TARGET ${PROJECT_ID} PROPERTY CXX_STANDARD 17)
set_property (TARGET ${PROJECT_ID} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property (TARGET ${PROJECT_ID} PROPERTY CXX_EXTENSIONS OFF)

target_sources(
    ${PROJECT_ID}
    PRIVATE
)

target_link_libraries(
    ${PROJECT_ID}
    PRIVATE
        fmt::fmt
        SDL::SDL2
        SDL::SDL_mixer
        stb::stb
)

target_compile_definitions(
    ${PROJECT_ID}
    PRIVATE
        -DPROJECT_NAME="${PROJECT_NAME}"
)

add_custom_command(
    TARGET
        ${PROJECT_ID}
    POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/dist $<TARGET_FILE_DIR:${PROJECT_ID}>/dist
)
