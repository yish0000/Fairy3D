/**
 * ------------------------------------------------------------------------
 *  Name:   FairyActivity.java
 *  Desc:   Activity for android application.
 *  Author: Yish
 *  Date:   2015/1/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft 2015. All right Observed.
 * ------------------------------------------------------------------------
 */
package com.yishpear.fairy3d;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;

/**
 * @author Yish
 * 
 */
public class FairyActivity extends Activity {
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
	}
	
	@Override
	public void onDestroy() {
		
	}
	
	@Override
	public void onPause() {
		
	}
	
	@Override
	public void onResume() {
		
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		return true;
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		
		return true;
	}
	
	// Native functions for engine.
	public static native void nativeInit();
	public static native void nativeFrame();
	
	public static native void nativeOnTouchesBegin(final int id, final float x, final float y);
	public static native void nativeOnTouchesMove(final int[] ids, final float[] xs, final float[] ys);
	public static native void nativeOnTouchesEnd(final int id, final float x, final float y);
	public static native void nativeOnTouchesCancel(final int[] ids, final float[] xs, final float[] ys);
	
	public static native void nativeOnKeyDown(int keycode);
	public static native void nativeOnKeyUp(int keycode);
	public static native void nativeOnResume();
	public static native void nativeOnPause();
}
