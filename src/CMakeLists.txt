find_package(LLVM REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
  Error.cpp
  sqlgen.cpp
  SQLEmitter.cpp
  SQLInsertEmitter.cpp
  SQLQueryEmitter.cpp
  SQLTableEmitter.cpp)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  myproject::myproject_options
  myproject::myproject_warnings)

target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIBS support tablegen)


target_link_system_libraries(
  ${PROJECT_NAME}
  PRIVATE
  ${LLVM_LIBS}
  fmt::fmt
  spdlog::spdlog)
