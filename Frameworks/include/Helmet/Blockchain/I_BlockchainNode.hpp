//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Blockchain/Configuration.hpp>

#include <Helmet/Core/Utility/I_Visitor.hpp>

#include <Helmet/Enterprise/I_ApplicationService.hpp>
#include <Helmet/Enterprise/I_Peer.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Blockchain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Block;

class HELMET_BLOCKCHAIN_DLL_LINK I_BlockchainNode
:   public Enterprise::I_ApplicationService
{
    /// @name Forward Declarations
    /// @{
public:
    class I_Peer;
    /// @}

    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Peer>       pBlockchainPeer_type;
    /// @}

    /// @name Internal Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class HELMET_BLOCKCHAIN_DLL_LINK I_Peer
    :   public Helmet::Enterprise::I_Peer
    {
        /// @name Types
        /// @{
    public:
        typedef boost::shared_ptr<I_Block>  pBlock_type;
        /// @}

        /// @name I_Peer interface
        /// @{
    public:
        virtual void sendBlock(pBlock_type _pBlock) const = 0;
        virtual void getBlock(boost::uint64_t _index) const = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_Peer();
        virtual ~I_Peer();
        /// @}

    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name I_BlockchainNode interface
    /// @{
public:
    virtual void addPeer(pBlockchainPeer_type _pPeer) = 0;
    virtual void removePeer(pBlockchainPeer_type _pPeer) = 0;
    virtual void getPeers(Core::Utility::I_Visitor<I_Peer>& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BlockchainNode();
    virtual ~I_BlockchainNode();
    /// @}

};  // class I_Node

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Blockchain
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
