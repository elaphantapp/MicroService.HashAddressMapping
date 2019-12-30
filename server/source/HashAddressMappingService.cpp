//
// Created by luocf on 2019/6/13.
//
#include <cstring>
#include <future>
#include <stack>
#include <iostream>
#include <ctime>

using namespace std;
#include <thread>
#include <chrono>
#include <Tools/Log.hpp>
#include "Json.hpp"
#include <ThirdParty/CompatibleFileSystem.hpp>
#include <Command/HashAddressMappingCmd.hpp>
#include "HashAddressMappingService.h"
#include "ErrCode.h"
namespace micro_service {
    /***********************************************/
    /***** static function implement ***************/
    /***********************************************/
    HashAddressMappingService::HashAddressMappingService(const std::string& path,const std::string& info_path)
            : mPath(path), mInfoPath(info_path){
        mConnector = new Connector(HashAddressMappingService_TAG);
        this->Start();
    }
    
    HashAddressMappingService::~HashAddressMappingService() {
        this->Stop();
    }

    int HashAddressMappingService::Start() {
        if (mConnector == NULL) return -1;
        printf("Service start!\n");
        std::shared_ptr<PeerListener::MessageListener> message_listener = std::make_shared<HashAddressMappingMessageListener>(this);
        mConnector->SetMessageListener(message_listener);
        auto status = PeerNode::GetInstance()->GetStatus();
        printf("HashAddressMappingService Start status: %d\n",static_cast<int>(status));
        std::shared_ptr<ElaphantContact::UserInfo> user_info = mConnector->GetUserInfo();
        if (user_info.get() != NULL) {
            user_info->getHumanCode(mOwnerHumanCode);
            printf("Service start mOwnerHumanCode:%s\n", mOwnerHumanCode.c_str());
            std::shared_ptr<Json> user_info_json = std::make_shared<Json>();
            user_info->toJson(user_info_json);
            std::string user_info_str = user_info_json->dump();
            int data_length = user_info_str.length();
            uint8_t data[data_length];
            std::copy(std::begin(user_info_str), std::end(user_info_str), data);
            FileUtils::writeToFile(mInfoPath.c_str(), data, data_length) ;
        }
        return 0;
    }

    int HashAddressMappingService::Stop() {
        if (mConnector == NULL) return -1;
        printf("Service stop!\n");
        return 0;
    }
    std::time_t HashAddressMappingService::getTimeStamp() {
        return time(0);
    }

    void HashAddressMappingService::receiveMessage(const std::string& friend_id, const std::string& message, std::time_t send_time) {
        std::string errMsg;
        std::string msg = message;
        std::string pre_cmd = msg + " " + friend_id;//Pretreatment cmd
        HashAddressMappingCmd::Do(this, pre_cmd, errMsg);
    }

    int HashAddressMappingService::acceptFriend(const std::string& friendid) {
        mConnector->AcceptFriend(friendid);
        return 0;
    }

    void HashAddressMappingService::helpCmd(const std::vector<std::string> &args, const std::string &message) {
        if (args.size() >= 2) {
            const std::string friend_id = args[1];
            Json respJson;
            respJson["serviceName"] = HashAddressMappingService_TAG;
            respJson["type"] = "textMsg";
            respJson["content"] = message;
            int ret = mConnector->SendMessage(friend_id, respJson.dump());
            if (ret != 0) {
                Log::I(HashAddressMappingService_TAG,
                       "helpCmd .c_str(): %s errno:(0x%x)",
                       message.c_str());
            }
        }
    }

    void HashAddressMappingService::replyAddressCmd(const std::vector<std::string> &args) {
        if (args.size() >= 2) {
            const std::string friend_id = args[1];
            std::string message;
            std::shared_ptr <uint8_t> data = std::make_shared<uint8_t>(1024*1024);
            std::string file_path = mPath+"/"+"HashAddressMapping.txt";
            int length = FileUtils::readFromFile(file_path.c_str(), data);
            if (length == 0) {
                message = "No Content!";
            } else {
                message = std::string(data.get(), data.get()+length);
            }

            Json respJson;
            respJson["serviceName"] = HashAddressMappingService_TAG;
            respJson["type"] = "textMsg";
            respJson["content"] = message;
            int ret = mConnector->SendMessage(friend_id, respJson.dump());
            if (ret != 0) {
                Log::I(HashAddressMappingService_TAG,
                       "replyAddressCmd .c_str(): %s errno:(0x%x)",
                       message.c_str());
            }
        }
    }
    HashAddressMappingMessageListener::HashAddressMappingMessageListener(HashAddressMappingService* service) {
        mHashAddressMappingService = service;
    }

    HashAddressMappingMessageListener::~HashAddressMappingMessageListener() {
    }

    void HashAddressMappingMessageListener::onEvent(ElaphantContact::Listener::EventArgs& event) {
        Log::W(HashAddressMappingService_TAG, "onEvent type: %d\n", event.type);
        switch (event.type) {
            case ElaphantContact::Listener::EventType::FriendRequest: {
                auto friendEvent = dynamic_cast<ElaphantContact::Listener::RequestEvent*>(&event);
                Log::W(HashAddressMappingService_TAG, "FriendRequest from: %s\n", friendEvent->humanCode.c_str());
                mHashAddressMappingService->acceptFriend(friendEvent->humanCode);
                break;
            }
            case ElaphantContact::Listener::EventType::StatusChanged: {
                auto statusEvent = dynamic_cast<ElaphantContact::Listener::StatusEvent*>(&event);
                Log::I(HashAddressMappingService_TAG, "StatusChanged from: %s, statusEvent->status:%d\n", statusEvent->humanCode.c_str(), static_cast<int>(statusEvent->status));
                if (mHashAddressMappingService->mOwnerHumanCode != statusEvent->humanCode) {
                    if(statusEvent->status == elastos::HumanInfo::Status::Online) {
                        std::vector <std::string> args{"",statusEvent->humanCode};
                        mHashAddressMappingService->replyAddressCmd(args);
                    }
                }
                break;
            }
            case ElaphantContact::Listener::EventType::HumanInfoChanged:{
                auto infoEvent = dynamic_cast<ElaphantContact::Listener::InfoEvent*>(&event);
                Log::I(HashAddressMappingService_TAG, "HumanInfoChanged from: %s\n", infoEvent->humanCode.c_str());
                break;
            }
            default: {
                break;
            }
        }
    };

    void HashAddressMappingMessageListener::onReceivedMessage(const std::string& humanCode, ElaphantContact::Channel channelType,
                                                     std::shared_ptr<ElaphantContact::Message> msgInfo) {

        auto text_data = dynamic_cast<ElaphantContact::Message::TextData*>(msgInfo->data.get());
        std::string content = text_data->toString();
        try {
            Json json = Json::parse(content);
            std::string msg_content = json["content"];
            printf("HashAddressMappingMessageListener onReceivedMessage humanCode: %s,msg_content:%s \n", humanCode.c_str(), msg_content.c_str());
            mHashAddressMappingService->receiveMessage(humanCode,
                                          msg_content, mHashAddressMappingService->getTimeStamp());
        } catch (const std::exception& e) {
            printf("HashAddressMappingMessageListener parse json failed\n");
        }
    }

    extern "C" {
    micro_service::HashAddressMappingService* CreateService(const char* path, const char* info_path) {
        return new micro_service::HashAddressMappingService(path, info_path);
    }
    void DestroyService(micro_service::HashAddressMappingService* service) {
        if (service) {
            delete service;
        }
    }
    }
}
