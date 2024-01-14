cmake_minimum_required(VERSION 3.0.0)
project(bongcloud-engine VERSION 0.1.0 LANGUAGES C CXX)

INCLUDE_DIRECTORIES(
	D:/robert2021/tools/vcpkg-2023.12.12/installed/x64-windows/include/
)
  
LINK_DIRECTORIES(
	D:/robert2021/tools/vcpkg-2023.12.12/installed/x64-windows/lib/
)
add_executable(bongcloud-engine main.cpp)
TARGET_LINK_LIBRARIES(bongcloud-engine
	cpr curl
)
