cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(glm)
enable_testing()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if(CMAKE_COMPILER_IS_GNUCXX)
	#add_definitions(/Za)
	#add_definitions(-pedantic)
	#add_definitions(-S)
	#add_definitions(-s)
	#add_definitions(-msse2)
	#add_definitions(-std=c++0x)
	#add_definitions(-fms-extensions)
	#add_definitions(-D_MSC_EXTENSIONS)
	#add_definitions(-m32)
	#add_definitions(-mfpmath=387)
	#add_definitions(-ffast-math)
	#add_definitions(-O3)
    
    #add_definitions(-fprofile-arcs -ftest-coverage) gcov
    #ctest_enable_coverage()
endif()

include_directories(".")
include_directories("./test/external")

add_subdirectory(glm)
#add_subdirectory(test) # @rlyeh +comment
#add_subdirectory(bench) # @rlyeh +comment
#add_subdirectory(doc) # @rlyeh +comment

option(GLM_TEST_MODE "GLM test" ON) # @rlyeh -OFF +ON
if(NOT GLM_TEST_MODE)
	message(FATAL_ERROR "GLM is a header only library, no need to build it. Set the option GLM_TEST_MODE with ON to build and run the test bench")
endif()

install(DIRECTORY glm DESTINATION include)
