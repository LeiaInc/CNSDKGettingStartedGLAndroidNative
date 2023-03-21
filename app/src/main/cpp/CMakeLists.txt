# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("cnsdkgettingstartedglandroidnative")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(cnsdkgettingstartedglandroidnative SHARED native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        cnsdkgettingstartedglandroidnative
        PRIVATE

        # link to CNSDK library
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../CNSDK/libs/native/arm64-v8a/libleiaSDK.so"

        # Link to logging library
        ${log-lib})

# Set Leia CNSDK include directories.
target_include_directories(
        cnsdkgettingstartedglandroidnative
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../CNSDK/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../CNSDK/include/third_party")
