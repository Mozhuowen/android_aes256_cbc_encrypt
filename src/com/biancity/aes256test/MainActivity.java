package com.biancity.aes256test;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		TextView textv1 = (TextView) findViewById(R.id.text1);
		TextView textv2 = (TextView) findViewById(R.id.text2);
		TextView textv3 = (TextView) findViewById(R.id.text3);
		
		String sourceStr = "Hello World!";
		textv1.setText(sourceStr);
		String enStr = new String(EncryptUtil.getImportantInfoByJNI(sourceStr, 0));
		textv2.setText("after encrypt: "+ enStr);
		textv3.setText("after decrypt: "+new String(EncryptUtil.getImportantInfoByJNI(enStr, 1)));
	}

}
