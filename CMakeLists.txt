cmake_minimum_required(VERSION 2.4)
PROJECT(Player)
#SET (CMAKE_BUILD_TYPE Release)
SET (CMAKE_BUILD_TYPE Debug)

#This should be done by Find

# /usr/local/include/Poco
SET(Poco_INCLUDE_DIR /home/olas/poco-1.6.0)

SET(Poco_INCLUDE_DIRS
    poco/Net/include
    ${Poco_INCLUDE_DIR}/Foundation/include
    ${Poco_INCLUDE_DIR}/Net/include
    ${Poco_INCLUDE_DIR}/Util/include
    ${Poco_INCLUDE_DIR}/XML/include
    ${Poco_INCLUDE_DIR}/Zip/include
    ${Poco_INCLUDE_DIR}/JSON/include
)

#/usr/local/lib
SET(Poco_LIBRARY_DIR /usr/local/lib)

SET(Poco_LIBRARIES
    ${Poco_LIBRARY_DIR}/libPocoJSONd.a
    ${Poco_LIBRARY_DIR}/libPocoNetd.a
    ${Poco_LIBRARY_DIR}/libPocoUtild.a
    ${Poco_LIBRARY_DIR}/libPocoFoundationd.a
    ${Poco_LIBRARY_DIR}/libPocoXMLd.a
)

#    ${Poco_LIBRARY_DIR}/libPocoDatad.a
#    ${Poco_LIBRARY_DIR}/libPocoCrypto.a


INCLUDE_DIRECTORIES(
${Poco_INCLUDE_DIRS}
src
)


IF(WIN32)
ELSE(WIN32)
set(CMAKE_CXX_FLAGS "-g -Wall")
ENDIF(WIN32)




SET(websock_SRCS
#../evr/source/evrgen.c
webmain.cpp
src/config.cpp
src/config.h
src/server.cpp
src/server.h
)



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

ADD_EXECUTABLE(
websock
${websock_SRCS}
)

#
# PocoNetd PocoJSONd PocoUtild  PocoXMLd   PocoFoundationd
TARGET_LINK_LIBRARIES(websock pthread PocoUtild PocoNetd PocoXMLd PocoJSONd   PocoFoundationd  rt pthread)



#SET_TARGET_PROPERTIES (utest_idp PROPERTIES LINK_FLAGS "-z muldefs")

#set(CMAKE_C_FLAGS "-g -Wall -lstdc++")

