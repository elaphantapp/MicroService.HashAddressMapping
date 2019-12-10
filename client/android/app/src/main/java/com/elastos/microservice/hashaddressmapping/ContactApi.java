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

import org.elastos.sdk.elephantwallet.contact.Contact;
import org.elastos.sdk.elephantwallet.contact.internal.AcquireArgs;
import org.elastos.sdk.elephantwallet.contact.internal.ContactChannel;
import org.elastos.sdk.elephantwallet.contact.internal.EventArgs;
import org.elastos.sdk.keypair.ElastosKeypair;
import org.json.JSONException;
import org.json.JSONObject;

import static com.elastos.microservice.hashaddressmapping.MainActivity.TAG;

public class ContactApi {
    private Context mContext;
    private Contact mContact;
    private Contact.Listener mContactListener;
    private Contact.DataListener mContactDataListener;
    private String ErrorPrefix = "ContactApi";
    private static final String KeypairLanguage = "english";
    private static final String KeypairWords = "";
    private String mSavedMnemonic;
    private static final String SavedMnemonicKey = "mnemonic";
    private static final String UrlKey = "urlkey";
    private static final String FriendAddressKey = "friendaddr";
    private static final String Summary = "summary";
    private MsgListener mMsgListener = null;
    private JSONObject mUrls = null;
    private String mUrlKey = null;

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
        if (mContact == null) { // noneed to restart
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
        if (mContact == null) {
            return -1;
        }

        return mContact.addFriend(friendCode, summary);
    }

    private String getDeviceId() {
        String devId = Settings.Secure.getString(mContext.getContentResolver(), Settings.Secure.ANDROID_ID);
        return devId;
    }

    private String NewContact() {
        if (mContact != null) {
            return "Contact is created.";
        }

        Contact.Factory.SetLogLevel(7);

        Contact.Factory.SetDeviceId(getDeviceId());

        int ret = Contact.Factory.SetLocalDataDir(mContext.getCacheDir().getAbsolutePath());
        if (ret < 0) {
            return "Failed to call Contact.Factory.SetLocalDataDir() ret=" + ret;
        }

        mContact = Contact.Factory.Create();
        if (mContact == null) {
            return "Failed to call Contact.Factory.Create()";
        }

        if (mContactListener != null) {
            mContactListener = null;
        }
        mContactListener = new Contact.Listener() {
            @Override
            public byte[] onAcquire(AcquireArgs request) {
                byte[] ret = processAcquire(request);

                String msg = "onAcquire(): req=" + request + "\n";
                msg += "onAcquire(): resp=" + ret + "\n";
                showEvent(msg);
                return ret;
            }

            @Override
            public void onEvent(EventArgs event) {
                processEvent(event);
                String msg = "onEvent(): ev=" + event + "\n";
                showEvent(msg);
            }

            @Override
            public void onReceivedMessage(String humanCode, ContactChannel channelType, Contact.Message message) {
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

            @Override
            public void onError(int errCode, String errStr, String ext) {
                String msg = errCode + ": " + errStr;
                msg += "\n" + ext;
                showError(msg);
            }
        };

        mContact.setListener(mContactListener);
        return "Success to create a contact instance.";
    }

    private byte[] processAcquire(AcquireArgs request) {
        byte[] response = null;
        switch (request.type) {
            case PublicKey:
                response = getPublicKey().getBytes();
                break;
            case EncryptData:
                response = request.data; // plaintext
                break;
            case DecryptData:
                response = request.data; // plaintext
                break;
            case DidPropAppId:
//                String appId = "DC92DEC59082610D1D4698F42965381EBBC4EF7DBDA08E4B3894D530608A64AAA65BB82A170FBE16F04B2AF7B25D88350F86F58A7C1F55CC29993B4C4C29E405";
//                response = appId.getBytes();
                response = null;
                break;
            case DidAgentAuthHeader:
                response = null;
                break;
            case SignData:
                response = signData(request.data);
                break;
            default:
                throw new RuntimeException("Unprocessed request: " + request);
        }

        return response;
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

    private void processEvent(EventArgs event) {
        switch (event.type) {
            case StatusChanged:
                break;
            case FriendRequest:
                Contact.Listener.RequestEvent requestEvent = (Contact.Listener.RequestEvent) event;
                mContact.acceptFriend(requestEvent.humanCode);
                break;
            case HumanInfoChanged:
                Contact.Listener.InfoEvent infoEvent = (Contact.Listener.InfoEvent) event;
                String msg = event.humanCode + " info changed: " + infoEvent.toString();
                showEvent(msg);
                break;
            default:
                Log.w(TAG, "Unprocessed event: " + event);
        }
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
        if (mContact == null) {
            return ErrorPrefix + "Contact is null.";
        }

        int ret = mContact.start();
        if (ret < 0) {
            return "Failed to start contact instance. ret=" + ret;
        }

        return "Success to start contact instance.";
    }

    private String StopContact() {
        if (mContact == null) {
            return ErrorPrefix + "Contact is null.";
        }

        int ret = mContact.stop();
        if (ret < 0) {
            return "Failed to stop contact instance. ret=" + ret;
        }
        return "Success to stop contact instance.";
    }

}
