/**
 * @file	SecurityManager.hpp
 * @brief	SecurityManager
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_SECURITY_MANAGER_HPP_
#define _ELASTOS_SECURITY_MANAGER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "ErrCode.hpp"

namespace elastos {

class SecurityManager final {
public:
    /*** type define ***/
    class SecurityListener {
    public:
        virtual std::string onAcquirePublicKey() = 0;

        virtual std::vector<uint8_t> onEncryptData(const std::string& pubKey, const std::vector<uint8_t>& src) = 0;
        virtual std::vector<uint8_t> onDecryptData(const std::vector<uint8_t>& src) = 0;

        virtual std::string onAcquireDidPropAppId() = 0;
        virtual std::string onAcquireDidAgentAuthHeader() = 0;


        virtual std::vector<uint8_t> onSignData(const std::vector<uint8_t>& originData) = 0;

    protected:
        explicit SecurityListener() = default;
        virtual ~SecurityListener() = default;
    };

    /*** static function and variable ***/
    static int GetElaAddress(const std::string& pubKey, std::string& elaAddr);
    static int GetDid(const std::string& pubKey, std::string& did);
    static bool IsValidElaAddress(const std::string& code);
    static bool IsValidDid(const std::string& code);

    /*** class function and variable ***/
    explicit SecurityManager();
    virtual ~SecurityManager();

    void setSecurityListener(std::shared_ptr<SecurityListener> listener);

    int getPublicKey(std::string& pubKey);
    int getElaAddress(std::string& elaAddr);
    int getDid(std::string& did);

    int encryptData(const std::string& key, const std::vector<uint8_t>& src, std::vector<uint8_t>& dest);
    int decryptData(const std::vector<uint8_t>& src, std::vector<uint8_t>& dest);

    int saveCryptoFile(const std::string& filePath, const std::vector<uint8_t>& originData);
    int loadCryptoFile(const std::string& filePath, std::vector<uint8_t>& originData);

    int signData(const std::vector<uint8_t>& src, std::vector<uint8_t>& dest);

    int getDidPropAppId(std::string& appId);
    int getDidAgentAuthHeader(std::string& authHeader);

    void clear();

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    std::shared_ptr<SecurityListener> mSecurityListener;

}; // class SecurityManager

} // namespace elastos

#endif /* _ELASTOS_SECURITY_MANAGER_HPP_ */
