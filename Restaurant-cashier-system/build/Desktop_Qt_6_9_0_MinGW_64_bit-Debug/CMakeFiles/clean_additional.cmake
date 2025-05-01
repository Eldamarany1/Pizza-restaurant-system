# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Restaurant-cashier-system_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Restaurant-cashier-system_autogen.dir\\ParseCache.txt"
  "Restaurant-cashier-system_autogen"
  )
endif()
