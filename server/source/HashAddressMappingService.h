//
// Created by luocf on 2019/6/13.
//

#ifndef HASH_ADDRESS_MAPPINGSERVICE_H
#define HASH_ADDRESS_MAPPINGSERVICE_H

#include <stdlib.h>
#include <functional>
#include <memory> // std::unique_ptr
#include <ctime>
#include <thread>
#include <regex>
#include "Common/FileUtils.hpp"
#include "../lib/include/Connector.h"
#include "../lib/include/PeerListener.h"
#include "../lib/include/PeerNode.h"
#include "../lib/include/IMicroService.h"
#include "../lib/include/Elastos.SDK.Contact/Contact.hpp"
#include "../lib/include/Elastos.SDK.Contact/ContactListener.hpp"
using namespace elastos;

namespace micro_service {
    static const char *HashAddressMappingService_TAG = "HashAddressMappingService";
    class HashAddressMappingService:public std::enable_shared_from_this<HashAddressMappingService>{
    public:
        HashAddressMappingService(const std::string& path);
        ~HashAddressMappingService();
        int acceptFriend(const std::string& friendid);
        void receiveMessage(const std::string& friend_id, const std::string& message, std::time_t send_time);
        void helpCmd(const std::vector<std::string> &args, const std::string& message);
        void replyAddressCmd(const std::vector<std::string> &args);
        std::time_t getTimeStamp();
        std::string mOwnerHumanCode;
    protected:
        std::string mPath;

    private:
        void getAddressFromFile(std::string& address);
        Connector* mConnector;
        int Start();
        int Stop();
    };

    class HashAddressMappingMessageListener :public PeerListener::MessageListener{
    public:
        HashAddressMappingMessageListener( HashAddressMappingService* chatGroupService);
        ~HashAddressMappingMessageListener();
        void onEvent(ContactListener::EventArgs& event) override ;
        void onReceivedMessage(const std::string& humanCode, ContactChannel channelType,
                               std::shared_ptr<ElaphantContact::Message> msgInfo) override;
    private:
        HashAddressMappingService*mHashAddressMappingService;
    };

    extern "C" {
        micro_service::HashAddressMappingService* CreateService(const char* path);
        void DestroyService(micro_service::HashAddressMappingService* service);
    }
}

#endif //HASH_ADDRESS_MAPPINGSERVICE_H
