cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(GleepRemote)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Gleep Remote")
set(VITA_TITLEID  "GLEEP0001")
set(VITA_VERSION  "01.00")

add_executable(GleepRemote main.c)

# Link libraries in the correct order:
# 1. System stubs first (these provide sceGxm, sceDisplay, sceFont, etc.)
# 2. vita2d after the stubs (it depends on them)
# 3. Math library last
target_link_libraries(GleepRemote
    SceCtrl_stub
    SceNet_stub
    SceNetCtl_stub
    SceSysmodule_stub
    SceGxm_stub
    SceDisplay_stub
    ScePgf_stub
    SceLibKernel_stub
)

target_link_libraries(GleepRemote vita2d)

target_link_libraries(GleepRemote m)

vita_create_self(GleepRemote.self GleepRemote UNSAFE)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} GleepRemote.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
)
