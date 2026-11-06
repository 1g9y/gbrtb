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

# This specific order and syntax is required by the VitaSDK Docker container.
target_link_libraries(GleepRemote
    SceCtrl_stub
    SceNet_stub
    SceNetCtl_stub
    SceSysmodule_stub
    SceDisplay_stub
    SceGxm_stub
    ScePgf_stub
    vita2d
    SceKernelThreadMgr
    SceKernelModulemgr
    SceAppMgr_stub
    SceCommonDialog_stub
    SceLibKernel_stub
    SceIofilemgr_stub
    SceSysmem_stub
    SceThreadmgr_stub
    SceProcessmgr_stub
    m
)

vita_create_self(GleepRemote.self GleepRemote UNSAFE)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} GleepRemote.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
)
