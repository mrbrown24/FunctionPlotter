# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FunctionPlotter_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FunctionPlotter_autogen.dir\\ParseCache.txt"
  "FunctionPlotter_autogen"
  )
endif()
