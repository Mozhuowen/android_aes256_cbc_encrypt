/**
 * `EncryptUtil.java'
 *
 * copyright (c) 2015 https://github.com/Mozhuowen
 */
package com.biancity.aes256test;

public class EncryptUtil
{
	static {
		System.loadLibrary("EncryptMsg");
	}

	public static native byte[] getImportantInfoByJNI(String str,int jint);
}