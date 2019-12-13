package com.elastos.microservice.hashaddressmapping;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.net.http.SslError;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.webkit.SslErrorHandler;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import org.elastos.sdk.elephantwallet.contact.Contact;
import org.json.JSONArray;
import org.json.JSONException;

import java.util.ArrayList;
import java.util.List;

import static com.elastos.microservice.hashaddressmapping.Helper.DialogActionClear;

public class MainActivity extends Activity implements ContactApi.MsgListener {
    public static final String TAG = "MainActivity";
    private int EDIT_OK = 100;
    private  WebView mWebView;
    private EditText mEditText;
    private Context mContext;
    private ContactApi mContactApi;
    private ProgressBar mProgressBar;
    private Handler mMainHandler = new Handler(Looper.getMainLooper());
    private JSONArray mHistoryAddress = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this.getApplicationContext();
        mContactApi = new ContactApi(mContext);
        layoutView();
        readLocalHistory();
        mContactApi.startContact();
    }

    private void layoutView() {
        //获得控件
        mWebView = (WebView) findViewById(R.id.CarrierBrowser);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            mWebView.getSettings().setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);
        }

        //系统默认会通过手机浏览器打开网页，为了能够直接通过WebView显示网页，则必须设置
        mWebView.setWebViewClient(new WebViewClient() {
            @Override
            public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
                handler.proceed();
            }@Override
            public void onPageStarted(WebView view, String url, Bitmap favicon) {
                mProgressBar.setVisibility(View.VISIBLE);
                super.onPageStarted(view, url, favicon);
            }

            @Override
            public void onPageFinished(WebView view, String url) {
                mProgressBar.setVisibility(View.GONE);
                Log.d(TAG,"WebView load url:"+url);
                if (!url.equals("about:blank")) {
                    addHistoryAddress(mEditText.getText().toString());
                }
                super.onPageFinished(view, url);
            }
        });

        mWebView.getSettings().setJavaScriptEnabled(true);
        mProgressBar = (ProgressBar) findViewById(R.id.progressBar);
        mProgressBar.setVisibility(View.GONE);
        mEditText = (EditText) findViewById(R.id.UrlInput);
        mEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            //输入时的调用
            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                Log.d(TAG, "onTextChanged() returned: ");
                mHandler.removeCallbacks(mRunnable);
                //800毫秒没有输入认为输入完毕
                mHandler.postDelayed(mRunnable, 1000);
            }

            @Override
            public void afterTextChanged(Editable editable) {
                Log.d(TAG, "onTextChanged() returned: 2222");
            }
        });
    }
    private void loadUrl(String src_url) {
        if (src_url.toLowerCase().startsWith("carrier")) {
            String[]urls = src_url.split("/");
            if (urls.length >=2) {
                String hash_addr = urls[2];
                mContactApi.requireRealUrl(hash_addr, MainActivity.this);
            } else {
                Toast.makeText(mContext, R.string.invalid_url, Toast.LENGTH_LONG).show();
            }
        } else {
            Toast.makeText(mContext, R.string.invalid_url, Toast.LENGTH_LONG).show();
        }
    }
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (EDIT_OK == msg.what) {
                Log.d(TAG, "handleMessage() returned:输入完成 " );
                String src_url = mEditText.getText().toString();
                loadUrl(src_url);
            }
        }
    };

    private Runnable mRunnable = new Runnable() {
        @Override
        public void run() {
            mHandler.sendEmptyMessage(EDIT_OK);
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.scan_qrcode) {
            scanUserInfo();
            return true;
        } else if (id == R.id.about_id) {
            showGetUserInfo();
            return true;
        } else if (id == R.id.history_id) {
            showHistory();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
    private void showHistory() {
        List<String> addressList =new ArrayList<String>();
        for(int i=0; i<mHistoryAddress.length(); i++) {
            try {
                addressList.add(mHistoryAddress.getString(i));
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
        Helper.showAddressList(this, addressList, new Helper.OnListener() {
            @Override
            public void onResult(String address) {
                mEditText.setText(address);
            }
            @Override
            public void onAction(int action) {
                if (action == DialogActionClear) {
                    clearHistoryAddress();
                }
            }
        });

    }
    private String showGetUserInfo() {
        Contact.UserInfo info = mContactApi.getUserInfo();
        if(info == null) {
            return "Failed to get user info.";
        }
        Helper.showDetails(MainActivity.this, info.toJson());
        return info.toString();
    }

    private String scanUserInfo() {
        Helper.scanAddress(this, result -> {
            Log.d(TAG, "result:"+result);
            Helper.showAddFriend(this, result, (summary) -> {
                int ret = mContactApi.addFriend(result, summary);
                if(ret < 0) {
                    Log.e(TAG, "Failed to add friend. ret=" + ret);
                }
            });
        });
        return "";
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        Helper.onRequestPermissionsResult(this, requestCode, permissions, grantResults);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        Helper.onActivityResult(this, requestCode, resultCode, data);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mContactApi.stopContact();
        Process.killProcess(Process.myPid());
    }

    @Override
    public void onReceiveRealUrl(String url) {
        mMainHandler.post(new Runnable() {
            @Override
            public void run() {
                mWebView.loadUrl(url);//访问网页
            }
        });
    }

    private void readLocalHistory() {
        SharedPreferences sp = mContext.getSharedPreferences("HashAddressMapping", Context.MODE_PRIVATE);
        try {
            String cache_string = sp.getString("history", null);
            if (cache_string != null) {
                mHistoryAddress = new JSONArray(cache_string);
            }
        } catch (JSONException e) {
            Log.e(TAG, "readLocalHistory  history cache failed ", e);
        }
        if (mHistoryAddress == null) {
            mHistoryAddress = new JSONArray();
        }
    }
    private void addHistoryAddress(String address) {
        boolean found = false;
        for(int i=0; i<mHistoryAddress.length(); i++) {
            try {
                if (mHistoryAddress.getString(i).equals(address)) {
                    found = true;
                    break;
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
        if (!found) {
            mHistoryAddress.put(address);
            SharedPreferences sp = mContext.getSharedPreferences("HashAddressMapping", Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = sp.edit();
            editor.putString("history", mHistoryAddress.toString());
            editor.commit();
        }
    }

    private void clearHistoryAddress() {
        mHistoryAddress = new JSONArray();
        SharedPreferences sp = mContext.getSharedPreferences("HashAddressMapping", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sp.edit();
        editor.putString("history", mHistoryAddress.toString());
        editor.commit();
    }
}
