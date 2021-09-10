#define _CRT_SECURE_NO_WARNINGS//VS �꣬����ʹ�ò���ȫ���������
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "aesfunction.c"
typedef uint8_t byte;
typedef uint32_t word;

const char* AES_MODE[] = { "ECB","CBC","CFB","OFB" };
char* plainfile = NULL;
char* keyfile = NULL;
char* vifile = NULL;
char* mode = NULL;
char* cipherfile = NULL;

byte* plaintext = NULL;
byte* keytext = NULL;
byte* vitext = NULL;
byte* ciphertext = NULL;

uint64_t plaintextlength = 0;
uint64_t vitextlegnth = 0;
uint64_t keytextlength = 0;
uint64_t ciphertextlength = 0;
//��S��
const uint8_t rsbox[256] = {
  		//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    /*0*/ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    /*1*/ 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    /*2*/ 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    /*3*/ 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    /*4*/ 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    /*5*/ 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    /*6*/ 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    /*7*/ 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    /*8*/ 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    /*9*/ 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    /*A*/ 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    /*B*/ 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    /*C*/ 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    /*D*/ 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    /*E*/ 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    /*F*/ 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
void print_usage() {
    /*
        �������������ʾ�����˳���
    */
    printf("\n�Ƿ�����,֧�ֵĲ��������£�\n-p plainfile ָ�������ļ���λ�ú�����\n-k keyfile  ָ����Կ�ļ���λ�ú�����\n-v vifile  ָ����ʼ�������ļ���λ�ú�����\n-m mode  ָ�����ܵĲ���ģʽ(ECB,CBC,CFB,OFB)\n-c cipherfile ָ�������ļ���λ�ú����ơ�\n");
    exit(-1);
}
bool readfile2memory(const char* filename, byte** memory, uint64_t* memorylength) {
	/*
	��ȡ�ļ����ڴ棬ͬʱ���ַ���4e�� ת��һ���ֽ�0x4e
	*/
	FILE* fp = NULL;
	int i;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (size % 2 != 0) {
		printf("%s:�ļ��ֽ�����Ϊż����\n", filename);
		fclose(fp);
		return false;
	}
	byte* tmp = malloc(size);
	memset(tmp, 0, size);

	fread(tmp, size, 1, fp);
	if (ferror(fp)) {
		printf("��ȡ%s�����ˣ�\n", filename);
		fclose(fp);
		return false;
	}
	else {
		fclose(fp);
	}

	*memory = malloc(size / 2);
	memset(*memory, 0, size / 2);
	*memorylength = size / 2;

	byte parsewalker[3] = { 0 };
	printf("readfile2memory debug info:");
	for (i = 0; i < size; i += 2) {
		parsewalker[0] = tmp[i];
		parsewalker[1] = tmp[i + 1];
		(*memory)[i / 2] = strtol(parsewalker, 0, 16);
		printf("%c", (*memory)[i / 2]);
	}
	printf("\n");

	free(tmp);

	return true;
}
void print_help(char* bufname, byte* buf, uint8_t bytes) {
	/*
	��ӡ������Ϣ
	*/
	int i;
	printf("%s��Ϣ:\n", bufname);
	/*for (int i = 0; i < bytes; i++) {
		printf("%c", buf[i]);
	}*/
	//printf("\n");
	for (i = 0; i < bytes; i++) {
		printf("%02x ", buf[i]);
	}
	printf("\n\n");
	/*for (int i = 0; i < bytes; i++) {
		for (int j = 7; j >= 0; j--) {
			if (buf[i] & 1 << j) {
				printf("1");
			}
			else {
				printf("0");
			}
		}
		printf("\n");
	}
	printf("\n\n");*/
}
void AES(const byte* inputtext, const byte* inputkey,const uint8_t keylength, const char mode, byte* output) {
	/*
	AES �ӽ��ܺ�����
	@inputtext: ������ַ�����ָ�룬��СΪ128λ��16�ֽ�
	@inputkey: �������Կ�ַ�����ָ�룬��С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@mode: ģʽ��'e'Ϊ����ģʽ��'d'Ϊ����ģʽ
	@output: ������ַ�����ָ�룬��СΪ128λ��16�ֽ�
	*/
	int i,j,k = 0;
	int u,v,w=0;
	int count = 0;
	uint8_t pp[17];
	uint8_t kk[17];
	uint8_t out1[17];
	memcpy(pp,inputtext,16);
	memcpy(kk,inputkey,16);
//	StringToHex(inputtext,pp);//�ַ���ת��Ϊʮ��������ֵ 
	pp[16]='\0';
//	printf("%x\n",pp[1]);
//	StringToHex(inputkey,kk);
	kk[16]='\0';
//	printf("%x\n",kk[0]);
	KeyExpansion(kk);//��Կ��չ
//	for(i=0;i<44;i++){
//		for(j=0;j<4;++j){
//			printf("%x",expk[j][i]);
//		}
//		puts("");
//	}
	//Ԥ����������� 
//	printf("%x\n",kk[0]);
//	pp[12] = 0x20;
/*	for(i=0;i<16;++i){
		printf("%x ",pp[i]);
	}
	puts("");
	for(i=0;i<16;++i){
		printf("%x ",kk[i]);
	}
	puts("");
*/
	AddRoundKey(pp,kk,out1);
	memcpy(pp,out1,16);
	//ǰ9��
	for(i=0;i<9;++i){
		SubBytes(pp,out1);
		memcpy(pp,out1,16);
		ShiftRows(pp,out1);
		memcpy(pp,out1,16);
		MixColumns(pp,out1);
		memcpy(pp,out1,16);
		count = 0;
		for(j=0;j<4;++j){
			for(k=0;k<4;++k){
				kk[count] = expk[k][j+i*4+4];
				++count;
			}
		}
		kk[16]='\0';
		AddRoundKey(pp,kk,out1);

		memcpy(pp,out1,16);
	}

	//���һ�ּ���
	SubBytes(pp,out1);
	memcpy(pp,out1,16);
	ShiftRows(pp,out1);
	memcpy(pp,out1,16);
	count=0;
	for(j=0;j<4;++j){
		for(k=0;k<4;++k){
			kk[count] = expk[k][40+j];
			++count;
		}
	}
	AddRoundKey(pp,kk,out1);
	memcpy(pp,out1,16); 
	for(i=0;i<16;++i){
		output[i]=pp[i];
	}
//	for(i=0;i<16;++i){
//		printf("%x",pp[i]);
//	}
	assert(inputtext != NULL && inputkey != NULL && (keylength==128 || keylength == 192 || keylength == 256) && ( mode=='e' || mode=='d' ) && output != NULL);
}
void ECBe(const byte* plaintext, const uint64_t plainlength, const byte* keytext, const uint8_t keylength, byte** ciphertext, uint64_t cipherlength) {
	/*
	AES ECBģʽ���ܺ�����
	@plaintext: ����������ַ�����ָ�룬
	@plainlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬��С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@ciphertext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@cipherlength: ��������ַ����еĳ���
	*/

	int i,j,k = 0;
	int u,v,w=0;
	int count = 0; 
//	printf("%d\n%d\n%d\n",plainlength,keylength,cipherlength);
	uint8_t pp[plainlength+1];
	uint8_t kk[keylength+1];
	uint8_t out1[cipherlength+1];

	uint8_t tp[17];
	uint8_t tout1[17];
	* ciphertext = malloc(sizeof(byte)*cipherlength);
	MStringToHex(plaintext, pp,plainlength);//�ַ���ת��Ϊʮ��������ֵ 
	printf("������\n");
	pp[plainlength]='\0';
	StringToHex(keytext,kk);
	kk[keylength]='\0';
	count = plainlength/16;

	for(i=0;i<count;++i){
		printf("��%d�μ���\n",i);
		for(j=0;j<16;++j){
			tp[j] = pp[i*16+j]; 
		}
		AES(tp,kk,keylength*8,'d',tout1);

		for(j=0;j<16;++j){
			out1[i*16+j] = tout1[j];
		}
	}
//	for(i=0;i<16*count;++i){
//		printf("%x ",out1[i]);
//	}
//	puts("");
	* ciphertext = out1;
//	assert(plaintext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void CBCe(const byte* plaintext, const uint64_t plainlength, const byte* keytext, const uint8_t keylength, const byte* vitext, byte** ciphertext, uint64_t cipherlength) {
	/*
	AES CBCģʽ���ܺ�����
	@plaintext: ����������ַ�����ָ�룬
	@plainlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬��С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@ciphertext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@cipherlength: ��������ַ����еĳ���
	*/
	int i,j,k = 0;
	int u,v,w=0;
	int count = 0;
//	printf("%d\n%d\n%d\n",plainlength,keylength,cipherlength);
	uint8_t pp[plainlength+1];
	uint8_t kk[keylength+1];
	uint8_t vv[17];
	uint8_t out1[cipherlength+1];
	
	uint8_t tp[17];
	uint8_t tout1[33];
	uint8_t tout2[17];
	
	char bv[33];
	byte tbv[129];
	
	* ciphertext = malloc(sizeof(byte)*cipherlength);
	StringToHex(plaintext,pp);//�ַ���ת��Ϊʮ��������ֵ 
	pp[plainlength]='\0';
	StringToHex(keytext,kk);
	kk[keylength]='\0';
	StringToHex(vitext,vv);
	vv[16] = '\0';
//	printf("%d",strlen(vv));
	count = plainlength/16;
	for(i=0;i<count;++i){
		for(j=0;j<16;++j){
			tp[j] = pp[i*16+j]; 
		}
		tp[16]='\0';
		//��һ�����ķ���ͳ�ʼ�������
		strHexxor(tp,vv,tout2);//�����Ϊ128bit 
		//���� 
		AES(tout2,kk,keylength*8,'d',tout1);
		for(j=0;j<16;++j){
			out1[i*16+j] = tout1[j];
			vv[j] = tout1[j];//��һ��������Ϊ������� 
		}
		vv[16]='\0';
	}
//	for(i=0;i<16*count;++i){
//		printf("%x ",out1[i]);
//	}
	puts("");
	* ciphertext = out1;
//	assert(plaintext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void CFBe(const byte* plaintext, const uint64_t plainlength, const byte* keytext, const uint8_t keylength, const byte* vitext, byte** ciphertext, uint64_t cipherlength) {
	/*
	AES CFBģʽ���ܺ�����
	@plaintext: ����������ַ�����ָ�룬
	@plainlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬��С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@ciphertext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@cipherlength: ��������ַ����еĳ���
	*/
	int s = 8;//8λģʽ 
	int Flag = (plainlength*8)/s;//������� 
	byte DM[Flag+1];//������ 
	byte M;
	byte DC[Flag+1];//������ 
	byte C;
	byte V;
	int count;
	int i,j,k;
	int u,v,w;
	//�����ķ��� 
	StringToHex(plaintext,DM);
	DM[plainlength]='\0';
	//��ʼ��������Ĵ��� 
	byte rig[17];
	StringToHex(vitext,rig);
	rig[16]='\0';
	byte kk[keylength+1];
	StringToHex(keytext,kk);
	kk[keylength]='\0';
	byte tout1[17];
	
	char* V1 = NULL;
	char TV[9]={'\0'};
	char TM[9]={'\0'};
	char TC[9]={'\0'};
	char *TCptr = NULL;
	byte out1;
	//��ʼ������� 
	for(i=0;i<Flag;++i){
		AES(rig,kk,keylength*8,'d',tout1);
		//ѡ��sλ������� 
		V = tout1[0];
		V1 = hex2b(V);
		memcpy(TV,V1,8);
		
		V = DM[i];
		V1 = hex2b(V);
		memcpy(TM,V1,8);
		
		TCptr = bxor(TV,TM);
		memcpy(TC,TCptr,8);
		TC[8] = '\0';
		
		u = 0;
		v = 0;
		w = 0;
		for(u=0;u<8;++u){
			if(u<4){
				if(TC[u]=='1'){
					v += pow(2,3-u);
				}
			}
			else{
				if(TC[u]=='1'){
					w += pow(2,7-u);
				}
			}
		}
		out1 = HEX[v]*0x10 + HEX[w];
		//�õ���һ������ out1
		DC[i] = out1;
		//��λ
		for(u=0;u<15;++u){
			rig[u]=rig[u+1];
		}
		rig[15]=out1;
		rig[16]='\0'; 
	}
	DC[Flag] = '\0';
	* ciphertext = DC;
//	assert(plaintext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void OFBe(const byte* plaintext, const uint64_t plainlength, const byte* keytext, const uint8_t keylength, const byte* vitext, byte** ciphertext, uint64_t cipherlength) {
	/*
	AES OFBģʽ���ܺ�����
	@plaintext: ����������ַ�����ָ�룬
	@plainlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬��С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@ciphertext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@cipherlength: ��������ַ����еĳ���
	*/
	int s = 8;//8λģʽ 
	int Flag = (plainlength*8)/s;//������� 
	byte DM[Flag+1];//������ 
	byte M;
	byte DC[Flag+1];//������ 
	byte C;
	byte V;
	int count;
	int i,j,k;
	int u,v,w;
	//�����ķ��� 
	StringToHex(plaintext,DM);
	DM[plainlength]='\0';
	//��ʼ��������Ĵ��� 
	byte rig[17];
	StringToHex(vitext,rig);
	rig[16]='\0';
	byte kk[keylength+1];
	StringToHex(keytext,kk);
	kk[keylength]='\0';
	byte tout1[17];
	
	char* V1 = NULL;
	char TV[9]={'\0'};
	char TM[9]={'\0'};
	char TC[9]={'\0'};
	char *TCptr = NULL;
	byte out1;
	//��ʼ������� 
	for(i=0;i<Flag;++i){
		AES(rig,kk,keylength*8,'d',tout1);
		//ѡ��sλ������� 
		V = tout1[0];
		V1 = hex2b(V);
		memcpy(TV,V1,8);
		
		V = DM[i];
		V1 = hex2b(V);
		memcpy(TM,V1,8);
		
		TCptr = bxor(TV,TM);
		memcpy(TC,TCptr,8);
		TC[8] = '\0';
		
		u = 0;
		v = 0;
		w = 0;
		for(u=0;u<8;++u){
			if(u<4){
				if(TC[u]=='1'){
					v += pow(2,3-u);
				}
			}
			else{
				if(TC[u]=='1'){
					w += pow(2,7-u);
				}
			}
		}
		DC[i] = HEX[v]*0x10 + HEX[w];
		//�õ�һ������
		//��λ
		for(u=0;u<15;++u){
			rig[u]=rig[u+1];
		}
		rig[15]= tout1[0];
		rig[16]='\0'; 
	}
	DC[Flag] = '\0';
	* ciphertext = DC;
	//assert(plaintext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void ECBd(const byte* ciphertext, const uint64_t cipherlength, const byte* keytext, const uint8_t keylength, byte** plaintext, uint64_t* plainlength) {
	/*
	DES ECBģʽ���ܺ�����
	@ciphertext: ����������ַ�����ָ�룬
	@cipherlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬����С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@plaintext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@plainlength:��������ַ����еĳ���
	*/
	assert(ciphertext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void CBCd(const byte* ciphertext, const uint64_t cipherlength, const byte* keytext, const byte* vitext, const uint8_t keylength, byte** plaintext, uint64_t* plainlength) {
	/*
	DES CBCģʽ���ܺ�����
	@ciphertext: ����������ַ�����ָ�룬
	@cipherlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬����С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@vitext: ����ĳ�ʼ�������ַ�����ָ��
	@plaintext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@plainlength:��������ַ����еĳ���
	*/
	assert(ciphertext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void CFBd(const byte* ciphertext, const uint64_t cipherlength, const byte* keytext, const byte* vitext, const uint8_t keylength, byte** plaintext, uint64_t* plainlength) {
	/*
	DES CFBģʽ���ܺ�����
	@ciphertext: ����������ַ�����ָ�룬
	@cipherlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬����С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@vitext: ����ĳ�ʼ�������ַ�����ָ��
	@plaintext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@plainlength:��������ַ����еĳ���
	*/
	assert(ciphertext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
void OFBd(const byte* ciphertext, const uint64_t cipherlength, const byte* keytext, const byte* vitext, const uint8_t keylength, byte** plaintext, uint64_t* plainlength) {
	/*
	DES OFBģʽ���ܺ�����
	@ciphertext: ����������ַ�����ָ�룬
	@cipherlength: ����������ַ����г��ȣ��ֽڣ�
	@keytext: �������Կ�ַ�����ָ�룬����С��keylengthָ��
	@keylength: �������Կ���ȣ�ֻ��Ϊ128��192��256����ֵ(λ)
	@vitext: ����ĳ�ʼ�������ַ�����ָ��
	@plaintext: ��������ַ����ж���ָ�룬�����ռ䣬����ʧ�ܵģ��������ؼ��֣�C ����ָ�� ���� ����ռ�
	@plainlength:��������ַ����еĳ���
	*/
	assert(ciphertext != NULL && keytext != NULL && (keylength == 128 || keylength == 192 || keylength == 256));
}
int main(int argc, char** argv) {
    int i;	
	uint64_t cipherlength;
	printf("argc:%d\n", argc);
	for (i = 0; i < argc; i++) {
		printf("%d : %s\n", i, argv[i]);
	}

	/*
	-p plainfile ָ�������ļ���λ�ú�����
	-k keyfile  ָ����Կ�ļ���λ�ú�����
	-v vifile  ָ����ʼ�������ļ���λ�ú�����
	-m mode  ָ�����ܵĲ���ģʽ
	-c cipherfile ָ�������ļ���λ�ú����ơ�
	*/

	if (argc % 2 == 0) {
		print_usage();
	}

	for (i = 1; i < argc; i += 2) {
		if (strlen(argv[i]) != 2) {
			print_usage();
		}
		switch (argv[i][1]) {
		case 'p':
			plainfile = argv[i + 1];
			break;
		case 'k':
			keyfile = argv[i + 1];
			break;
		case 'v':
			vifile = argv[i + 1];
			break;
		case 'm':
			if (strcmp(argv[i + 1], AES_MODE[0]) != 0 && strcmp(argv[i + 1], AES_MODE[1]) != 0 && strcmp(argv[i + 1], AES_MODE[2]) != 0 && strcmp(argv[i + 1], AES_MODE[3]) != 0) {
				print_usage();
			}
			mode = argv[i + 1];
			break;
		case 'c':
			cipherfile = argv[i + 1];
			break;
		default:
			print_usage();
		}
	}

	if (plainfile == NULL || keyfile == NULL || mode == NULL || cipherfile == NULL) {
		print_usage();
	}

	if (strcmp(mode, "ECB") != 0 && vifile == NULL) {
		print_usage();
	}

	printf("����������ɣ�\n");
	printf("����Ϊ�����ļ���λ�ú�����:%s\n", plainfile);
	printf("����Ϊ��Կ�ļ���λ�ú�����:%s\n", keyfile);
	if (strcmp(mode, "ECB") != 0) {
		printf("����Ϊ��ʼ�������ļ��ļ���λ�ú�����:%s\n", vifile);
	}
	printf("����Ϊ�����ļ���λ�ú�����:%s\n", cipherfile);
	printf("����Ϊ���ܵ�ģʽ:%s\n", mode);

	printf("���ڿ�ʼ��ȡ�ļ���\n");

	printf("��ȡ�����ļ�...\n");
	bool read_result = readfile2memory(plainfile, &plaintext, &plaintextlength);
	if (read_result == false) {
		printf("��ȡ�����ļ�ʧ�ܣ�����·�����ļ��Ƿ����\n");
		exit(-1);
	}
	printf("��ȡ�����ļ��ɹ���\n");

	printf("��ȡ��Կ�ļ�...\n");
	read_result = readfile2memory(keyfile, &keytext, &keytextlength);
	if (read_result == false) {
		printf("��ȡ��Կ�ļ�ʧ�ܣ�����·�����ļ��Ƿ����\n");
		exit(-1);
	}
	printf("��ȡ��Կ�ļ��ɹ���\n");

	if (strcmp(mode, "ECB") != 0) {
		printf("��ȡ��ʼ�����ļ�...\n");
		read_result = readfile2memory(vifile, &vitext, &vitextlegnth);
		if (read_result == false) {
			printf("��ȡ��ʼ�����ļ�ʧ�ܣ�����·�����ļ��Ƿ����\n");
			exit(-1);
		}
		printf("��ȡ��ʼ�����ļ��ɹ���\n");
	}

	if (strcmp(mode, "ECB") == 0) {
		ciphertext = (char*)malloc(sizeof(char)*plaintextlength);
		cipherlength = plaintextlength;
		ECBe(plaintext, plaintextlength, keytext, keytextlength, &ciphertext, cipherlength);
	}
	else if (strcmp(mode, "CBC") == 0) {
		ciphertext = (char*)malloc(sizeof(char)*plaintextlength);
		cipherlength = plaintextlength;
		CBCe(plaintext , plaintextlength , keytext, keytextlength , vitext , &ciphertext, cipherlength);
	}
	else if (strcmp(mode, "CFB") == 0) {
		ciphertext = (char*)malloc(sizeof(char)*plaintextlength);
		cipherlength = plaintextlength;
		CFBe(plaintext , plaintextlength , keytext, keytextlength , vitext , &ciphertext, cipherlength);

	}
	else if (strcmp(mode, "OFB") == 0) {
		ciphertext = (char*)malloc(sizeof(char)*plaintextlength);
		cipherlength = plaintextlength;
		OFBe(plaintext , plaintextlength , keytext, keytextlength , vitext , &ciphertext, cipherlength);
	}
	else {
		//��Ӧ���ܵ�������
		printf("�������󣡣���\n");
		exit(-2);
	}


	if (ciphertext == NULL) {
		printf("ͬѧ��ciphertextû�з����ڴ�Ŷ����Ҫ��������~\nʧ�ܣ������˳���...");
		exit(-1);
	}
	byte outputf[plaintextlength];
	memcpy(outputf,ciphertext,plaintextlength);
//	printf("���ܳ������ַ���Ϊ:%s\n", ciphertext);
	printf("16���Ʊ�ʾΪ:");
	int count = cipherlength;
	byte cipherhex[count*2];
	memset(cipherhex, 0, count*2);
	for(i=0;i<cipherlength;++i){
		printf("%x ",outputf[i]);
	}
	puts(""); 
	for (i = 0; i < count; i++) {
		sprintf(cipherhex+i*2 , "%2x", outputf[i]);
	}
	for(i=0;i<2*count;++i){
		if(cipherhex[i]==' '){
			cipherhex[i]='0';
		}
	}
	printf("%s\nд���ļ���...\n", cipherhex);

	FILE* fp = fopen(cipherfile, "w");
	if (fp == NULL) {
		printf("�ļ� %s ��ʧ��,����", cipherfile);
		exit(-1);
	}

	int writecount = fwrite(cipherhex, count * 2, 1, fp);
	if (writecount != 1) {
		printf("д���ļ����ֹ��ϣ������³��ԣ�");
		fclose(fp);
		exit(-1);
	}
	fclose(fp);
	//benchmark();
	printf("��ϲ������˸ó������ύ����!");

    return 0;
}
