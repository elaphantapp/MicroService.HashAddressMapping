#ifndef _ELASTOS_IDENTIFY_CODE_HPP_
#define _ELASTOS_IDENTIFY_CODE_HPP_

#include <map>
#include <string>

#include <ErrCode.hpp>
#include <Json.hpp>

namespace elastos {

class IdentifyCode {
public:
    /*** type define ***/
    enum class Type: int {
        PhoneNumber = 1,
        EmailAddress,
        WechatId,
    };

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit IdentifyCode();
    virtual ~IdentifyCode();

    virtual int setIdentifyCode(Type type, const std::string& value);
    virtual int getIdentifyCode(Type type, std::string& value) const;

    virtual int serialize(std::string& value) const;
    virtual int deserialize(const std::string& value);
    virtual int toJson(std::shared_ptr<Json>& value) const;
    // virtual int print(std::string value);

    virtual int mergeIdentifyCode(const IdentifyCode& value);
private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    std::map<Type, std::string> mIdCodeMap;
    int64_t mUpdateTime;
}; // class IdentifyCode

} // namespace elastos

#endif /* _ELASTOS_IDENTIFY_CODE_HPP_ */
