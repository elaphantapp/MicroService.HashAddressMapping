//
//  FileUtils.cpp
//  Forge
//
//  Created by mengxk on 9/12/16.
//  Copyright © 2016 mengxk. All rights reserved.
//

#include "FileUtils.hpp"
#include <fstream>
#include <sstream>
#include <Tools/Log.hpp>
namespace micro_service {
/***********************************************/
/***** static variables initialize *************/
/***********************************************/

/***********************************************/
/***** static function implement ***************/
/***********************************************/
    bool FileUtils::exists(const char *path) {
        struct stat info;

        int ret = stat(path, &info);

        return (ret == 0);
    }

    int FileUtils::length(const char *path) {
        std::ifstream ifs(path, std::ios::binary);
        if (ifs.is_open() == false) {
            Log::E(Log::TAG, "Failed to get file length: can't open.", path);
            return -1;
        }

        // get length of file:
        ifs.seekg(0, ifs.end);
        int size = ifs.tellg();
        ifs.close();

        return size;
    }

    bool FileUtils::isdir(const char *path) {
        struct stat info;

        int ret = stat(path, &info);
        if (ret == 0
            && info.st_mode & S_IFDIR) {
            return true;
        }

        return false;
    }

    int FileUtils::mkdirs(const char *path, mode_t mode) {
        // const cast for hack
        char *p = const_cast<char *>(path);

        // Do mkdir for each slash until end of string or error
        while (*p != '\0') {
            // Skip first character
            p++;

            // Find first slash or end
            while (*p != '\0' && *p != '/') p++;

            // Remember value from p
            char v = *p;

            // Write end of string at p
            *p = '\0';

            // Create folder from path to '\0' inserted at p
            if (mkdir(path, mode) == -1 && errno != EEXIST) {
                *p = v;
                return errno;
            }

            // Restore path to it's former glory
            *p = v;
        }

        return 0;
    }

    int FileUtils::readFromFile(const char *path, std::shared_ptr <uint8_t> &data) {
        std::ifstream ifs(path, std::ios::binary);
        if (ifs.is_open() == false) {
            Log::E(Log::TAG, "Failed to read from file: can't open '%s'.", path);
            return -1;
        }

        int size = FileUtils::length(path);
        data = std::shared_ptr<uint8_t>(new uint8_t[size + 1], std::default_delete<uint8_t[]>());

        ifs.seekg(0, ifs.beg);
        ifs.read(reinterpret_cast<char *>(data.get()), size);
        ifs.close();

        (data.get())[size] = '\0'; // 补足字符串尾标志

        return size;
    }

    int FileUtils::writeToFile(const char *path, const uint8_t data[], std::size_t size) {
        std::string tmp_path(path);
        tmp_path.append(".tmp");

        std::ofstream ofs(tmp_path, std::ios::binary);
        if (ofs.is_open() == false) {
            Log::E(Log::TAG, "Failed to write to file: can't open.", path);
            return -1;
        }

        ofs.write(reinterpret_cast<const char *>(data), size);

        ofs.flush();
        ofs.close();

        // rename tmp ==> path
        int ret = std::rename(tmp_path.c_str(), path);
        if (ret < 0) {
            Log::E(Log::TAG, "Failed to write to file: %s, err=%d", path, ret);
            return ret;
        }

        // try to get file length
        size = FileUtils::length(path);

        return size;
    }


/***********************************************/
/***** class public function implement  ********/
/***********************************************/


/***********************************************/
/***** class protected function implement  *****/
/***********************************************/


/***********************************************/
/***** class private function implement  *******/
/***********************************************/
}