///**
// * @file	FileManager.hpp
// * @brief	FileManager
// * @details
// *
// * @author	xxx
// * @author	<xxx@xxx.com>
// * @copyright	(c) 2012 xxx All rights reserved.
// **/
//
//#ifndef _ELASTOS_FILE_MANAGER_HPP_
//#define _ELASTOS_FILE_MANAGER_HPP_
//
//#include "Config.hpp"
//#include "ErrCode.hpp"
//#include "HumanInfo.hpp"
//#include "SecurityManager.hpp"
//#include "UserManager.hpp"
//#include "FriendManager.hpp"
//
//#include "FileChannelStrategy.hpp"
//
//namespace elastos {
//
//class FileManager : public std::enable_shared_from_this<FileManager> {
//public:
//    /*** type define ***/
//    struct FileInfo {
//    public:
//        FileType mType;
//        std::vector<uint8_t> mPlainContent;
//        std::string mCryptoAlgorithm;
//        uint64_t mTimeStamp;
//    private:
//        explicit FileInfo(FileType type,
//                             const std::vector<uint8_t>& plainContent,
//                             const std::string& cryptoAlgorithm);
//        explicit FileInfo(const FileInfo& info,
//                             bool ignoreContent = true);
//        explicit FileInfo();
//        virtual ~FileInfo();
//
//        friend FileManager;
//    };
//
//    class FileListener : public MessageChannelStrategy::ChannelListener {
//    public:
//        explicit FileListener() = default;
//        virtual ~FileListener() = default;
//
//        virtual void onStatusChanged(std::shared_ptr<UserInfo> userInfo,
//                                     ChannelType channelType,
//                                     UserInfo::Status status) = 0;
//
//        virtual void onReceivedFile(std::shared_ptr<HumanInfo> humanInfo,
//                                       ChannelType channelType,
//                                       const std::shared_ptr<FileInfo> msgInfo) = 0;
//
//        virtual void onSentFile(int msgIndex, int errCode) override = 0;
//
//        virtual void onFriendRequest(std::shared_ptr<FriendInfo> friendInfo,
//                                     ChannelType channelType,
//                                     const std::string& summary) = 0;
//
//        virtual void onFriendStatusChanged(std::shared_ptr<FriendInfo> friendInfo,
//                                           ChannelType channelType,
//                                           FriendInfo::Status status) = 0;
//
//        virtual void onHumanInfoChanged(std::shared_ptr<HumanInfo> humanInfo,
//                                        ChannelType channelType) = 0;
//
//    private:
//        virtual void onStatusChanged(const std::string& userCode,
//                                     uint32_t channelType,
//                                     ChannelStatus status) override;
//
//        virtual void onReceivedFile(const std::string& friendCode,
//                                       uint32_t channelType,
//                                       const std::vector<uint8_t>& msgContent) override;
//
//        //virtual void onSentFile(int msgIndex, int errCode) override;
//
//        virtual void onFriendRequest(const std::string& friendCode,
//                                     uint32_t channelType,
//                                     const std::string& summary) override;
//
//        virtual void onFriendStatusChanged(const std::string& friendCode,
//                                           uint32_t channelType,
//                                           ChannelStatus status) override;
//
//        std::weak_ptr<FileManager> mFileManager;
//        friend class FileManager;
//    };
//
//    /*** static function and variable ***/
//    static std::shared_ptr<FileInfo> MakeEmptyFile();
//
//    static std::shared_ptr<FileInfo> MakeFile(FileType type,
//                                                    const std::vector<uint8_t>& plainContent,
//                                                    const std::string& cryptoAlgorithm = "");
//    static std::shared_ptr<FileInfo> MakeTextFile(const std::string& plainContent,
//                                                        const std::string& cryptoAlgorithm = "");
//
//
//    /*** class function and variable ***/
//    explicit FileManager(std::weak_ptr<SecurityManager> sectyMgr,
//                            std::weak_ptr<UserManager> userMgr,
//                            std::weak_ptr<FriendManager> friendMgr);
//    virtual ~FileManager();
//
//    virtual void setFileListener(std::shared_ptr<FileListener> listener);
//
//    virtual int presetChannels(std::weak_ptr<Config> config);
//    virtual int openChannels();
//    virtual int closeChannels();
//
//    virtual int requestFriend(const std::string& friendAddr,
//                              ChannelType chType,
//                              const std::string& summary,
//                              bool remoteRequest = true,
//                              bool forceRequest = false);
//
//    virtual int updateFriend(const std::string& did);
//
//    virtual int removeFriend(const std::string& friendCode, ChannelType humanChType);
//
////    virtual int monitorDidChainCarrierID(const std::string& did);
//
//    virtual int sendFile(const std::shared_ptr<HumanInfo> humanInfo,
//                            ChannelType chType,
//                            const std::shared_ptr<FileInfo> msgInfo,
//                            bool sendToOtherDev = true);
//
//    virtual int broadcastDesc(ChannelType chType);
//
//private:
//    /*** type define ***/
//
//    /*** static function and variable ***/
//    static std::shared_ptr<FileInfo> MakeFile(std::shared_ptr<FileInfo> from, bool ignoreContent = true);
//
//    /*** class function and variable ***/
//    template <class T>
//    int getChannel(ChannelType chType, std::weak_ptr<T>& channel);
//    int sendDescFile(const std::vector<std::shared_ptr<HumanInfo>>& humanList, ChannelType chType);
//
//    std::weak_ptr<SecurityManager> mSecurityManager;
//    std::weak_ptr<UserManager> mUserManager;
//    std::weak_ptr<FriendManager> mFriendManager;
//    std::shared_ptr<FileListener> mFileListener;
//
//    std::map<ChannelType, std::shared_ptr<FileChannelStrategy>> mFileChannelMap;
//
//    friend class UserManager;
//}; // class FileManager
//
///***********************************************/
///***** class template function implement *******/
///***********************************************/
//template <class T>
//int FileManager::getChannel(ChannelType chType, std::weak_ptr<T>& channel)
//{
//    const auto& it = mFileChannelMap.find(chType);
//    if(it == mFileChannelMap.end()) {
//        return ErrCode::NotFoundError;
//    }
//
//    auto ptr = std::dynamic_pointer_cast<T>(it->second);
//    if(ptr.get() == nullptr) {
//        return ErrCode::InvalidArgument;
//    }
//
//    channel = ptr;
//
//    return 0;
//}
//
//} // namespace elastos
//
//#endif /* _ELASTOS_FILE_MANAGER_HPP_ */
