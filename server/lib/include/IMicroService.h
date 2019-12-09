

#ifndef __ELASTOS_IMICROSERVICE_H__
#define __ELASTOS_IMICROSERVICE_H__

#include <string>
#include <vector>
#include <memory>

namespace elastos {

class IMicroService {

public:
    enum OperationType {
        OperationType_Encrypt = 0,
        OperationType_Decrypt,
        OperationType_Sign,
        OperationType_Verify
    };

    class DataHandler {
    public:
        virtual std::shared_ptr<std::vector<uint8_t>> EncryptData(const std::vector<uint8_t>& data) = 0;

        virtual std::shared_ptr<std::vector<uint8_t>> DecryptData(const std::vector<uint8_t>& cipherData) = 0;

        virtual std::shared_ptr<std::vector<uint8_t>> SignData(const std::vector<uint8_t>& data) = 0;

        virtual bool VerifyData(const std::vector<uint8_t>& data, const std::vector<uint8_t>& signedData) = 0;
    };

public:
    virtual int Start() = 0;
    virtual int Stop() = 0;
};

}

#endif //__ELASTOS_IMICROSERVICE_H__
