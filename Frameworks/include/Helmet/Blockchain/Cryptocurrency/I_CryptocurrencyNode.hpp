//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Blockchain/I_BlockchainNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Blockchain {
namespace Cryptocurrency {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Transaction;

class HELMET_BLOCKCHAIN_DLL_LINK I_CryptocurrencyNode
:   public I_BlockchainNode
{
    /// @name Forward Declarations
    /// @{
public:
    class I_Peer;
    /// @}

    /// @name Type
    /// @{
public:
    typedef boost::shared_ptr<I_Transaction>    pTransaction_type;
    typedef boost::shared_ptr<I_Peer>           pCryptocurrencyPeer_type;
    /// @}

    /// @name Internal Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class HELMET_BLOCKCHAIN_DLL_LINK I_Peer
    :   public Helmet::Blockchain::I_BlockchainNode::I_Peer
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_Peer interface
        /// @{
    public:
        virtual void sendTransaction(pTransaction_type _pTransaction) const = 0;
        virtual void getConfirmation(pTransaction_type _pTransaction) const = 0;
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

    /// @name I_CryptocurrencyNode interface
    /// @{
public:
    virtual void addTransaction(pTransaction_type _pTransaction) = 0;
    virtual void getTransactions(Core::Utility::I_Visitor<I_Transaction>& _transaction) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CryptocurrencyNode();
    virtual ~I_CryptocurrencyNode();
    /// @}

};  // interface I_CryptocurrencyNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Cryptocurrency
}   // namespace Blockchain
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
