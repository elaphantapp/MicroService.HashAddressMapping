/**
 * @file	ContactDebug.hpp
 * @brief	Contact
 * @details
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_SDK_JNI_CONTACT_DEBUG_HPP_
#define _ELASTOS_SDK_JNI_CONTACT_DEBUG_HPP_

#include <sstream>

#ifdef WITH_CROSSPL
namespace crosspl {
namespace native {
#endif // WITH_CROSSPL
    
class ContactDebug {
public:
    /*** type define ***/

    /*** static function and variable ***/
    static int GetCachedDidProp(std::stringstream* value);

    /*** class function and variable ***/
    explicit ContactDebug() = default;
    virtual ~ContactDebug() = default;

private:
    /*** type define ***/

    /*** static function and variable ***/

    /*** class function and variable ***/
}; // class Contact

#ifdef WITH_CROSSPL
} //namespace native
} //namespace crosspl
#endif // WITH_CROSSPL

#endif /* _ELASTOS_SDK_JNI_CONTACT_DEBUG_HPP_ */

