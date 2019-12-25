package com.elastos.microservice.hashaddressmapping;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.os.Handler;
import android.os.Looper;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import androidx.core.app.ActivityCompat;

import com.blikoon.qrcodescanner.QrCodeActivity;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.WriterException;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;

import java.util.HashMap;
import java.util.List;

import static com.elastos.microservice.hashaddressmapping.MainActivity.TAG;

public class Helper {
    private static AlertDialog mLastDialog;
    public static int DialogActionCancel = 0;
    public static int DialogActionClear = 1;

    public interface OnListener {
        void onResult(String result);
        default void onAction(int action) {};//action 0:cancel,1:clear
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
            dismissDialog();
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

    public static void showAddressList(Context context, List<String> addressList, OnListener listener) {
        ListView listView = new ListView(context);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(context, android.R.layout.simple_list_item_1, addressList);
        listView.setAdapter(adapter);
        listView.setOnItemClickListener((parent, view, position, id) -> {
            listener.onResult(((TextView)view).getText().toString());
            dismissDialog();
        });

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("History");
        builder.setView(listView);
        builder.setNegativeButton("Cancel", (dialog, which) -> {
            listener.onAction(DialogActionCancel);
            dismissDialog();
        });
        builder.setNeutralButton("Clear", (dialog, which) -> {
            listener.onAction(DialogActionClear);
            dismissDialog();
        });

        showDialog(builder);
    }
    public static void showDetails(Context context, String msg) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Details");
        builder.setMessage(msg);
        builder.setNegativeButton("Cancel", (dialog, which) -> {
            dialog.dismiss();
        });

        builder.create().show();
    }

    private static Bitmap makeQRCode(String value) {
        HashMap<EncodeHintType, ErrorCorrectionLevel> hintMap = new HashMap<>();
        hintMap.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L);
        BitMatrix matrix = null;
        try {
            matrix = new MultiFormatWriter().encode(value, BarcodeFormat.QR_CODE, 512, 512, hintMap);
        } catch (WriterException e) {
            Log.e(TAG, "Failed to MultiFormatWriter().encode()", e);
            throw new RuntimeException("Failed to MultiFormatWriter().encode()", e);
        }

        //converting bitmatrix to bitmap
        int width = matrix.getWidth();
        int height = matrix.getHeight();
        int[] pixels = new int[width * height];
        // All are 0, or black, by default
        for (int y = 0; y < height; y++) {
            int offset = y * width;
            for (int x = 0; x < width; x++) {
                pixels[offset + x] = matrix.get(x, y) ? Color.BLACK : Color.WHITE;
            }
        }

        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap.setPixels(pixels, 0, width, 0, 0, width, height);

        return bitmap;
    }

    private static View makeAddressView(Context context, HashMap<String, String> humanCode, String presentDevId, String ext,
                                        OnListener listener) {
        TextView txtDevId = new TextView(context);
        ImageView image = new ImageView(context);
        TextView txtCode = new TextView(context);
        RadioGroup radioGrp = new RadioGroup(context);
        Button btn = new Button(context);

        radioGrp.setOnCheckedChangeListener((group, checkedId) -> {
            RadioButton checkedView = group.findViewById(checkedId);
            int mapIdx = group.indexOfChild(checkedView);
            String checkedVal = (String) humanCode.values().toArray()[mapIdx];
            String showed = checkedVal;
            if(mapIdx == humanCode.size() - 1) {
                showed += "\n----------------\n" + ext;
            }
            Bitmap bitmap = makeQRCode(checkedVal);
            image.setImageBitmap(bitmap);
            txtCode.setText(showed);
        });
        for(HashMap.Entry<String, String> entry : humanCode.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue();

            RadioButton radiobtn = new RadioButton(context);
            radiobtn.setText(key + ": " + value.substring(0, 5) + " ... " + value.substring(value.length()-5));
            radiobtn.setId(View.generateViewId());

            radioGrp.addView(radiobtn);
            if(radioGrp.getChildCount() == 1) {
                radiobtn.setChecked(true);
            }
        }

        LinearLayout root = new LinearLayout(context);
        root.setOrientation(LinearLayout.VERTICAL);
        if(presentDevId != null) {
            root.addView(txtDevId);
            txtDevId.setText("Present DevId: " + presentDevId);
        }
        root.addView(image);
        root.addView(txtCode);
        root.addView(radioGrp);
        root.addView(btn);

        ViewGroup.MarginLayoutParams txtLayout = (ViewGroup.MarginLayoutParams) txtCode.getLayoutParams();
        txtLayout.setMargins(20, 10, 20, 20);

        btn.setText("Details");
        btn.setOnClickListener((v) -> {
            listener.onResult(null);
        });

        return root;
    }

    public static void showAddress(Context context, HashMap<String, String> humanCode, String presentDevId, String ext,
                                   OnListener listener) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("My Address");
        try {
            View root = makeAddressView(context, humanCode, presentDevId, ext, listener);
            builder.setView(root);
        } catch (Exception e) {
            String msg = "Failed to show address.";
            builder.setMessage(msg + e);
            Log.w(TAG, msg, e);
        }

        builder.setNegativeButton("Cancel", (dialog, which) -> {
            dismissDialog();
        });

        showDialog(builder);
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
