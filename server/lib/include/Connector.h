
#ifndef __ELASTOS_CONNECTOR_H__
#define __ELASTOS_CONNECTOR_H__

#include <string>
#include <memory>
#include "PeerListener.h"
#include "PeerNode.h"
#include "HumanInfo.hpp"
#include "Contact.hpp"

namespace elastos {

class Connector {
public:
    Connector(const std::string& serviceName);

    ~Connector();

    void SetMessageListener(std::shared_ptr<PeerListener::MessageListener>& listener);

    void SetDataListener(std::shared_ptr<PeerListener::DataListener>& listener);

    int AddFriend(const std::string& friendCode, const std::string& summary);
    int RemoveFriend(const std::string& friendCode);
    int AcceptFriend(const std::string& friendCode);
    int GetFriendList(std::stringstream* info);
    int SetFriendInfo(const std::string& friendCode, int item, const std::string& value);

    int GetStatus();
    int GetFriendStatus(const std::string& friendCode);

    std::shared_ptr<ElaphantContact::UserInfo> GetUserInfo();
    std::vector<std::shared_ptr<ElaphantContact::FriendInfo>> ListFriendInfo();
    int SendMessage(const std::string& friendCode, const std::string& message);

private:
    std::string mServiceName;

    std::shared_ptr<PeerNode> mPeerNode;
};

}

#endif //__ELASTOS_CONNECTOR_H__
