# Notification Editor Demo - ImGui + Zep
# Build with Zep and show notifications alongside editor

IF (BUILD_IMGUI)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)

set(DEMO_NAME ZepNotificationDemo)

add_executable(${DEMO_NAME}
    main.cpp
    ${ZEP_ROOT}/notification_app.cpp
)

target_compile_definitions(${DEMO_NAME} PUBLIC -DZEP_USE_SDL)

target_include_directories(${DEMO_NAME}
    PRIVATE
    ${ZEP_ROOT}/include
    ${FREETYPE_INCLUDE_DIRS}
    ${SDL2_ROOT_DIR}
    ${CMAKE_BINARY_DIR})

target_link_libraries(${DEMO_NAME}
    PRIVATE
    Zep::Zep
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    SDL2::SDL2
    imgui::imgui
    glad::glad
    ${PLATFORM_LINKLIBS})

install(TARGETS ${DEMO_NAME}
    RUNTIME DESTINATION bin)

endif()