cmake_minimum_required(VERSION 3.5)
project(DockMode)

include("$ENV{VITASDK}/share/vita.cmake")

add_executable(DockMode main.c)

target_link_libraries(DockMode 
    vita2d png z m
    SceGxm_stub SceDisplay_stub SceSysmodule_stub SceCtrl_stub 
    ScePower_stub SceRtc_stub SceCommonDialog_stub ScePvf_stub
    SceAppMgr_stub ScePgf_stub SceSharedFb_stub_weak freetype
)

# This creates the .velf file
vita_create_self(DockMode.self DockMode DOCK00001)
