cmake_minimum_required(VERSION 3.10)

# Tell CMake where VitaSDK lives (GitHub sets this env var)
if(DEFINED ENV{VITASDK})
  set(VITASDK $ENV{VITASDK})
else()
  message(FATAL_ERROR "VITASDK environment variable not set!")
endif()

set(CMAKE_TOOLCHAIN_FILE "${VITASDK}/share/vita.toolchain.cmake")
project(GleepRemote)

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Gleep Remote")
set(VITA_TITLEID  "GLEEP0001")
set(VITA_VERSION  "01.00")

add_executable(GleepRemote main.c)

# We use the exact naming convention for the Docker image's libraries
target_link_libraries(GleepRemote
    vita2d
    SceCtrl_stub
    SceNet_stub
    SceNetCtl_stub
    SceSysmodule_stub
    SceKernelThreadMgr_stub
    SceKernelModulemgr_stub
    SceKernelThreadMgr_stub
    SceDisplay_stub
)

vita_create_self(GleepRemote.self GleepRemote UNSAFE)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} GleepRemote.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
)
