#ifndef _BASE64_H_
#define _BASE64_H_

typedef struct {
	unsigned char *data;
	int len;
} basedata;

static const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
char* base64_encode(const char*, int);
basedata base64_decode(const char*, int);
static char find_pos(char);

#endif
