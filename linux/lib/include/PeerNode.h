
#ifndef __ELASTOS_PEER_NODE_H__
#define __ELASTOS_PEER_NODE_H__

#include <string>
#include <memory>
#include "PeerListener.h"
#include "Contact.hpp"

namespace elastos {

class PeerNode {
private:
    class ContactListener : public ElaphantContact::Listener {
    public:
        explicit ContactListener(PeerNode* node)
            : mNode(node)
        {};

        virtual ~ContactListener() = default;

        virtual std::shared_ptr<std::vector<uint8_t>> onAcquire(const ContactListener::AcquireArgs& request) override;

        virtual void onEvent(ContactListener::EventArgs& event) override;

        virtual void onReceivedMessage(const std::string& humanCode,
                                       ContactChannel channelType,
                                       std::shared_ptr<ElaphantContact::Message> msgInfo) override;

        virtual void onError(int errCode, const std::string& errStr,
                             const std::string& ext) override;
    private:
        std::shared_ptr<PeerListener::MessageListener> FindListener(const std::string& content);

    private:
        PeerNode* mNode;
    };

    class ContactDataListener : public ElaphantContact::DataListener {
    public:
        explicit ContactDataListener(PeerNode* node)
            : mNode(node)
        {};

        virtual ~ContactDataListener() = default;

        virtual void onNotify(const std::string& humanCode,
                              ContactChannel channelType,
                              const std::string& dataId, int status) override;

        virtual int onReadData(const std::string& humanCode,
                               ContactChannel channelType,
                               const std::string& dataId, uint64_t offset,
                               std::vector<uint8_t>& data) override;

        virtual int onWriteData(const std::string& humanCode,
                                ContactChannel channelType,
                                const std::string& dataId, uint64_t offset,
                                const std::vector<uint8_t>& data) override;
    private:
        PeerNode* mNode;
    };

public:
    ~PeerNode()
    {
        mMsgListenerMap.clear();
        mDataListenerMap.clear();
    }

    void SetListener(std::shared_ptr<PeerListener::Listener>& listener);

    void AddMessageListener(const std::string& serviceName, std::shared_ptr<PeerListener::MessageListener>& listener);
    void RemoveMessageListener(const std::string& serviceName);

    void AddDataListener(const std::string& serviceName, std::shared_ptr<PeerListener::DataListener>& listener);
    void RemoveDataListener(const std::string& serviceName);


    int Start();
    int Stop();

    int SetUserInfo(int item, const std::string& value);
    int SetIdentifyCode(int type, const std::string& value);

    int AddFriend(const std::string& friendCode, const std::string& summary);
    int RemoveFriend(const std::string& friendCode);
    int AcceptFriend(const std::string& friendCode);
    int GetFriendList(std::stringstream* info);
    int SetFriendInfo(const std::string& friendCode, int item, const std::string& value);

    int PullData(const std::string& humanCode, int chType, const std::string& devId, const std::string& dataId);
    int CancelPullData(const std::string& humanCode, int chType, const std::string& devId, const std::string& dataId);

    int SyncInfoDownloadFromDidChain();
    int SyncInfoUploadToDidChain();

    int SetWalletAddress(const std::string& name, const std::string& value);

    int GetStatus();
    int GetFriendStatus(const std::string& friendCode);

    std::shared_ptr<ElaphantContact::UserInfo> GetUserInfo();
    std::vector<std::shared_ptr<ElaphantContact::FriendInfo>> ListFriendInfo();

    int SendMessage(const std::string& friendCode, const std::string& message);

public:
    // create instance if sInstance is null.
    static std::shared_ptr<PeerNode> GetInstance(const std::string& path);
    //do not create instance, just return sInstance.
    static std::shared_ptr<PeerNode> GetInstance();

private:
    PeerNode(const std::string& path);

    std::string GetDid();

private:
    static std::shared_ptr<PeerNode> sInstance;

    std::shared_ptr<PeerListener::Listener> mListener;
    std::map<std::string, std::shared_ptr<PeerListener::MessageListener>> mMsgListenerMap;
    std::map<std::string, std::shared_ptr<PeerListener::DataListener>> mDataListenerMap;

    std::mutex mListenerMutex;
    std::mutex mMsgListenerMutex;
    std::mutex mDataListenerMutex;

    std::shared_ptr<ElaphantContact> mContact;
    std::shared_ptr<ContactListener> mContactListener;
    std::shared_ptr<ContactDataListener> mContactDataListener;
};

}

#endif // __ELASTOS_PEER_NODE_H__
