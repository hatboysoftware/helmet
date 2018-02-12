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
namespace Contracts {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Contract;

class HELMET_BLOCKCHAIN_DLL_LINK I_ContractsNode
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
    typedef boost::shared_ptr<I_Contract>    pContract_type;
    typedef boost::shared_ptr<I_Peer>        pContractsPeer_type;
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
        virtual void sendContract(pContract_type _pContract) const = 0;
        virtual void getConfirmation(pContract_type _pContract) const = 0;
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

    /// @name I_ContractsNode interface
    /// @{
public:
    virtual void addContract(pContract_type _pContract) = 0;
    virtual void getContracts(Core::Utility::I_Visitor<I_Contract>& _contract) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ContractsNode();
    virtual ~I_ContractsNode();
    /// @}

};  // interface I_ContractsNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Contracts
}   // namespace Blockchain
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
