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

#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Blockchain {
namespace Cryptocurrency {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_BLOCKCHAIN_DLL_LINK I_Transaction
{
    /// @name Types
    /// @{
public:
    typedef enum {
        REGULAR,
        FEE,
        REWARD
    } TransactionType_enum;
    /// @}

    /// @name Internal structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class HELMET_BLOCKCHAIN_DLL_LINK I_Recipient
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_Recipient interface
        /// @{
    public:
        virtual boost::uint64_t getAmount() const = 0;
        virtual const std::string& getAddress() const = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_Recipient();
        virtual ~I_Recipient();
        /// @}

    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name I_Transaction interface
    /// @{
public:
    virtual const std::string& getId() const = 0;
    virtual const std::string& getHash() const = 0;
    virtual TransactionType_enum getType() const = 0;
    virtual const std::string& getPreviousHash() const = 0;
    virtual boost::uint64_t getPreviousIndex() const = 0;
    virtual boost::uint64_t getTotalAmount() const = 0;
    virtual const std::string& getOriginAddress() const = 0;
    virtual const std::string& getOriginHash() const = 0;
    virtual void getRecipients(Core::Utility::I_Visitor<I_Recipient>& _visitor) const = 0;

    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Transaction();
    virtual ~I_Transaction();
    /// @}

};  // interface I_Transaction

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Cryptocurrency
}   // namespace Blockchain
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
