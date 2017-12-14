project(BigNumber)

SET(SOURCES src/BigNumber.cpp src/main.cpp)
SET(HEADERS src/BigNumber.h)


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})