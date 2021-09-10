#define _CRT_SECURE_NO_WARNINGS
//#include "staticdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*�������ַ�ת����*/
int BcharTransint(char a){
	if(a == '1'){
		return 1;
	}
	else{
		return 0;
	}
}

char * f_function(const char * R , const char* keytext){//�˴���R��keytext�Ƕ�������ʽ���ַ������� 
	int count = 0;
	char ER[49]={'0'};//��չ���48λ����
	char KER[49]={'0'};//������Կ��������� 
//	char test[49]={'0','1','1','1','0','0','0','0','1','0','1','0','1','0','0','1','1','0','0','1','0','0','0','0','1','1','1','1','0','1','0','1','1','1','1','1','1','1','0','0','0','0','1','1','0','1','1','0'};
	int S[9] = {0};//�˸�S�е���� 
	char CS[33] = {'0'};//S�������32λ���� 
	char PCS[33] = {'0'};//����p�û����CS��Ҳ���Ǿ����ֺ�������������� 
	int i,k = 0;
	int c,r = 0;//�к��� 
	int j = 0;
	int temp = 0;
    /*��չ����*/
    count = 0;
	for(i=0;i<8;++i){
		for(j=0;j<6;++j){
			temp = EP[i][j];
			ER[count] = R[temp-1];
			++count;
		}
	}
	ER[48] = '\0';
	/*��չ����������Կ�������*/
	for(i=0;i<48;++i){
		if(ER[i] == keytext[i]){
			KER[i] = '0';
		}
		else{
			KER[i] = '1';
		}
	}
	KER[48] = '\0';
	/*S������*/
	count = 0;
	for(i=0;i<43;i+=6){
		c = BcharTransint(KER[i])*2 + BcharTransint(KER[i+5]);
		r = BcharTransint(KER[i+1])*2*2*2 + BcharTransint(KER[i+2])*2*2 + BcharTransint(KER[i+3])*2 + BcharTransint(KER[i+4]);
		switch(i){//����˸��н���ѡ�� 
			case 0:
				S[count++] = S1[c][r];//c��r�е�Ԫ�� 
				break;
			case 6:
				S[count++] = S2[c][r];//c��r�е�Ԫ�� 
				break;
			case 12:
				S[count++] = S3[c][r];//c��r�е�Ԫ�� 
				break;
			case 18:
				S[count++] = S4[c][r];//c��r�е�Ԫ�� 
				break;
			case 24:
				S[count++] = S5[c][r];//c��r�е�Ԫ�� 
				break;
			case 30:
				S[count++] = S6[c][r];//c��r�е�Ԫ�� 
				break;
			case 36:
				S[count++] = S7[c][r];//c��r�е�Ԫ�� 
				break;	
			case 42:
				S[count++] = S8[c][r];//c��r�е�Ԫ�� 
				break;		
		};			
	}//�õ�S�е���� int S[8],ʮ����Ԫ�� 
	count=0;
	for(i=0;i<8;++i){//����32λ����CS 
		temp = S[i];
		for(j=0;j<4;++j){
			CS[count] = HX[temp][j];
			++count;
		}
	}
	CS[32] = '\0';
//	printf("%s\n",CS);
	//p�û� 
	count = 0; 
	for(i=0;i<4;++i){
		for(j=0;j<8;++j){
			temp = P[i][j];
			PCS[count] = CS[temp-1];
			++count;
		}
	};
	PCS[32] = '\0';
//	printf("%s\n",PCS);
	return PCS; 
}

/*int main(){
	char R[32] = {0};
	char C[2] = {0};
	char K[48] = {0};
	int i,j;
	for(i=0;i<32;++i){
			R[i] = '1';
	}
	for(i=0;i<2;++i){
		C[i] = '/0';
	}
	for(i=0;i<48;++i){
		if(i%2==0){
			K[i] = '0';
		}
		else{
			K[i] = '1';
		}
	}
	R[32] = '\0';
	K[48] = '\0';
	printf("R:%s\nK:%s\n",R,K);
	char * F = f_function(R,K);
	printf("F:%s",F);
	return 0;
}
*/



