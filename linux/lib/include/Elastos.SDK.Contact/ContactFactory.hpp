/**
 * @file	ContactFactory.hpp
 * @brief	Contact
 * @details
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_SDK_JNI_CONTACT_FACTORY_HPP_
#define _ELASTOS_SDK_JNI_CONTACT_FACTORY_HPP_

#include <string>

#ifdef WITH_CROSSPL
namespace crosspl {
namespace native {
#endif // WITH_CROSSPL

class ContactFactory {
public:
    /*** type define ***/

    /*** static function and variable ***/
    static void SetLogLevel(int level);

    static void SetDeviceId(const std::string& devId);

    static int SetLocalDataDir(const std::string& dir);

    /*** class function and variable ***/
    explicit ContactFactory() = default;
    virtual ~ContactFactory() = default;

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
}; // class Contact

#ifdef WITH_CROSSPL
} //namespace native
} //namespace crosspl
#endif // WITH_CROSSPL
  
#endif /* _ELASTOS_SDK_JNI_CONTACT_FACTORY_HPP_ */

