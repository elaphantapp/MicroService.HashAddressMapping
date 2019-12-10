package com.elastos.microservice.hashaddressmapping;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Handler;
import android.os.Looper;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.core.app.ActivityCompat;

import com.blikoon.qrcodescanner.QrCodeActivity;

import static com.elastos.microservice.hashaddressmapping.MainActivity.TAG;

public class Helper {
    private static AlertDialog mLastDialog;
    public interface OnListener {
        void onResult(String result);
    };
    public static void showAddFriend(Context context, String friendCode, OnListener listener) {
        EditText edit = new EditText(context);
        View root = makeEditView(context, friendCode, edit);

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Find Address");
        builder.setView(root);
        builder.setNegativeButton("Cancel", (dialog, which) -> {
            dismissDialog();
        });
        builder.setPositiveButton("Add Friend", (dialog, which) -> {
            listener.onResult(edit.getText().toString());
        });

        showDialog(builder);
    }

    private static void showDialog(AlertDialog.Builder builder) {
        new Handler(Looper.getMainLooper()).post(() -> {
            if(mLastDialog != null) {
                mLastDialog.dismiss();
            }
            mLastDialog = builder.create();
            mLastDialog.show();
        });
    }

    public static void dismissDialog() {
        new Handler(Looper.getMainLooper()).post(() -> {
            if(mLastDialog == null) {
                return;
            }
            mLastDialog.dismiss();
            mLastDialog = null;
        });

    }
    private static View makeEditView(Context context, String friendCode, EditText edit) {
        TextView txtCode = new TextView(context);
        TextView txtMsg = new TextView(context);

        LinearLayout root = new LinearLayout(context);
        root.setOrientation(LinearLayout.VERTICAL);
        root.addView(txtCode);
        root.addView(txtMsg);
        root.addView(edit);

        txtCode.setText("FriendCode: \n  " + friendCode);
        txtMsg.setText("Message:");
        edit.setText("{\"serviceName\":\"HashAddressMappingService\",\"content\":\"hello\"}");

        return root;
    }

    public static void scanAddress(MainActivity activity, OnListener listener) {
        mOnResultListener = listener;

        int hasCameraPermission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.CAMERA);
        if(hasCameraPermission == PackageManager.PERMISSION_GRANTED) {
            Intent intent = new Intent(activity, QrCodeActivity.class);
            activity.startActivityForResult(intent, REQUEST_CODE_QR_SCAN);
        } else {
            ActivityCompat.requestPermissions(activity,
                    new String[]{Manifest.permission.CAMERA},
                    1);
        }
    }

    public static void selectPhoto(MainActivity activity, OnListener listener) {
        mOnResultListener = listener;

        int hasCameraPermission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.READ_EXTERNAL_STORAGE);
        if(hasCameraPermission == PackageManager.PERMISSION_GRANTED) {
            Intent intent = new Intent(Intent.ACTION_PICK, null);
            intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/*");
            activity.startActivityForResult(intent, REQUEST_CODE_SEL_PHOTO);
        } else {
            ActivityCompat.requestPermissions(activity,
                    new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                    1);
        }
    }

    public static void onRequestPermissionsResult(MainActivity activity, int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode != 1) {
            return;
        }

        for (int idx = 0; idx < permissions.length; idx++) {
            if(permissions[idx].equals(Manifest.permission.CAMERA) == false) {
                continue;
            }

            if (grantResults[idx] == PackageManager.PERMISSION_GRANTED) {
                Intent intent = new Intent(activity, QrCodeActivity.class);
                activity.startActivityForResult(intent, REQUEST_CODE_QR_SCAN);
            }
        }
    }

    public static void onActivityResult(MainActivity activity, int requestCode, int resultCode, Intent data) {
        if(resultCode != Activity.RESULT_OK) {
            Log.d(TAG,"COULD NOT GET A GOOD RESULT.");
            if(data == null) {
                return;
            }
            String result = data.getStringExtra("com.blikoon.qrcodescanner.error_decoding_image");
            if(result == null) {
                return;
            }

           Log.d(TAG,"QR Code could not be scanned.");
        }

        if(requestCode == REQUEST_CODE_QR_SCAN) {
            if(data==null)
                return;
            //Getting the passed result
            String result = data.getStringExtra("com.blikoon.qrcodescanner.got_qr_scan_relult");
            Log.d(TAG,"Scan result:"+ result);

            mOnResultListener.onResult(result);
            mOnResultListener = null;
        } else if(requestCode == REQUEST_CODE_SEL_PHOTO) {
            Uri uri = data.getData();
            String result = FileUtils.getFilePathByUri(activity, uri);

            mOnResultListener.onResult(result);
            mOnResultListener = null;
        }
    }

    private static OnListener mOnResultListener;
    private static final int REQUEST_CODE_QR_SCAN = 101;
    private static final int REQUEST_CODE_SEL_PHOTO = 102;
}
