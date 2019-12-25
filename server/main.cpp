#include <iostream>
#include <thread>
#include "HashAddressMappingService.h"
#include "PeerNodeSDK/PeerNode.h"
#include "Elastos.SDK.Contact/Contact.hpp"
using namespace micro_service;
int main(int argc,char *argv[]) {
    std::cout << "I'm ChatGroup Service !" << std::endl;
    if (argc < 2) {
        return -1;
    }
    char* data_dir = argv[1];
    printf("data_dir :%s\n", data_dir);
    auto mPeerNode = PeerNode::GetInstance(data_dir);
    class Listener : public elastos::PeerListener::Listener {
        std::string testPublicKey = "02bc11aa5c35acda6f6f219b94742dd9a93c1d11c579f98f7e3da05ad910a48306";
        virtual std::shared_ptr<std::vector<uint8_t>> onAcquire(const ContactListener::AcquireArgs& request) {
            std::shared_ptr<std::vector<uint8_t>> response;
            switch(request.type) {

                case ElaphantContact::Listener::AcquireType::PublicKey:

                    response = std::make_shared<std::vector<uint8_t>>(

                            testPublicKey.begin(), testPublicKey.end());

                    break;

                case ElaphantContact::Listener::AcquireType::EncryptData:

                    response = std::make_shared<std::vector<uint8_t>>(request.data);

                    break;

                case ElaphantContact::Listener::AcquireType::DecryptData:

                    response = std::make_shared<std::vector<uint8_t>>(request.data);

                    break;

                case ElaphantContact::Listener::AcquireType::DidPropAppId:

                    break;

                case ElaphantContact::Listener::AcquireType::DidAgentAuthHeader:

                {

                    std::string appid = "org.elastos.microservice.test";

                    std::string appkey = "b2gvzUM79yLhCbbGNWCuhSsGdqYhA7sS";

                    std::string header = "id=" + appid + ";time=77777;auth=" + appkey;

                    response = std::make_shared<std::vector<uint8_t>>(header.begin(), header.end());

                    break;

                }

                case ElaphantContact::Listener::AcquireType::SignData:

                {

                    break;

                }


            }
            return response;
        }
        virtual void onError(int errCode, const std::string& errStr, const std::string& ext) {

            printf("PeerNode error code: %d: %s, %s\n", errCode, errStr.c_str(), ext.c_str());

        }

    };


    static std::shared_ptr<elastos::PeerListener::Listener> listener(new Listener());
    mPeerNode->SetListener(listener);
    mPeerNode->Start();
    //char* pub_key = "02bc11aa5c35acda6f6f219b94742dd9a93c1d11c579f98f7e3da05ad910a48306";
    //std::cout << "I'm ChatGroup Service:"<<data_dir<<","<<pub_key << std::endl;
    CreateService(data_dir);
    while(1) {
        std::chrono::milliseconds dur(1000);
        std::this_thread::sleep_for(dur);
    }
    return 0;
}
