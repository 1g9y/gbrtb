cmake_minimum_required(VERSION 3.10)

# --- VITASDK DETECTION ---
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(GleepRemote)

# Include the Vita macros (this is the line that was failing)
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

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
