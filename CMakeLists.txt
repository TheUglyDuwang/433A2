#Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
add_compile_options(-pthread)
add_compile_options(-g -Wall -Werror -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)

#Enable address sanitizer
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
#What folders to build
add_subdirectory(hal)
add_subdirectory(app)