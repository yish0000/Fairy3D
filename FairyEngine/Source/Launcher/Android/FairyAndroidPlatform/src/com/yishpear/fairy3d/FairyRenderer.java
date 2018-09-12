/**
 * ------------------------------------------------------------------------
 *  Name:   FairyRenderer.java
 *  Desc:   Activity for android application.
 *  Author: Yish
 *  Date:   2015/1/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft 2015. All right Observed.
 * ------------------------------------------------------------------------
 */
package com.yishpear.fairy3d;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;

public class FairyRenderer implements GLSurfaceView.Renderer {

	private static final boolean DEBUG = false;

	private static Context mContext = null;
	
	public FairyRenderer(Context context, FairyView view) {
		mContext = context;
		
	}
	
	public void onDrawFrame(GL10 gl) {
		
		FairyActivity.nativeFrame();		
	}
	
	public void  onSurfaceChanged(GL10 gl, int width, int height) {
		gl.glViewport(0, 0, width, height);
	}
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		
	}
}
