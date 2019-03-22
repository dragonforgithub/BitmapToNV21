package com.arcsoft.bitmaptonv21;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v4.provider.DocumentFile;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.sdsmdg.tastytoast.TastyToast;
import com.yanzhenjie.permission.AndPermission;
import com.yanzhenjie.permission.PermissionListener;
import com.yanzhenjie.permission.Rationale;
import com.yanzhenjie.permission.RationaleListener;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static String TAG = "ArcSoft_NV21_Convert";
    private static String[] NEEDED_PERMISSIONS = new String[]{
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
    };

    private static final int REQUEST_PERMISSION_STORAGE = 200;
    private static final int RESULT_LOAD_IMAGE = 300;
    private static final int RESULT_LOAD_FILE = 400;

    private Activity mContext;
    private ImageView mImage;
    private Button mBtnJpg;
    private Button mBtnNv21;
    private InputStream is = null;
    private AssetManager am;
    private Bitmap mBitmap;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("arcsoft_bitmap_to_nv21");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this;

        mImage = findViewById(R.id.image);
        mBtnJpg = findViewById(R.id.btn_jpg);
        mBtnJpg.setOnClickListener(new clickListener());
        mBtnNv21 = findViewById(R.id.btn_nv21);
        mBtnNv21.setOnClickListener(new clickListener());


        AndPermission.with(mContext)
                .permission(NEEDED_PERMISSIONS)
                .requestCode(REQUEST_PERMISSION_STORAGE)
                .rationale(mRationaleListener)
                .callback(mPermissionlistener)
                .start();
    }

    // ================================[Click Result]===========================>>
    private class clickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.btn_jpg:
                    //点击事件，而重定向到图片库
                    Intent picture_intent = new Intent(
                            Intent.ACTION_PICK,
                            android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                    //这里要传一个整形的常量RESULT_LOAD_IMAGE到startActivityForResult()方法。
                    startActivityForResult(picture_intent, RESULT_LOAD_IMAGE);
                    break;

                case R.id.btn_nv21:
                    //选择NV21格式文件
                    Intent file_intent = new Intent(
                            Intent.ACTION_OPEN_DOCUMENT,
                            android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                    startActivityForResult(file_intent, RESULT_LOAD_FILE);
                    break;
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_PERMISSION_STORAGE: {
                if (AndPermission.hasPermission(mContext,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                    //do nothing
                }
                break;
            }

            case RESULT_LOAD_IMAGE:{
                // 判断requestCode是否是之前传的RESULT_LOAD_IMAGE，并且返回的数据不能为空
                if (resultCode == RESULT_OK && null != data) {
                    Uri selectedImage = data.getData();
                    String picturePath = getRealPathFromURI(mContext, selectedImage);
                    String fileName = getFileNameNoSuffix(picturePath);
                    Log.d(TAG,"picturePath : " + picturePath + ", fileName = " + fileName);
                    mBitmap = BitmapFactory.decodeFile(picturePath);
                    mImage.setImageBitmap(mBitmap);

                    // 选择jpg后直接转换成nv21格式文件
                    int ret = generateNv21(fileName, mBitmap);
                    if(ret == 0){ //成功
                        TastyToast.makeText(
                                getApplicationContext(),
                                getString(R.string.convert_nv21_success),
                                TastyToast.LENGTH_SHORT,
                                TastyToast.SUCCESS);
                    }else{ //失败
                        TastyToast.makeText(
                                getApplicationContext(),
                                getString(R.string.convert_nv21_fail),
                                TastyToast.LENGTH_SHORT,
                                TastyToast.ERROR);
                    }
                }
                break;
            }

            case RESULT_LOAD_FILE:{
                // 判断requestCode是否是之前传的RESULT_LOAD_FILE，并且返回的数据不能为空
                if (resultCode == RESULT_OK && null != data) {
                    Uri fileUri = data.getData();
                    DocumentFile pickedFile = DocumentFile.fromSingleUri(this, fileUri);
                    String fileName = pickedFile.getName();
                    assert fileName != null;

                    String suffix = fileName.substring(fileName.lastIndexOf(".") + 1);
                    if(suffix.equalsIgnoreCase("nv21")){
                        String fileNameNoSuffix= getFileNameNoSuffix(fileName);
                        Log.d(TAG,"pickedFile name : " + fileNameNoSuffix);
                        Bitmap bm = (Bitmap) objectFromJNI(fileNameNoSuffix);
                        if(bm!=null){
                            TastyToast.makeText(
                                    getApplicationContext(),
                                    getString(R.string.convert_rgb_success),
                                    TastyToast.LENGTH_LONG,
                                    TastyToast.SUCCESS);
                            mImage.setImageBitmap(bm);
                        } else {
                            TastyToast.makeText(
                                    getApplicationContext(),
                                    getString(R.string.convert_rgb_fail),
                                    TastyToast.LENGTH_LONG,
                                    TastyToast.ERROR);
                        }

                    } else {
                        TastyToast.makeText(
                                getApplicationContext(),
                                getString(R.string.format_error),
                                TastyToast.LENGTH_LONG,
                                TastyToast.ERROR);
                        Log.e(TAG,"Format is not .nv21");
                    }
                }
                break;
            }
        }
    }

    // 从Uri获取文件路径
    public static String getRealPathFromURI(Context context, Uri contentURI) {
        String result;
        Cursor cursor = context.getContentResolver().query(contentURI,
                new String[]{MediaStore.Images.ImageColumns.DATA},
                null, null, null);
        if (cursor == null)
            result = contentURI.getPath();
        else {
            cursor.moveToFirst();
            int index = cursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA);
            result = cursor.getString(index);
            cursor.close();
        }
        return result;
    }

    // 提取文件名(无后缀)
    public String getFileNameNoSuffix(String pathAndName){

        int start=pathAndName.lastIndexOf("/");
        int end=pathAndName.lastIndexOf(".");
        if(start!=-1 && end!=-1){ //从完整路径中提取
            return pathAndName.substring(start+1,end);
        }else if(start==-1 && end!=-1){ //从文件名中提取
            return pathAndName.substring(0, end);
        } else{
            return null;
        }

    }
    // ================================[Click Result] END===========================<<

    // ================================[PERMISSION FUNCTION]===========================>>
    private PermissionListener mPermissionlistener = new PermissionListener() {
        @Override
        public void onSucceed(int requestCode, List<String> grantedPermissions) {
            if (requestCode == REQUEST_PERMISSION_STORAGE) {
                List<String> requiredPermissions = new ArrayList<String>();
                for (int i = 0; i < NEEDED_PERMISSIONS.length; i++) {
                    requiredPermissions.add(NEEDED_PERMISSIONS[i]);
                }
                boolean grantedAll = grantedPermissions.containsAll(requiredPermissions);
                if (grantedAll) {
                    //  call to a native method
                    try {
                        am = mContext.getResources().getAssets();
                        is = am.open("welcome.jpg");
                        if (null == is) {
                            Log.e(TAG, "assets open failed");
                        }
                        mBitmap = BitmapFactory.decodeStream(is);
                        mImage.setImageBitmap(mBitmap);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        @Override
        public void onFailed(int requestCode, List<String> deniedPermissions) {
            if (requestCode == REQUEST_PERMISSION_STORAGE) {
                TastyToast.makeText(
                        getApplicationContext(),
                        getString(R.string.permission_deny),
                        TastyToast.LENGTH_LONG,
                        TastyToast.CONFUSING);

                if (AndPermission.hasAlwaysDeniedPermission(mContext, deniedPermissions)) {
                    // First type: with AndPermission default prompt.
                    AndPermission.defaultSettingDialog(mContext,
                            REQUEST_PERMISSION_STORAGE).show();
                }
            }
        }
    };

    private RationaleListener mRationaleListener = new RationaleListener() {
        @Override
        public void showRequestPermissionRationale(int requestCode, Rationale rationale) {
            if (requestCode == REQUEST_PERMISSION_STORAGE) {
                AndPermission.rationaleDialog(mContext, rationale).show();
            }
        }
    };
    // ================================[PERMISSION FUNCTION] END===========================<<

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int generateNv21(String name, Bitmap bitmap);
    public native Object objectFromJNI(String name);
}
