project(dragon)

add_executable(dragon dragon.c dragon.h image.c image.h)

target_link_libraries(dragon m)