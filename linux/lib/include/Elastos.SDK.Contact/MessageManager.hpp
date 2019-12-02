/**
 * @file	MessageManager.hpp
 * @brief	MessageManager
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_MESSAGE_MANAGER_HPP_
#define _ELASTOS_MESSAGE_MANAGER_HPP_

#include "Config.hpp"
#include "ErrCode.hpp"
#include "HumanInfo.hpp"
#include "SecurityManager.hpp"
#include "UserManager.hpp"
#include "FriendManager.hpp"

#include "MessageChannelStrategy.hpp"

namespace elastos {

class MessageManager : public std::enable_shared_from_this<MessageManager> {
public:
    /*** type define ***/
    enum class ChannelType: int {
        Carrier = 1,
        ElaChain = 2,
        Email = 3,
    };

    enum class MessageType: uint32_t {
        Empty = 0x00000000,

        Message = 0x000000FF,
        MsgText = 0x00000001,
        MsgAudio = 0x00000002,
        MsgTransfer = 0x00000004,
        MsgImage = 0x00000008,
        MsgFile = 0x00000010,
        MsgBinary = 0x00000020,

        Control = 0x00FF0000,
        CtrlSyncDesc    = 0x00010000,
        CtrlPullData    = 0x00020000,
        CtrlPullDataAck = 0x00040000,
    };

    struct MessageInfo {
    public:
        MessageType mType;
        std::vector<uint8_t> mPlainContent;
        std::string mCryptoAlgorithm;
        uint64_t mTimeStamp;
    private:
        explicit MessageInfo(MessageType type,
                             const std::vector<uint8_t>& plainContent,
                             const std::string& cryptoAlgorithm);
        explicit MessageInfo(const MessageInfo& info,
                             bool ignoreContent = true);
        explicit MessageInfo() = default;
        virtual ~MessageInfo() = default;

        friend MessageManager;
    };

    struct FileInfo {
    public:
        std::string mDevId;
        std::string mName;
        uint64_t mSize;
        std::string mMd5;
    private:
        explicit FileInfo(const std::string& devId,
                          const std::string& name,
                          uint64_t size,
                          const std::string& md5);
        explicit FileInfo() = default;
        virtual ~FileInfo() = default;

        friend MessageManager;
    };

    class MessageListener : public MessageChannelStrategy::ChannelListener {
    public:
        explicit MessageListener() = default;
        virtual ~MessageListener() = default;

        virtual void onStatusChanged(std::shared_ptr<UserInfo> userInfo,
                                     ChannelType channelType,
                                     UserInfo::Status status) = 0;

        virtual void onReceivedMessage(std::shared_ptr<HumanInfo> humanInfo,
                                       ChannelType channelType,
                                       const std::shared_ptr<MessageInfo> msgInfo) = 0;

        virtual void onSentMessage(int msgIndex, int errCode) override = 0;

        virtual void onFriendRequest(std::shared_ptr<FriendInfo> friendInfo,
                                     ChannelType channelType,
                                     const std::string& summary) = 0;

        virtual void onFriendStatusChanged(std::shared_ptr<FriendInfo> friendInfo,
                                           ChannelType channelType,
                                           FriendInfo::Status status) = 0;

        virtual void onHumanInfoChanged(std::shared_ptr<HumanInfo> humanInfo,
                                        ChannelType channelType) = 0;

    private:
        virtual void onStatusChanged(const std::string& userCode,
                                     uint32_t channelType,
                                     ChannelStatus status) override;

        virtual void onReceivedMessage(const std::string& friendCode,
                                       uint32_t channelType,
                                       const std::vector<uint8_t>& msgContent) override;

        //virtual void onSentMessage(int msgIndex, int errCode) override;

        virtual void onFriendRequest(const std::string& friendCode,
                                     uint32_t channelType,
                                     const std::string& summary) override;

        virtual void onFriendStatusChanged(const std::string& friendCode,
                                           uint32_t channelType,
                                           ChannelStatus status) override;

        std::weak_ptr<MessageManager> mMessageManager;
        friend class MessageManager;
    };

    class DataListener : public MessageChannelStrategy::ChannelDataListener {
    public:
        explicit DataListener() = default;
        virtual ~DataListener() = default;

        virtual void onNotify(std::shared_ptr<HumanInfo> humanInfo,
                              ChannelType channelType,
                              const std::string& dataId,
                              int status) = 0;
        virtual int onReadData(std::shared_ptr<HumanInfo> humanInfo,
                               ChannelType channelType,
                               const std::string& dataId,
                               uint64_t offset,
                               std::vector<uint8_t>& data) = 0;
        virtual int onWriteData(std::shared_ptr<HumanInfo> humanInfo,
                                ChannelType channelType,
                                const std::string& dataId,
                                uint64_t offset,
                                const std::vector<uint8_t>& data) = 0;


    private:
        virtual void onNotify(const std::string& friendCode,
                              uint32_t channelType,
                              const std::string& dataId,
                              Status status) override;

        virtual int onReadData(const std::string& friendCode,
                               uint32_t channelType,
                               const std::string& dataId,
                               uint64_t offset,
                               std::vector<uint8_t>& data) override;

        virtual int onWriteData(const std::string& friendCode,
                                uint32_t channelType,
                                const std::string& dataId,
                                uint64_t offset,
                                const std::vector<uint8_t>& data) override ;

        std::weak_ptr<MessageManager> mMessageManager;
        friend class MessageManager;
    };

    /*** static function and variable ***/
    static std::shared_ptr<MessageInfo> MakeEmptyMessage();

    static std::shared_ptr<MessageInfo> MakeMessage(MessageType type,
                                                    const std::vector<uint8_t>& plainContent,
                                                    const std::string& cryptoAlgorithm = "");
    static std::shared_ptr<MessageInfo> MakeTextMessage(const std::string& plainContent,
                                                        const std::string& cryptoAlgorithm = "");

    static std::shared_ptr<FileInfo> MakeEmptyFileInfo();
    static std::shared_ptr<FileInfo> MakeFileInfo(const std::string& mDevId,
                                                  const std::string& mName, uint64_t mSize, const std::string& mMd5);


    /*** class function and variable ***/
    explicit MessageManager(std::weak_ptr<SecurityManager> sectyMgr,
                            std::weak_ptr<UserManager> userMgr,
                            std::weak_ptr<FriendManager> friendMgr);
    virtual ~MessageManager();

    virtual void setMessageListener(std::shared_ptr<MessageListener> listener);
    virtual void setDataListener(std::shared_ptr<DataListener> listener);

    virtual int presetChannels(std::weak_ptr<Config> config);
    virtual int openChannels();
    virtual int closeChannels();

    virtual int requestFriend(const std::string& friendAddr,
                              ChannelType chType,
                              const std::string& summary,
                              bool remoteRequest = true,
                              bool forceRequest = false);

    virtual int updateFriend(const std::string& did);

    virtual int removeFriend(const std::string& friendCode, ChannelType humanChType);

