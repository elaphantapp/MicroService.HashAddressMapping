#ifndef _ELASTOS_CONFIG_HPP_
#define _ELASTOS_CONFIG_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ErrCode.hpp"

namespace elastos {

class Config {
public:
    /*** type define ***/
    struct CarrierConfig {
        struct Node {
            const std::string mIpv4;
            const std::string mPort;
        };
        struct BootstrapNode : Node {
            const std::string mPublicKey;
        };
        struct HiveNode : Node {
        };

        int mLogLevel;
        bool mEnableUdp;
        std::vector<BootstrapNode> mBootstrapNodes;
        std::vector<HiveNode> mHiveNodes;
    };

    struct ElaChainConfig {
        struct Api {
            std::string mGetBalance;
            std::string mCreateTx;
            std::string mCreateCrossTx;
            std::string mSendRawTx;
            std::string mGetTx;
            std::string mGetAllTxs;
            std::string mGetHistory;
            std::string mGetPubKey;
        };

        std::string mUrl;
        Api mApi;
    };

    struct DidChainConfig : ElaChainConfig {
        struct AgentApi {
            std::string mGetDidProps;
            std::string mUploadDidProps;
            std::string mDidPropHistory;
            std::string mDidProp;
        };
        std::string mDidUrl;
        AgentApi mAgentApi;
    };

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit Config(const std::string& userDataDir);
    virtual ~Config();

    int load();
    int save();

    const std::string mUserDataDir;

    std::unique_ptr<CarrierConfig> mCarrierConfig;
    std::unique_ptr<ElaChainConfig> mElaChainConfig;

    std::unique_ptr<DidChainConfig> mDidChainConfig;
private:
    /*** type define ***/

    /*** static function and variable ***/
    static constexpr const char* ConfigFileName = "config.dat";

    /*** class function and variable ***/
    int loadDefaultValues();



}; // class Config

} // namespace elastos

#endif /* _ELASTOS_CONFIG_HPP_ */
