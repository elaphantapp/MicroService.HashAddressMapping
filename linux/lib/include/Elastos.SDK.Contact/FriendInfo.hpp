#ifndef _ELASTOS_FRIEND_INFO_HPP_
#define _ELASTOS_FRIEND_INFO_HPP_

#include <memory>
#include "HumanInfo.hpp"

namespace elastos {

class FriendManager;

class FriendInfo : public HumanInfo {
public:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    explicit FriendInfo(std::weak_ptr<FriendManager> friendMgr);
    virtual ~FriendInfo();

    int setAlias(const std::string& value);
    std::string getAlias() const;

    virtual int serialize(std::string& value, bool summaryOnly = false) const override;
    virtual int deserialize(const std::string& value, bool summaryOnly = false) override;
    virtual int toJson(std::shared_ptr<Json>& value) const override;

    virtual int addCarrierInfo(const CarrierInfo& info, const Status status) override;
    virtual int delCarrierInfo(const std::string& carrierUsrId) override;
    virtual int setHumanInfo(Item item, const std::string& value) override;
    virtual int mergeHumanInfo(const HumanInfo& value, const Status status) override;

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
    std::weak_ptr<FriendManager> mFriendManager;

    std::string mAlias;

}; // class FriendInfo

} // namespace elastos

#endif /* _ELASTOS_FRIEND_INFO_HPP_ */
