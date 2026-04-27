cmake_minimum_required(VERSION 3.19)
project(Worksheet6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)
qt_standard_project_setup()

# Point to the newly built VTK (Qt 6.10.2 compatible)
set(VTK_DIR "C:/Program Files (x86)/VTK/lib/cmake/vtk-9.6")

find_package(VTK REQUIRED COMPONENTS
  CommonCore
  CommonDataModel
  FiltersSources
  FiltersGeneral
  InteractionStyle
  RenderingCore
  RenderingOpenGL2
  RenderingQt
  GUISupportQt
  IOGeometry
  IOLegacy
)

qt_add_executable(Worksheet6
  WIN32
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  ModelPart.cpp
  ModelPart.h
  ModelPartList.cpp
  ModelPartList.h
  optiondialog.cpp
  optiondialog.h
  optiondialog.ui
  resources.qrc
)

target_link_libraries(Worksheet6 PRIVATE
  Qt6::Core
  Qt6::Widgets
  Qt6::OpenGLWidgets
  ${VTK_LIBRARIES}
)

vtk_module_autoinit(
  TARGETS Worksheet6
  MODULES ${VTK_LIBRARIES}
)

# Copy VR bindings
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E
  copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/)
