cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mandelbrot VERSION 1.0)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i686")
    set(ARCH_FLAGS "-mavx2 -march=native -mtune=native")
else()
    set(ARCH_FLAGS "-march=native -mtune=native")
endif()

set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -D NDEBUG ${ARCH_FLAGS} -Ofast")
set(CMAKE_CXX_LINKER_RELEASE "${CMAKE_CXX_LINKER_RELEASE} -D NDEBUG ${ARCH_FLAGS} -Ofast")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG -no-pie -ggdb3 -std=c++20 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE ${ARCH_FLAGS} -O0")
    set(CMAKE_CXX_LINKER_DEBUG "${CMAKE_CXX_LINKER_DEBUG} -D _DEBUG -no-pie -ggdb3 -std=c++20 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE ${ARCH_FLAGS} -O0")
else()
    # Clang (including Apple Clang on macOS) - omit GCC-only warnings and Linux-specific flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D _DEBUG -ggdb3 -std=c++20 -Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fsized-deallocation -fstack-protector -fno-omit-frame-pointer ${ARCH_FLAGS} -O0")
    set(CMAKE_CXX_LINKER_DEBUG "${CMAKE_CXX_LINKER_DEBUG} -D _DEBUG ${ARCH_FLAGS} -O0")
endif()

# Copy runtime dependencies
file(COPY res DESTINATION "${CMAKE_BINARY_DIR}")

# add_compile_definitions(GL_LOG_CALLS)
add_subdirectory(lib/gl)

add_subdirectory(lib/imgui)

add_subdirectory(lib/gl-imgui)

add_subdirectory(src)
