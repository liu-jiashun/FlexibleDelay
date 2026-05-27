cmake_minimum_required(VERSION 3.10)

project(FlexibleDelayTest)

set(CMAKE_C_STANDARD 99)

# 添加上级目录的源文件
set(FLEXIBLE_DELAY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../flexible_delay.c
)

set(TEST_SOURCES
    main.c
)

# 创建库
add_library(flexible_delay STATIC ${FLEXIBLE_DELAY_SOURCES})
target_include_directories(flexible_delay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# 创建可执行文件
add_executable(flexible_delay_test ${TEST_SOURCES})
target_link_libraries(flexible_delay_test flexible_delay)
target_include_directories(flexible_delay_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)