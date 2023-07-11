cmake_minimum_required(VERSION 3.18.1)

project(cnsdkgettingstartedglandroidnative)

add_library(cnsdkgettingstartedglandroidnative SHARED native-lib.cpp)

find_package(CNSDK CONFIG REQUIRED)

target_link_libraries(cnsdkgettingstartedglandroidnative PRIVATE CNSDK::leiaSDK)
