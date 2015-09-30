/**
 * `EncryptMsg.c'
 *
 * copyright (c) 2015 https://github.com/Mozhuowen
 */
#include "aes256.h"
#include "base64.h"
#include "b64.h"

#ifndef LOGE
#define LOGE(...){\
	if (1) {\
		__android_log_print(ANDROID_LOG_ERROR,"jni---get---key",__VA_ARGS__);\
	}\
	}
#endif

jstring getImportInfo(JNIEnv *, jstring,jint);
jstring getDecrypt(JNIEnv *,jstring);
jstring charToJstring(JNIEnv* envPtr, char *src);
jbyteArray charToJbyteArray(JNIEnv* env,char *src,int bufflen);

JNIEXPORT
jbyteArray JNICALL Java_com_biancity_aes256test_EncryptUtil_getImportantInfoByJNI(
		JNIEnv *env, jobject thiz, jstring mingwen,jint ji) {

	return getImportInfo(env, mingwen,ji);
}

jbyteArray getImportInfo(JNIEnv* envPtr, jstring mingwen,jint ji) {
//	LOGE("enter getImportInfo method ok!");
	if (ji == 1)
		return getDecrypt(envPtr, mingwen);

	JNIEnv env = *envPtr;
	const int BLOCK_SIZE = 32;

	//encrypt key
	unsigned char key[32] = {0x6D,0xA7,0x79,0x1D,0xF8,0x21,0x8A,0x39,0x25,0x9A,
			0x7A,0x29,0xAA,0xBB,0x2D,0xBA,0xCF,0xFC,0x31,0x2D,0x74,0xDB,0x7E,0x39,
			0xEB,0xBF,0x7C,0x10,0x04,0x00,0x83,0x61};

//	LOGE(key);

	//****************************************开始加密******************************************************
	//1.初始化数据
	//初始化向量
	uint8_t iv[16] = {0x6D,0xA7,0x1D,0x79,0xF8,0x21,0x8A,0x39,0x25,0x9A,0x7A,0x29,0xAA,0xBB,0x2D,0xBA};

	//初始化加密参数
	aes256_context ctx;
	aes256_init(&ctx, key);

	//2.将jstring转为char
	const char *mwChar = env->GetStringUTFChars(envPtr, mingwen, JNI_FALSE);

	//3.分组填充加密
	int i;
	int mwSize = strlen(mwChar);
	int remainder = mwSize % BLOCK_SIZE;
	jbyteArray entryptString;
	if (mwSize < BLOCK_SIZE) {	//小于32字节，填充32字节，后面填充几个几
		uint8_t input[BLOCK_SIZE];
		for (i = 0; i < BLOCK_SIZE; i++) {
			if (i < mwSize) {
				input[i] = mwChar[i];
			} else {
				input[i] = BLOCK_SIZE - mwSize;
			}
		}
		//加密
		uint8_t output[BLOCK_SIZE];
		aes256_encrypt_cbc(&ctx, input, iv, output);
		//base64加密后然后jstring格式输出
		char *enc = base64_encode(output, sizeof(output));
//		entryptString = charToJstring(envPtr, enc);
		entryptString = charToJbyteArray(envPtr,enc,strlen(enc));

		free(enc);
	} else {	//如果是32的倍数，填充32字节，后面填充0x20
		int group = mwSize / BLOCK_SIZE;
		int size = BLOCK_SIZE * (group + 1);
		uint8_t input[size];
		for (i = 0; i < size; i++) {
			if (i < mwSize) {
				input[i] = mwChar[i];
			} else {
				if (remainder == 0) {
					input[i] = 0x20;
				} else {	//如果不足32位 少多少位就补几个几  如：少4为就补4个4 以此类推
					int dif = BLOCK_SIZE - mwSize;
					input[i] = dif;
				}
			}
		}
		//加密
		uint8_t output[size];
		aes256_encrypt_cbc(&ctx, input, iv, output);
		//base64加密后然后jstring格式输出
		char *enc = base64_encode(output, sizeof(output));
//		entryptString = charToJstring(envPtr, enc);
		entryptString = charToJbyteArray(envPtr,enc,strlen(enc));

		free(enc);
	}

	//释放mwChar
	env->ReleaseStringUTFChars(envPtr, mingwen, mwChar);

	return entryptString;
}

