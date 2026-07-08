cmake_minimum_required(VERSION 3.20)
project(realtime_event_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Зависимости ---
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

# =========================================================
# Основная библиотека (всё, кроме main.cpp) —
# так main.cpp можно линковать и в server.exe, и переиспользовать
# в тестах без дублирования исходников
# =========================================================
set(CORE_SOURCES
    src/core/logger.cpp
    src/core/thread_pool.cpp

    src/domain/auth_service.cpp
    src/domain/session_registry.cpp

    src/network/framing.cpp
    src/network/server.cpp
    src/network/session.cpp

    src/pipeline/event_router.cpp
    src/pipeline/handlers/auth_handler.cpp
    src/pipeline/handlers/broadcast_handler.cpp
    src/pipeline/handlers/state_handler.cpp

    src/storage/async_writer.cpp
    src/storage/file_storage.cpp
)

add_library(engine_core STATIC ${CORE_SOURCES})

target_include_directories(engine_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(engine_core
    PUBLIC
        Boost::system
        Threads::Threads
)

if (MSVC)
    target_compile_options(engine_core PRIVATE /W4)
else()
    target_compile_options(engine_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================================================
# Исполняемый файл сервера
# =========================================================
add_executable(server src/main.cpp)
target_link_libraries(server PRIVATE engine_core)

# =========================================================
# Тесты (опционально, включаются флагом)
# =========================================================
option(BUILD_TESTS "Build unit tests" OFF)

if (BUILD_TESTS)
    enable_testing()

    add_executable(unit_tests
        tests/test_concurrent_queue.cpp
        tests/test_thread_pool.cpp
        tests/test_event_router.cpp
        tests/test_session_registry.cpp
    )

    target_link_libraries(unit_tests PRIVATE engine_core)
    add_test(NAME unit_tests COMMAND unit_tests)
endif()

# =========================================================
# Бенчмарки (опционально)
# =========================================================
option(BUILD_BENCHMARKS "Build benchmarks" OFF)

if (BUILD_BENCHMARKS)
    add_executable(throughput_bench benchmarks/throughput_bench.cpp)
    target_link_libraries(throughput_bench PRIVATE engine_core)
endif()
