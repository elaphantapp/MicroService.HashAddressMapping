/**
 * @file	FriendManager.hpp
 * @brief	FriendManager
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_FRIEND_MANAGER_HPP_
#define _ELASTOS_FRIEND_MANAGER_HPP_

#include "Config.hpp"
#include "FriendInfo.hpp"
#include "SecurityManager.hpp"

namespace elastos {

class MessageManager;

class FriendManager : public std::enable_shared_from_this<FriendManager> {
public:
    /*** type define ***/
    class FriendListener {
    public:
        explicit FriendListener() = default;
        virtual ~FriendListener() = default;

//        virtual int onFriendRequest(FriendInfo friendInfo, std::string message) = 0;
//        virtual void onStatusChanged(FriendInfo friendInfo, uint64_t status) = 0;
    };

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit FriendManager(std::weak_ptr<SecurityManager> sectyMgr);
    virtual ~FriendManager();

    virtual void setFriendListener(std::shared_ptr<FriendListener> listener);

    void setConfig(std::weak_ptr<Config> config, std::weak_ptr<MessageManager> msgMgr);
    int loadLocalData();
    int saveLocalData();
    int serialize(std::string& value) const;

    int restoreFriendsInfo();

    bool contains(const std::string& friendCode);
    bool contains(const std::shared_ptr<HumanInfo>& friendInfo);

    virtual int tryAddFriend(const std::string& friendCode, const std::string& summary, bool remoteRequest = true, bool forceRequest = false);
    virtual int tryAcceptFriend(const std::string& friendCode);
    virtual int tryRemoveFriend(const std::string& friendCode);
    virtual int tryGetFriendInfo(const std::string& friendCode, std::shared_ptr<FriendInfo>& friendInfo);

    virtual int addFriend(FriendInfo::HumanKind friendKind, const std::string& friendCode,
                          const std::string& summary, bool remoteRequest = true, bool forceRequest = false);
    virtual int removeFriend(FriendInfo::HumanKind friendKind, const std::string& friendCode);

    virtual int addFriendInfo(std::shared_ptr<FriendInfo> friendInfo);
    virtual int getFriendInfo(FriendInfo::HumanKind friendKind, const std::string& friendCode,
                              std::shared_ptr<FriendInfo>& friendInfo);
    virtual int getFriendInfoList(std::vector<std::shared_ptr<FriendInfo>>& friendList);

    virtual int acceptFriend(std::shared_ptr<FriendInfo> friendInfo);

    //virtual int getAllFriendInfo(std::shared_ptr<FriendInfo>& friendInfo);

    virtual std::vector<FriendInfo> filterFriends(std::string regex);

//    int syncDownloadDidChainData();
//    int monitorDidChainData();
//    int monitorDidChainFriendID();
    // int uploadFriendInfo();

    int cacheFriendToDidChain(std::shared_ptr<FriendInfo> friendInfo);
private:
    /*** type define ***/

    /*** static function and variable ***/
    static constexpr const char* DataFileName = "frienddata.dat";

    /*** class function and variable ***/
    virtual int addFriendByDid(const std::string& did, const std::string& summary, bool remoteRequest, bool forceRequest = false);
    virtual int addFriendByCarrier(const std::string& carrierAddress, const std::string& summary, bool remoteRequest, bool forceRequest = false);
    virtual int addFriendByEla(const std::string& elaAddress, const std::string& summary, bool remoteRequest, bool forceRequest);
    virtual int removeFriendByDid(const std::string& did);
    virtual int removeFriendByCarrier(const std::string& carrierAddress);
    virtual int removeFriendByEla(const std::string& elaAddress);
    virtual int getFriendInfoByDid(const std::string& did, std::shared_ptr<FriendInfo>& friendInfo);
    virtual int getFriendInfoByCarrier(const std::string& carrierUsrId, std::shared_ptr<FriendInfo>& friendInfo);
    virtual int getFriendInfoByEla(const std::string& elaAddress, std::shared_ptr<FriendInfo>& friendInfo);

    virtual int mergeFriendInfoFromJsonArray(const std::string& jsonArray);

    std::weak_ptr<SecurityManager> mSecurityManager;
    std::weak_ptr<MessageManager> mMessageManager;
    std::weak_ptr<Config> mConfig;
    std::shared_ptr<FriendListener> mFriendListener;
    std::vector<std::shared_ptr<FriendInfo>> mFriendList;

}; // class FriendManager

} // namespace elastos

#endif /* _ELASTOS_FRIEND_MANAGER_HPP_ */
