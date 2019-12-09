/**
 * @file	ContactMessage.hpp
 * @brief	Contact
 * @details
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_SDK_JNI_CONTACT_MESSAGE_HPP_
#define _ELASTOS_SDK_JNI_CONTACT_MESSAGE_HPP_

#include <string>
#include "CrossBase.hpp"
#include <MessageManager.hpp>
#include "experimental-span.hpp"

#ifdef WITH_CROSSPL
namespace crosspl {
namespace native {
#endif // WITH_CROSSPL

class ContactMessage : public CrossBase {
public:
    /*** type define ***/
    using Type = elastos::MessageManager::MessageType;

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit ContactMessage();
    virtual ~ContactMessage();

    int syncMessageToNative(int type,
                            const std::span<uint8_t>* data,
                            const char* cryptoAlgorithm,
                            int64_t timestamp);

    std::shared_ptr<elastos::MessageManager::MessageInfo> mMessageInfo;

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
}; // class Contact

#ifdef WITH_CROSSPL
} //namespace native
} //namespace crosspl
#endif // WITH_CROSSPL

#endif /* _ELASTOS_SDK_JNI_CONTACT_MESSAGE_HPP_ */