jbyteArray getDecrypt(JNIEnv *envPtr,jstring encryptstr)
{
//	LOGE("enter getDecrypt ok!");
	JNIEnv env = *envPtr;

	int i;
	const int BLOCK_SIZE = 32;
	unsigned char key[32] = {0x6D,0xA7,0x79,0x1D,0xF8,0x21,0x8A,0x39,0x25,0x9A,
			0x7A,0x29,0xAA,0xBB,0x2D,0xBA,0xCF,0xFC,0x31,0x2D,0x74,0xDB,0x7E,0x39,
			0xEB,0xBF,0x7C,0x10,0x04,0x00,0x83,0x61};

	//	LOGE(key);

	//****************************************开始加密******************************************************
	//1.初始化数据
	//初始化向量
	uint8_t iv[16] = {0x6D,0xA7,0x1D,0x79,0xF8,0x21,0x8A,0x39,0x25,0x9A,0x7A,0x29,0xAA,0xBB,0x2D,0xBA};

	//初始化加密参数
	aes256_context ctx;
	aes256_init(&ctx, key);

	//2.将jstring转为char
	const char *encryptChar = env->GetStringUTFChars(envPtr, encryptstr, JNI_FALSE);
//	LOGE("encryptChar lenght: %d",strlen(encryptChar));
	basedata decodebase64 = base64_decode(encryptChar,strlen(encryptChar));
//	LOGE("decodebase64info lenght: %d",decodebase64.len);
	int targetstrlen = decodebase64.len;
//	LOGE("decodebase64 ok!");
	int len = decodebase64.len;
	unsigned char resbuf[len];
	int ii;
	for (ii=0;ii<len;ii++) {
		resbuf[ii] = *(decodebase64.data + ii);
	}
//	LOGE("resbuf lenght: %d",sizeof(resbuf));
//	LOGE("resbuf first and last num is %X %X",resbuf[0],resbuf[len-1]);
	//3.解密
	uint8_t output[len];
	aes256_decrypt_cbc(&ctx, resbuf, iv, output,len);
//	LOGE("aes256_decrypt_cbc ok!");
//	LOGE("output lenght: %d",sizeof(output));
//	LOGE("output last num is %X",output[len-1]);
//	return encryptstr;
	//4.去除补位padding
	uint8_t pad = output[len-1];
	if (pad <1 || pad > 0x20)
		pad = 0;
	int reslen = len - pad;
	uint8_t resultChar[reslen];
	for (i=0;i<reslen;i++) {
		resultChar[i] = output[i];
	}
//	LOGE("ripe padding ok!");
//	LOGE("reslutChar lenght: %d",sizeof(resultChar));
//
//	//释放mwChar
	env->ReleaseStringUTFChars(envPtr, encryptstr,encryptChar);
	free(decodebase64.data);

	return charToJbyteArray(envPtr,resultChar,reslen);
}

jstring charToJstring(JNIEnv* envPtr, char src[]) {
	JNIEnv env = *envPtr;

	jsize len = strlen(src);
	jclass clsstring = env->FindClass(envPtr, "java/lang/String");
	jstring strencode = env->NewStringUTF(envPtr, "UTF-8");
	jmethodID mid = env->GetMethodID(envPtr, clsstring, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray barr = env->NewByteArray(envPtr, len);
	env->SetByteArrayRegion(envPtr, barr, 0, len, (jbyte*) src);

	return (jstring) env->NewObject(envPtr, clsstring, mid, barr, strencode);
}
jbyteArray charToJbyteArray(JNIEnv* env,char *src,int buffer_size) {
	int i;
	unsigned char buff[buffer_size];
	for (i=0;i<buffer_size;i++)
		buff[i] = src[i];
	jbyteArray array = (*env)->NewByteArray(env,buffer_size);
	(*env)->SetByteArrayRegion(env,array,0,buffer_size,buff);
	return array;
}
