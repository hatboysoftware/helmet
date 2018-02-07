cmake_minimum_required(VERSION 3.9)

project (Helmet.Enterprise)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Enterprise_HEADERS
        Configuration.hpp
        Exceptions.hpp
        I_AbstractNetworkAddressFactory.hpp
        I_AbstractNetworkServiceFactory.hpp
        I_Address.hpp
        I_ApplicationServer.hpp
        I_ApplicationServerManager.hpp
        I_ApplicationService.hpp
        I_ApplicationServiceFactory.hpp
        I_Connection.hpp
        I_Container.hpp
        I_Endpoint.hpp
        I_FullyQualifiedResourceLocation.hpp
        I_Message.hpp
        I_MessageFactory.hpp
        I_MessageHandler.hpp
        I_MessageHeader.hpp
        I_MessageRegistry.hpp
        I_MessageType.hpp
        I_NetworkAddress.hpp
        I_NetworkAddressFactory.hpp
        I_NetworkConnection.hpp
        I_NetworkConnectionListener.hpp
        I_NetworkManager.hpp
        I_NetworkReadReadyListener.hpp
        I_NetworkService.hpp
        I_NetworkServiceFactory.hpp
        I_NetworkWriteReadyListener.hpp
        I_NumericTypeMessageRegistry.hpp
        I_ProtocolManager.hpp
        I_ProtocolService.hpp
        I_ProtocolServiceFactory.hpp
        I_Reactor.hpp
        I_Request.hpp
        I_RequestHandler.hpp
        I_ResourceLocation.hpp
        I_Response.hpp
        I_ResponseHandler.hpp
        I_StartupShutdownParticipant.hpp
        I_SessionEvent.hpp
        I_TimeoutHandler.hpp
        )

string(REGEX REPLACE "([^;]+)" "${HELMET_INCLUDE_DIRS}/Helmet/Enterprise/\\1" Helmet.Enterprise_HEADERS "${Helmet.Enterprise_HEADERS}")

set (Helmet.Enterprise_SRC
        src/AbstractNetworkAddressFactory.cpp
        src/AbstractNetworkAddressFactory.hpp
        src/ApplicationServer.cpp
        src/ApplicationServer.hpp
        src/ApplicationServerManager.cpp
        src/ApplicationServerManager.hpp
        src/Container.cpp
        src/Container.hpp
        src/FullyQualifiedResourceLocation.cpp
        src/FullyQualifiedResourceLocation.hpp
        src/I_AbstractNetworkAddressFactory.cpp
        src/I_Address.cpp
        src/I_ApplicationServer.cpp
        src/I_ApplicationServerManager.cpp
        src/I_ApplicationService.cpp
        src/I_ApplicationServiceFactory.cpp
        src/I_Connection.cpp
        src/I_Container.cpp
        src/I_Endpoint.cpp
        src/I_FullyQualifiedResourceLocation.cpp
        src/I_Message.cpp
        src/I_MessageFactory.cpp
        src/I_MessageHandler.cpp
        src/I_MessageHeader.cpp
        src/I_MessageRegistry.cpp
        src/I_MessageType.cpp
        src/I_NetworkAddress.cpp
        src/I_NetworkAddressFactory.cpp
        src/I_NetworkConnection.cpp
        src/I_NetworkManager.cpp
        src/I_NetworkService.cpp
        src/I_NetworkServiceFactory.cpp
        src/I_NumericTypeMessageRegistry.cpp
        src/I_ProtocolManager.cpp
        src/I_ProtocolService.cpp
        src/I_ProtocolServiceFactory.cpp
        src/I_Request.cpp
        src/I_RequestHandler.cpp
        src/I_ResourceLocation.cpp
        src/I_Response.cpp
        src/I_ResponseHandler.cpp
        src/I_StartupShutdownParticipant.cpp
        src/I_TimeoutHandler.cpp
        src/NetworkManager.cpp
        src/NetworkManager.hpp
        src/NumericTypeMessageRegistry.cpp
        src/NumericTypeMessageRegistry.hpp
        src/QueuedRequestHandler.hpp
        src/ResourceLocation.cpp
        src/ResourceLocation.hpp
        )

set (Helmet.Enterprise_DEFINITIONS
        -DHELMET_ENTERPRISE_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Enterprise_DEFINITIONS})
add_library(Helmet.Enterprise SHARED ${Helmet.Enterprise_SRC} ${Helmet.Enterprise_HEADERS})
add_dependencies(Helmet.Enterprise Helmet.Core)
target_link_libraries(Helmet.Enterprise Helmet.Core ${Boost_LIBRARIES})
set_target_properties (Helmet.Enterprise PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
