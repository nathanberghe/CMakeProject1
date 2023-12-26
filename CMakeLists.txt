# CMakeList.txtÂ : projet CMake pour CMakeProject1, incluez la source et dÃ©finissez
# la logique spÃ©cifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
 
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
 
project ("CMakeProject1")
 
 
if(MSVC)
  set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
  set(SFML_STATIC_LIBRARIES TRUE)
  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )
  include_directories("C:/Program Files/SFML/include")
endif()
find_package(SFML 2.6.1 COMPONENTS window system graphics audio REQUIRED)
link_libraries(sfml-graphics sfml-window)
 
# Ajoutez une source Ã  l'exÃ©cutable de ce projet.
add_executable (CMakeProject1 "CMakeProject1.cpp"  )
 
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()
 
# TODO: Ajoutez des tests et installez des cibles si nÃ©cessaire.