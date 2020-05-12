CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

ADD_EXECUTABLE(Voidfall
    main.cpp
    Audio.cpp
    WindowManager.cpp
    ECS/Systems/RenderSystem.cpp
    tools/shaderHandler.h
    #debug.h
    CircularFIFO.h
    TaskSystem.h
    TaskSystem.cpp)
TARGET_LINK_LIBRARIES(Voidfall deps)

