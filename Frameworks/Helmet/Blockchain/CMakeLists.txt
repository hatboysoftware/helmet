cmake_minimum_required(VERSION 3.9)

project (Helmet.Blockchain)

find_package(Helmet REQUIRED)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${HELMET_INCLUDE_DIRS})

include(Utilities)

set (Helmet.Blockchain_HEADERS
        Contracts/I_Contract.hpp
        Contracts/I_ContractPayload.hpp
        Contracts/I_ContractsNode.hpp
        Cryptocurrency/I_CryptocurrencyNode.hpp
        Cryptocurrency/I_CurrencyPayload.hpp
        Cryptocurrency/I_Transaction.hpp
        Configuration.hpp
        I_Block.hpp
        I_Blockchain.hpp
        I_BlockchainNode.hpp
        I_BlockchainPlugin.hpp
        I_BlockPayload.hpp
        I_CompositePayload.hpp
)

string(REGEX REPLACE "([^;]+)" "${HELMET_INCLUDE_DIRS}/Helmet/Blockchain/\\1" Helmet.Blockchain_HEADERS "${Helmet.Blockchain_HEADERS}")

set (Helmet.Blockchain_SRC
        src/Contracts/I_Contract.cpp
        src/Contracts/I_ContractPayload.cpp
        src/Contracts/I_ContractsNode.cpp
        src/Cryptocurrency/I_CryptocurrencyNode.cpp
        src/Cryptocurrency/I_CurrencyPayload.cpp
        src/Cryptocurrency/I_Transaction.cpp
        src/I_Block.cpp
        src/I_Blockchain.cpp
        src/I_BlockchainNode.cpp
        src/I_BlockchainPlugin.cpp
        src/I_BlockPayload.cpp
        src/I_CompositePayload.cpp
)

set (Helmet.Blockchain_DEFINITIONS
        -DHELMET_BLOCKCHAIN_EXPORTS
        -DBOOST_LOG_DYN_LINK)

add_definitions(${Helmet.Blockchain_DEFINITIONS})
add_library(Helmet.Blockchain SHARED ${Helmet.Blockchain_SRC} ${Helmet.Blockchain_HEADERS})
add_dependencies(Helmet.Blockchain Helmet.Core Helmet.Enterprise)
target_link_libraries(Helmet.Blockchain Helmet.Core Helmet.Enterprise ${Boost_LIBRARIES})
set_target_properties (Helmet.Blockchain PROPERTIES INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

#add_subdirectory(test)