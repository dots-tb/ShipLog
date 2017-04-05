cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(shipLog_net)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
  add_definitions(-DENABLE_LOGGING)
endif()

add_executable(${PROJECT_NAME}
  shipLog_net.c
  vita_netdbg.c
  ../common/settings.c
  ../common/parser.c
)

target_link_libraries(${PROJECT_NAME}
  taihen_stub
  SceDisplay_stub
  SceSysmodule_stub
  SceGxm_stub
  SceSysmem_stub
  SceNet_stub
  SceNetCtl_stub
  SceIofilemgr_stub
  SceProcessmgr_stub
  SceLibKernel_stub
  SceKernelThreadMgr_stub
  SceKernelModulemgr_stub
  backdoor_exe_stub
  SceLibc_stub

)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(${PROJECT_NAME}.suprx ${PROJECT_NAME}
  UNSAFE
  CONFIG ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.yml
)