//    virtual int monitorDidChainCarrierID(const std::string& did);

    virtual int sendMessage(const std::shared_ptr<HumanInfo> humanInfo,
                            ChannelType humanChType,
                            const std::shared_ptr<MessageInfo> msgInfo,
                            bool sendToOtherDev = true);

    virtual int pullData(const std::shared_ptr<HumanInfo> humanInfo,
                         ChannelType humanChType,
                         const std::string& devId,
                         const std::string& dataId);
    virtual int cancelPullData(const std::shared_ptr<HumanInfo> humanInfo,
                               ChannelType humanChType,
                               const std::string& devId,
                               const std::string& dataId);

    virtual int broadcastDesc(ChannelType chType);

private:
    /*** type define ***/

    /*** static function and variable ***/
    static std::shared_ptr<MessageInfo> MakeMessage(std::shared_ptr<MessageInfo> from, bool ignoreContent = true);

    /*** class function and variable ***/
    template <class T>
    int getChannel(ChannelType chType, std::weak_ptr<T>& channel);
    int processCtrlMessage(std::shared_ptr<HumanInfo> humanInfo,
                           ChannelType channelType,
                           const std::string& friendCode,
                           const std::shared_ptr<MessageInfo> msgInfo);
    int sendDescMessage(const std::vector<std::shared_ptr<HumanInfo>>& humanList, ChannelType chType);

    std::weak_ptr<SecurityManager> mSecurityManager;
    std::weak_ptr<UserManager> mUserManager;
    std::weak_ptr<FriendManager> mFriendManager;
    std::shared_ptr<MessageListener> mMessageListener;
    std::shared_ptr<DataListener> mDataListener;

    std::map<ChannelType, std::shared_ptr<MessageChannelStrategy>> mMessageChannelMap;

    friend class UserManager;
}; // class MessageManager

/***********************************************/
/***** class template function implement *******/
/***********************************************/
template <class T>
int MessageManager::getChannel(ChannelType chType, std::weak_ptr<T>& channel)
{
    const auto& it = mMessageChannelMap.find(chType);
    if(it == mMessageChannelMap.end()) {
        return ErrCode::NotFoundError;
    }

    auto ptr = std::dynamic_pointer_cast<T>(it->second);
    if(ptr.get() == nullptr) {
        return ErrCode::InvalidArgument;
    }

    channel = ptr;

    return 0;
}

} // namespace elastos

#endif /* _ELASTOS_MESSAGE_MANAGER_HPP_ */
