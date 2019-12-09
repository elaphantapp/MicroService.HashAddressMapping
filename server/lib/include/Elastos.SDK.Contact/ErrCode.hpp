#ifndef _ELASTOS_ERRCODE_HPP_
#define _ELASTOS_ERRCODE_HPP_

#include <string>
#include <functional>

namespace elastos {

class ErrCode {
public:
    /*** type define ***/
#define CHECK_ERROR(ret) \
	if(ret < 0) { \
		Log::E(Log::TAG, "Failed to call %s in line %d, return %d.", __PRETTY_FUNCTION__, __LINE__, ret); \
		return ret; \
	}

#define CHECK_RETVAL(ret) \
	if(ret < 0) { \
		Log::E(Log::TAG, "Failed to call %s in line %d, return %d.", __PRETTY_FUNCTION__, __LINE__, ret); \
        return; \
	}

#define CHECK_AND_RETDEF(ret, def) \
	if(ret < 0) { \
		Log::E(Log::TAG, "Failed to call %s in line %d, return %d.", __PRETTY_FUNCTION__, __LINE__, ret); \
		return def; \
	}

#define CHECK_AND_NOTIFY_ERROR(ret) \
	if(ret < 0) { \
		elastos::ErrCode::SetError(ret, std::string(__PRETTY_FUNCTION__) + " line:" + std::to_string(__LINE__)); \
	} \
    CHECK_ERROR(ret) \

#define CHECK_AND_NOTIFY_RETVAL(ret) \
	if(ret < 0) { \
		elastos::ErrCode::SetError(ret, std::string(__PRETTY_FUNCTION__) + " line:" + std::to_string(__LINE__)); \
	} \
    CHECK_RETVAL(ret) \

    /*** static function and variable ***/
    constexpr static const int UnknownError = -1;
    constexpr static const int UnimplementedError = -2;
    constexpr static const int NotFoundError = -3;
    constexpr static const int NotReadyError = -4;
    constexpr static const int InvalidArgument = -5;
    constexpr static const int IOSystemException = -6;
    constexpr static const int NetworkException = -7;
    constexpr static const int PointerReleasedError = -8;
    constexpr static const int DevUUIDError = -9;
    constexpr static const int FileNotExistsError = -10;
    constexpr static const int JsonParseException = -11;
    constexpr static const int ConflictWithExpectedError = -12;
    constexpr static const int MergeInfoFailed = -13;
    constexpr static const int IgnoreMergeOldInfo = -14;
    constexpr static const int EmptyInfoError = -15;
    constexpr static const int InvalidFriendCode = -16;
    constexpr static const int RepeatOperationError = -17;

    constexpr static const int InvalidLocalDataDir = -50;
    constexpr static const int NoSecurityListener = -51;
    constexpr static const int BadSecurityValue = -52;
    constexpr static const int KeypairError = -53;

    constexpr static const int ChannelFailedPresetAll = -200;
    constexpr static const int ChannelFailedOpenAll = -201;
    constexpr static const int ChannelFailedCloseAll = -202;
    constexpr static const int ChannelFailedMultiOpen = -203;
    constexpr static const int ChannelFailedCarrier = -204;
    constexpr static const int ChannelFailedFriendExists = -205;
    constexpr static const int ChannelFailedFriendSelf = -206;
    constexpr static const int ChannelNotFound = -207;
    constexpr static const int ChannelNotReady = -208;
    constexpr static const int ChannelNotEstablished = -209;
    constexpr static const int ChannelNotOnline = -210;
    constexpr static const int ChannelNotSendMessage = -211;
    constexpr static const int ChannelDataTooLarge = -212;
	constexpr static const int ChannelFailedFileTrans = -213;
	constexpr static const int ChannelFileTransBusy = -214;
	constexpr static const int ChannelFailedReadData = -215;
	constexpr static const int ChannelInvalidDataId = -216;

    constexpr static const int BlkChnSetPropError = -300;
    constexpr static const int BlkChnGetPropError = -301;
    constexpr static const int BlkChnEmptyPropError = -302;
    constexpr static const int BlkChnBadTxIdError = -303;
    constexpr static const int BlkChnBadUpdateTimeError = -304;
    constexpr static const int BlkChnOldUpdateTimeError = -305;
    constexpr static const int BlkChnMonStoppedError = -306;
	constexpr static const int BlkChnIgnoreCacheProp = -307;


    constexpr static const int HttpClientError = -500;

    constexpr static const int StdSystemErrorIndex = -1000;

	static void SetErrorListener(std::function<void(int, const std::string&, const std::string&)> listener);
	static void SetError(int errCode, const std::string& ext);
    static std::string ToString(int errCode);

    /*** class function and variable ***/

private:
    /*** type define ***/

    /*** static function and variable ***/
	static std::function<void(int, const std::string&, const std::string&)> sErrorListener;

    /*** class function and variable ***/
    explicit ErrCode() = delete;
    virtual ~ErrCode() = delete;

}; // class ErrCode

} // namespace elastos

#endif /* _ELASTOS_ERRCODE_HPP_ */
