/**
 * @file	UserManager.hpp
 * @brief	UserManager
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_USER_MANAGER_HPP_
#define _ELASTOS_USER_MANAGER_HPP_

#include "Config.hpp"
#include "SecurityManager.hpp"
#include "UserInfo.hpp"

namespace elastos {

class MessageManager;

class UserManager : public std::enable_shared_from_this<UserManager> {
public:
    /*** type define ***/
    class UserListener {
    public:
        explicit UserListener() = default;
        virtual ~UserListener() = default;

//        virtual int onSigninDevicesOverflow(const std::weak_ptr<HumanInfo> userInfo, int capacity) = 0;
    };

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit UserManager(std::weak_ptr<SecurityManager> sectyMgr);
    virtual ~UserManager();

    virtual void setUserListener(std::shared_ptr<UserListener> listener);

    void setConfig(std::weak_ptr<Config> config, std::weak_ptr<MessageManager> msgMgr);

    bool contains(const std::string& userCode);
    bool contains(const std::shared_ptr<HumanInfo>& userInfo);

    int loadLocalData();
    int saveLocalData();
    int serialize(std::string& value) const;

    int restoreUserInfo();
    int ensureUserCarrierInfo();
    int getUserInfo(std::shared_ptr<UserInfo>& userInfo);
    int setUserInfo(UserInfo::Item item, const std::string& value);
    int setIdentifyCode(UserInfo::Type type, const std::string& value);

    int setWalletAddress(const std::string& name, const std::string& value);

//    int syncDownloadDidChainData();
//    int monitorDidChainData();
//    int monitorDidChainIdentifyCode();
//    int uploadUserInfo();

    int setupMultiDevChannels();

private:
    /*** type define ***/

    /*** static function and variable ***/
    static constexpr const char* DataFileName = "userdata.dat";

    /*** class function and variable ***/
    int syncHistoryInfo();
    int mergeIdentifyCodeFromJsonArray(const std::string& jsonArray);

    std::weak_ptr<SecurityManager> mSecurityManager;
    std::weak_ptr<MessageManager> mMessageManager;
    std::weak_ptr<Config> mConfig;
    std::shared_ptr<UserListener> mUserListener;
    std::shared_ptr<UserInfo> mUserInfo;

}; // class UserManager

} // namespace elastos

#endif /* _ELASTOS_USER_MANAGER_HPP_ */
