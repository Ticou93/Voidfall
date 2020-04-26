CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

FILE(GLOB headers *.h)
FILE(GLOB cpps *.cpp)

SET(source_files ${headers} ${cpps})
SOURCE_GROUP(Voidfall FILES ${source_files})

ADD_EXECUTABLE(Voidfall ${source_files})
TARGET_LINK_LIBRARIES(Voidfall deps)

