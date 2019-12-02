/**
 * @file	Elastos.SDK.Contact.hpp
 * @brief	Contact
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_OLDSDK_CONTACT_HPP_
#define _ELASTOS_OLDSDK_CONTACT_HPP_

#include "Config.hpp"
#include "FriendManager.hpp"
#include "MessageManager.hpp"
#include "SecurityManager.hpp"
#include "UserManager.hpp"

namespace elastos {

class Contact {
public:
    /*** type define ***/
    class Factory {
    public:
        static void SetLogLevel(int level);
        static int SetLocalDataDir(const std::string& dir);
        static std::shared_ptr<Contact> Create();

    private:
        static std::string sLocalDataDir;
        friend class Contact;
    }; // class Factory

    class Listener { // TODO
    public:
        virtual void onRequest() = 0;
        virtual void onEvent() = 0;
        virtual void onError(int errCode) = 0;
    protected:
        explicit Listener() = default;
        virtual ~Listener() = default;
    }; // class ContactListener

    /*** static function and variable ***/

    /*** class function and variable ***/
    void setListener(std::shared_ptr<SecurityManager::SecurityListener> sectyListener,
                     std::shared_ptr<UserManager::UserListener> userListener = nullptr,
                     std::shared_ptr<FriendManager::FriendListener> friendListener = nullptr,
                     std::shared_ptr<MessageManager::MessageListener> msgListener = nullptr);

    std::weak_ptr<SecurityManager> getSecurityManager();
    std::weak_ptr<UserManager> getUserManager();
    std::weak_ptr<FriendManager> getFriendManager();
    std::weak_ptr<MessageManager> getMessageManager();

    int start();
    int stop();
    bool isStarted();

    int syncInfoDownloadFromDidChain();
    int syncInfoUploadToDidChain();

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit Contact();
    virtual ~Contact();
    int getUserDataDir(std::string& dir);
    int initGlobal();
    int monitorDidChainData();

    std::shared_ptr<SecurityManager> mSecurityManager;
    std::shared_ptr<UserManager> mUserManager;
    std::shared_ptr<FriendManager> mFriendManager;
    std::shared_ptr<MessageManager> mMessageManager;
    std::shared_ptr<Config> mConfig;
    bool mStarted;
}; // class Contact

} // namespace elastos

#endif /* _ELASTOS_OLDSDK_CONTACT_HPP_ */
