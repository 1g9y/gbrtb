cmake_minimum_required(VERSION 3.10)
project(GleepRemote)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "Gleep Remote")
set(VITA_TITLEID  "GLEEP0001")
set(VITA_VERSION  "01.00")
add_executable(GleepRemote main.c)
target_link_libraries(GleepRemote vita2d SceCtrl SceNet SceNetCtl SceKernel SceSysmodule)
vita_create_self(GleepRemote.self GleepRemote UNSAFE)
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} GleepRemote.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
)