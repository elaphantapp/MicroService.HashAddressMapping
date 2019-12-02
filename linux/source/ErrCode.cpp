//
// Created by luocf on 2019/6/14.
//

#include <system_error>
#include "ErrCode.h"
namespace micro_service {
    std::string ErrCode::ToString(int errCode)
    {
        std::string errMsg;

        switch (errCode) {

            case InvalidArgument:
                errMsg = "InvalidArgument";
                break;
            case StdSystemErrorIndex:
                errMsg = "StdSystemErrorIndex";
                break;
        }

        if(errCode < StdSystemErrorIndex) {
            int stdErrVal = StdSystemErrorIndex - errCode;
            auto stdErrCode = std::error_code(stdErrVal, std::generic_category());
            errMsg = stdErrCode.message();
        }

        return errMsg;
    }
}