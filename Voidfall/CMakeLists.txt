CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

ADD_EXECUTABLE(Voidfall
    main.cpp
    Audio.cpp
    WindowManager.cpp
    ECS/Systems/RenderSystem.cpp
    tools/shaderHandler.h)
TARGET_LINK_LIBRARIES(Voidfall deps)

