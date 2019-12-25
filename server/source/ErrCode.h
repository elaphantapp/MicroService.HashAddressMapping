//
// Created by luocf on 2019/6/14.
//


#ifndef CHATROBOT_ERRCODE_H
#define CHATROBOT_ERRCODE_H
#include <string>
#endif //CHATROBOT_ERRCODE_H
namespace micro_service {
    class ErrCode {
    public:
        constexpr static const int FailedCarrier = -1;
        constexpr static const int InvalidArgument = -4;
        constexpr static const int StdSystemErrorIndex = -1000;
        static std::string ToString(int errCode);
    };
}
