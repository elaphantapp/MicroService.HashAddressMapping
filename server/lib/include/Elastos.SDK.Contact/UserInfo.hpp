/**
 * @file	UserInfo.hpp
 * @brief	UserInfo
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_USER_INFO_HPP_
#define _ELASTOS_USER_INFO_HPP_

#include <memory>
#include "HumanInfo.hpp"
#include "IdentifyCode.hpp"

namespace elastos {

class UserManager;

class UserInfo : public HumanInfo
               , public IdentifyCode {
public:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit UserInfo(std::weak_ptr<UserManager> userMgr);
    virtual ~UserInfo();

    // virtual int serialize(const CarrierInfo& info, std::string& value) const override;
    // virtual int deserialize(const std::string& value, CarrierInfo& info) const override;

    virtual int serialize(std::string& value, bool summaryOnly = false) const override;
    virtual int deserialize(const std::string& value, bool summaryOnly = false) override;
    virtual int toJson(std::shared_ptr<Json>& value) const override;

    virtual int addCarrierInfo(const CarrierInfo& info, const Status status) override;
    virtual int setHumanInfo(Item item, const std::string& value) override;
    virtual int mergeHumanInfo(const HumanInfo& value, const Status status) override;

    virtual int setWalletAddress(const std::string& name, const std::string& value);

    virtual int setIdentifyCode(Type type, const std::string& value) override;

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    std::weak_ptr<UserManager> mUserManager;

}; // class UserInfo

} // namespace elastos

#endif /* _ELASTOS_USER_INFO_HPP_ */
