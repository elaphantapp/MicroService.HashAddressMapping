package com.elastos.microservice.hashaddressmapping;

import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Looper;
import android.os.Process;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.util.Log;
import android.widget.Toast;

import org.elastos.sdk.elephantwallet.contact.Contact;
import org.elastos.sdk.elephantwallet.contact.Utils;
import org.elastos.sdk.keypair.ElastosKeypair;
import org.json.JSONException;
import org.json.JSONObject;

import app.elaphant.sdk.peernode.PeerNode;
import app.elaphant.sdk.peernode.PeerNodeListener;

import static com.elastos.microservice.hashaddressmapping.MainActivity.TAG;

public class ContactApi {
    private String ErrorPrefix = "ContactApi";
    private static final String KeypairLanguage = "english";
    private static final String KeypairWords = "";
    private static final String SavedMnemonicKey = "mnemonic";
    private static final String UrlKey = "urlkey";
    private static final String FriendAddressKey = "friendaddr";
    private static final String Summary = "summary";

    private static final int SERVER_STATE_NOT_CONNECT = 0;
    private static final int SERVER_STATE_CONNECTED = 1;
    private static final int SERVER_STATE_CONNECT_FAILED = 2;

    private String mSavedMnemonic;
    private String mUrlKey = null;
    private Handler mHandler = new Handler(Looper.getMainLooper());
    private MsgListener mMsgListener = null;
    private JSONObject mUrls = null;
    private Context mContext;
    private PeerNode mPeerNode;
    private String mSelfHumanCode;
    public interface MsgListener {
        public void onReceiveRealUrl(String url);
    }

    ContactApi(Context context) {
        mContext = context;
        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(mContext);
        mSavedMnemonic = pref.getString(SavedMnemonicKey, null);
        String devId = getDeviceId();
        Log.i(TAG, "Device ID:" + devId);
        if (mSavedMnemonic == null) {
            mSavedMnemonic = ElastosKeypair.generateMnemonic(KeypairLanguage, KeypairWords);
            newAndSaveMnemonic(mSavedMnemonic);
        }
        showMessage("Mnemonic:\n" + mSavedMnemonic);
    }

    public void requireRealUrl(String url_key, MsgListener listener) {
        Log.d(TAG, "requireRealUrl url_key:" + url_key);
        mMsgListener = listener;
        mUrlKey = url_key;

        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(mContext);
        String data = pref.getString(UrlKey, null);
        notifyRealUrl(data, false);
    }

