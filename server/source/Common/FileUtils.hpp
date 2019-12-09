#ifndef _FILE_UTILS_HPP_
#define _FILE_UTILS_HPP_

#include <sys/stat.h>
#include <string>
#include <vector>
#include <memory>
namespace micro_service {
    class FileUtils {
    public:
        /*** type define ***/
        /*** static function and variable ***/
        static bool exists(const char *path);

        static int length(const char *path);

        static bool isdir(const char *path);

        static int mkdirs(const char *path, mode_t mode = DEFAULT_MODE);

        static int readFromFile(const char *path, std::shared_ptr <uint8_t> &data);

        static int writeToFile(const char *path, const uint8_t data[], std::size_t size);

        static constexpr mode_t DEFAULT_MODE = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

        /*** class function and variable ***/

    private:
        /*** type define ***/

        /*** static function and variable ***/

        /*** class function and variable ***/
    };
}
#endif /* _FILE_UTILS_HPP_ */
