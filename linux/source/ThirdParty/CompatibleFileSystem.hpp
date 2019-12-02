/**
 * @file	ThreadPool.hpp
 * @brief	ThreadPool
 * @details	
 *
 * @author	xxx
 * @author	<xxx@xxx.com>
 * @copyright	(c) 2012 xxx All rights reserved.
 **/

#ifndef _ELASTOS_FILE_SYSTEM_HPP_
#define _ELASTOS_FILE_SYSTEM_HPP_

#if defined(__APPLE__) || defined(__ANDROID__) || defined(__linux__)
#include "ghc-filesystem.hpp"
namespace std {
namespace filesystem = ghc::filesystem;
}
#else /* defined(__APPLE__) || defined(__ANDROID__) */
#include <filesystem>
#endif /* defined(__APPLE__) || defined(__ANDROID__) */


#endif /* _ELASTOS_FILE_SYSTEM_HPP_ */
