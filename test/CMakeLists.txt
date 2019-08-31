project(rgbdriverv2Test)

set(LIB_MOCK_SRC
	${CMAKE_SOURCE_DIR}/mock/Serial.cpp
	${CMAKE_SOURCE_DIR}/mock/String.cpp
)

set(LIB_MOCK_HEADERS
	${CMAKE_SOURCE_DIR}/mock/Arduino.h
)

set(LIB_RGBDRIVER_SRC
	${LIB_MOCK_SRC}
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/actor.cpp
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/deltalimiter.cpp
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/deltatimer.cpp
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/framerate.cpp
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/neoanimation.cpp
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/neopatterns.cpp
)

set(LIB_RGBDRIVER_HEADERS
	${LIB_MOCK_HEADERS}
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/typedefinitions.h
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/actor.h
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/deltalimiter.h
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/deltatimer.h
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/framerate.h
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/neoanimation.h
	${CMAKE_SOURCE_DIR}/src/rgbdriverv2/neopatterns.h
)

add_library(RGBunit SHARED ${LIB_RGBDRIVER_SRC} ${LIB_RGBDRIVER_HEADERS})
set_target_properties(RGBunit PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${CMAKE_SOURCE_DIR}/mock)
include_directories(${CMAKE_SOURCE_DIR}/src/rgbdriverv2)


set(TEST_DELTALIMITER_SOURCE 
	testdeltalimiter.cpp
)
add_executable(testdeltalimiter ${TEST_DELTALIMITER_SOURCE})
target_link_libraries(
	testdeltalimiter
	RGBunit
)
add_test(TestDeltalimiter testdeltalimiter)



set(TEST_DELTATIMER_SOURCE 
	testdeltatimer.cpp
)
add_executable(testdeltatimer ${TEST_DELTATIMER_SOURCE})
target_link_libraries(
	testdeltatimer
	RGBunit
)
add_test(TestDeltaTimer testdeltatimer)



set(TEST_FRAMERATE_SOURCE 
	testframerate.cpp
)
add_executable(testframerate ${TEST_FRAMERATE_SOURCE})
target_link_libraries(
	testframerate
	RGBunit
)
add_test(TestFrameRate testframerate)



set(TEST_NEOPATTERNS_SOURCE 
	testneopatterns.cpp
)
add_executable(testneopatterns ${TEST_NEOPATTERNS_SOURCE})
target_link_libraries(
	testneopatterns
	RGBunit
)
add_test(TestNeoPatterns testneopatterns)