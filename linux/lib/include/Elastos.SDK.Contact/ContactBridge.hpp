/**
 * @file	Contact.hpp
 * @brief	Contact
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_SDK_JNI_CONTACT_BRIDGE_HPP_
#define _ELASTOS_SDK_JNI_CONTACT_BRIDGE_HPP_

#include <sstream>

#include <ContactListener.hpp>
#include <ContactDataListener.hpp>
#include "CrossBase.hpp"
#include <Elastos.SDK.Contact.hpp>

#ifdef WITH_CROSSPL
namespace crosspl {
namespace native {
#endif // WITH_CROSSPL

class ContactBridge {
public:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit ContactBridge();
    virtual ~ContactBridge();

#ifdef WITH_CROSSPL
    void setListener(CrossBase* listener);
    void setDataListener(CrossBase* listener);
#else
    void setListener(ContactListener* listener);
    void setDataListener(ContactDataListener* listener);
#endif // WITH_CROSSPL
    int start();
    int stop();

    int setIdentifyCode(int type, const char* value);

    int setHumanInfo(const std::string& humanCode, int item, const std::string& value);
    int getHumanInfo(const std::string& humanCode, std::stringstream* info);
    int getHumanStatus(const char* humanCode);

    int addFriend(const std::string& friendCode, const std::string& summary);
    int removeFriend(const std::string& friendCode);
    int acceptFriend(const std::string& friendCode);
    int getFriendList(std::stringstream* info);

    int sendMessage(const char* friendCode, int chType, CrossBase* message);
    int pullData(const char* humanCode, int chType, const char* devId, const char* dataId);
    int cancelPullData(const char* humanCode, int chType, const char* devId, const char* dataId);

    int syncInfoDownloadFromDidChain();
    int syncInfoUploadToDidChain();

    int setWalletAddress(const char* name, const char* value);

protected:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    std::shared_ptr<elastos::Contact> mContactImpl;

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    ContactListener* mListener;
    ContactDataListener* mDataListener;

}; // class Contact

#ifdef WITH_CROSSPL
} //namespace native
} //namespace crosspl
#endif // WITH_CROSSPL

#endif /* _ELASTOS_SDK_JNI_CONTACT_BRIDGE_HPP_ */
