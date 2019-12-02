/**
 * @file	ContactDataListener.hpp
 * @brief	Contact
 * @details
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_SDK_JNI_CONTACT_DATA_LISTENER_HPP_
#define _ELASTOS_SDK_JNI_CONTACT_DATA_LISTENER_HPP_

#include <mutex>

#include "CrossBase.hpp"
#include <Elastos.SDK.Contact.hpp>
#include <ContactListener.hpp>

#ifdef WITH_CROSSPL
namespace crosspl {
namespace native {
#endif // WITH_CROSSPL

class ContactDataListener : public CrossBase {
public:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit ContactDataListener();
    virtual ~ContactDataListener();

#ifdef WITH_CROSSPL
    void onNotify(const std::string& humanCode, ContactChannel channelType,
                  const std::string& dataId, int status);
    std::shared_ptr<std::span<uint8_t>> onReadData(const std::string& humanCode, ContactChannel channelType,
                                                   const std::string& dataId, uint64_t offset);
    int onWriteData(const std::string& humanCode, ContactChannel channelType,
                    const std::string& dataId, uint64_t offset, const std::span<uint8_t>* data);
#else
    virtual void onNotify(const std::string& humanCode, ContactChannel channelType,
                          const std::string& dataId, int status) = 0;
    virtual int onReadData(const std::string& humanCode, ContactChannel channelType,
                           const std::string& dataId, uint64_t offset,
                           std::vector<uint8_t>& data) = 0;
    virtual int onWriteData(const std::string& humanCode, ContactChannel channelType,
                            const std::string& dataId, uint64_t offset,
                            const std::vector<uint8_t>& data) = 0;
#endif // WITH_CROSSPL

    std::shared_ptr<elastos::MessageManager::DataListener> getDataListener();

private:
    /*** type define ***/

    /*** static function and variable ***/
    static ContactDataListener* sContactDataListenerInstance;

    /*** class function and variable ***/
    std::shared_ptr<elastos::MessageManager::DataListener> makeDataListener();

    std::shared_ptr<elastos::MessageManager::DataListener> mDataListener;
}; // class Contact

#ifdef WITH_CROSSPL
} //namespace native
} //namespace crosspl
#endif // WITH_CROSSPL

#endif /* _ELASTOS_SDK_JNI_CONTACT_DATA_LISTENER_HPP_ */