    private void notifyRealUrl(String data, boolean need_save) {
        try {
            if (data != null && !data.isEmpty()) {
                mUrls = new JSONObject(data);
                if (need_save) {
                    SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(mContext);
                    SharedPreferences.Editor editor = pref.edit();
                    editor.putString(UrlKey, data).commit();
                }
            }

            if (mUrls != null && mMsgListener != null) {
                String target_url = mUrls.optString(mUrlKey, "");
                mMsgListener.onReceiveRealUrl(target_url);
                mMsgListener = null;
            } else {
                //显示
                showServerState(SERVER_STATE_NOT_CONNECT);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    private String newAndSaveMnemonic(final String newMnemonic) {
        mSavedMnemonic = newMnemonic;
        if (mSavedMnemonic == null) {
            mSavedMnemonic = ElastosKeypair.generateMnemonic(KeypairLanguage, KeypairWords);
        }

        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(mContext);
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(SavedMnemonicKey, mSavedMnemonic).commit();
        if (mPeerNode == null) { // noneed to restart
            return ("Success to save mnemonic:\n" + mSavedMnemonic);
        }

        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
        builder.setMessage("New mnemonic can only be valid after restart,\ndo you want restart app?");
        builder.setPositiveButton("Restart", (dialog, which) -> {
            // restart
            Intent mStartActivity = new Intent(mContext, MainActivity.class);
            int mPendingIntentId = 123456;
            PendingIntent mPendingIntent = PendingIntent.getActivity(mContext, mPendingIntentId, mStartActivity, PendingIntent.FLAG_CANCEL_CURRENT);
            AlarmManager mgr = (AlarmManager) mContext.getSystemService(Context.ALARM_SERVICE);
            mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 100, mPendingIntent);
            Process.killProcess(Process.myPid());
        });
        builder.setNegativeButton("Cancel", (dialog, which) -> {
            dialog.dismiss();
        });
        new Handler(Looper.getMainLooper()).post(() -> {
            builder.create().show();
        });

        return ("Cancel to save mnemonic:\n" + newMnemonic);
    }

    public void startContact() {
        NewContact();
        StartContact();
    }

    public void stopContact() {
        StopContact();
    }

    public int addFriend(String friendCode, String summary) {
        if (mPeerNode == null) {
            return -1;
        }

        return mPeerNode.addFriend(friendCode, summary);
    }

    private String getDeviceId() {
        String devId = Settings.Secure.getString(mContext.getContentResolver(), Settings.Secure.ANDROID_ID);
        return devId;
    }
    private byte[] getAgentAuthHeader() {
        String appid = "org.elastos.debug.didplugin";
        String appkey = "b2gvzUM79yLhCbbGNWCuhSsGdqYhA7sS";
        long timestamp = System.currentTimeMillis();
        String auth = Utils.getMd5Sum(appkey + timestamp);
        String headerValue = "id=" + appid + ";time=" + timestamp + ";auth=" + auth;
        Log.i(TAG, "getAgentAuthHeader() headerValue=" + headerValue);

        return headerValue.getBytes();
    }
    private void showServerState(final int serverState) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                switch (serverState) {
                    case SERVER_STATE_CONNECTED:
                        Toast.makeText(mContext, R.string.server_connected, Toast.LENGTH_LONG).show();
                        break;
                    case SERVER_STATE_NOT_CONNECT:
                        Toast.makeText(mContext, R.string.server_not_connect, Toast.LENGTH_LONG).show();
                        break;
                    case SERVER_STATE_CONNECT_FAILED:
                        Toast.makeText(mContext, R.string.server_connect_failed, Toast.LENGTH_LONG).show();
                        break;
                }
            }
        });
    }

    private String NewContact() {
        mPeerNode = PeerNode.getInstance(mContext.getFilesDir().getAbsolutePath(),getDeviceId());
        mPeerNode.setListener(new PeerNodeListener.Listener() {
            @Override
            public byte[] onAcquire(Contact.Listener.AcquireArgs request) {
                byte[] response = null;
                switch (request.type) {
                    case PublicKey:
                        response = getPublicKey().getBytes();
                        break;
                    case EncryptData:
                        response = request.data;
                        break;
                    case DecryptData:
                        response = request.data;
                        break;
                    case DidPropAppId:
                        break;
                    case DidAgentAuthHeader:
                        response = getAgentAuthHeader();
                        break;
                    case SignData:
                        response = signData(request.data);
                        break;
                    default:
                        throw new RuntimeException("Unprocessed request: " + request);
                }
                return response;
            }

            @Override
            public void onError(int errCode, String errStr, String ext) {
                showError("Contact error: " + errCode + " " + errStr);
            }
        });
        mPeerNode.addMessageListener("HashAddressMappingService", new PeerNodeListener.MessageListener() {
            @Override
            public void onEvent(Contact.Listener.EventArgs event) {
                switch (event.type) {
                    case StatusChanged:
                        Contact.Listener.StatusEvent statusEvent = (Contact.Listener.StatusEvent) event;
                        if (!statusEvent.humanCode.equals(mSelfHumanCode)) {
                            showServerState(SERVER_STATE_CONNECTED);
                        }
                        break;
                    case FriendRequest:
                        Contact.Listener.RequestEvent requestEvent = (Contact.Listener.RequestEvent) event;
                        mPeerNode.acceptFriend(requestEvent.humanCode);
                        break;
                    case HumanInfoChanged:
                        Contact.Listener.InfoEvent infoEvent = (Contact.Listener.InfoEvent) event;
                        String msg = event.humanCode + " info changed: " + infoEvent.toString();
                        showEvent(msg);
                        break;
                    default:
                        Log.w(TAG, "Unprocessed event: " + event);
                }
                String msg = "onEvent(): ev=" + event.toString() + "\n";
                showEvent(msg);
            }

            @Override
            public void onReceivedMessage(String humanCode, Contact.Channel channelType, Contact.Message message) {
                String msg = "onRcvdMsg(): data=" + message.data + "\n";
                msg += "onRcvdMsg(): type=" + message.type + "\n";
                msg += "onRcvdMsg(): crypto=" + message.cryptoAlgorithm + "\n";
                showEvent(msg);
                if (message.type == Contact.Message.Type.MsgText && message.data != null) {
                    try {
                        JSONObject data_json = new JSONObject(message.data.toString());
                        String data = data_json.getString("content");
                        notifyRealUrl(data, true);
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }

                }
            }
        });
        Contact.UserInfo userInfo = mPeerNode.getUserInfo();
        if (userInfo != null) {
            mSelfHumanCode = userInfo.humanCode;
        }

        return "Success to create a contact instance.";
    }

    private byte[] signData(byte[] data) {
        String privKey = getPrivateKey();

        ElastosKeypair.Data originData = new ElastosKeypair.Data();
        originData.buf = data;
        ElastosKeypair.Data signedData = new ElastosKeypair.Data();
        int signedSize = ElastosKeypair.sign(privKey, originData, originData.buf.length, signedData);
        if (signedSize <= 0) {
            return null;
        }

        return signedData.buf;
    }

    public void showMessage(String msg) {
        Log.i(TAG, msg);
    }

    public void showError(String newErr) {
        Log.e(TAG, "showError:" + newErr);
    }

    private String getPublicKey() {
        ElastosKeypair.Data seedData = new ElastosKeypair.Data();
        int seedSize = ElastosKeypair.getSeedFromMnemonic(seedData, mSavedMnemonic,
                KeypairLanguage, KeypairWords, "");
        String pubKey = ElastosKeypair.getSinglePublicKey(seedData, seedSize);
        return pubKey;
    }

    private String getPrivateKey() {
        ElastosKeypair.Data seedData = new ElastosKeypair.Data();
        int seedSize = ElastosKeypair.getSeedFromMnemonic(seedData, mSavedMnemonic,
                KeypairLanguage, KeypairWords, "");
        String privKey = ElastosKeypair.getSinglePrivateKey(seedData, seedSize);
        return privKey;
    }

    private void showEvent(String msg) {
        Log.d(TAG, "showEvent:" + msg);
    }

    private String StartContact() {
        if (mPeerNode == null) {
            return ErrorPrefix + "PeerNode is null.";
        }

        int ret = mPeerNode.start();
        if (ret < 0) {
            return "Failed to start PeerNode instance. ret=" + ret;
        }

        return "Success to start PeerNode instance.";
    }

    private String StopContact() {
        if (mPeerNode == null) {
            return ErrorPrefix + "PeerNode is null.";
        }
        int ret = mPeerNode.stop();
        if (ret < 0) {
            return "Failed to stop PeerNode instance. ret=" + ret;
        }
        return "Success to stop PeerNode instance.";
    }

}
