#define _CRT_SECURE_NO_WARNINGS
#include "staticdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "thekeys����������Կ��.c" 
#include "desfunction���ֺ���f��.c"
//char plaintext[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//char keytext[16] = {'1','3','3','4','5','7','7','9','9','B','B','C','D','F','F','1'};
char DV[65]; //��ʼ�����Ķ���������//CFB�õ�ȫ�ֱ��� 
int DESEk(const char* plaintext, const char* keytext, char** ciphertext){//plaintext��ʮ�����Ƶ��ַ����У�64λ����keytext��һ��ʮ�������ַ������С� 
	/*���������Ƕ������ַ�������ciphertext��*/
	int i,j,k,z = 0;
	int temp;
	int tDP[17] = {0};//ʮ����������תΪʮ������ֵ���� 
	char BP[65];//64λ�������������� 
	char iPP[65];//�û���Ķ�����64λ�������� 
	char BPP[65];//תΪ���������е����� 
	char PR[36];
	char PL[36];
	char PR1[36];
	char PL1[36];
	char*Ftr=NULL;
	char ciph[65];
	char Pciph[65]={'\0'};
	char *F =NULL;
	int count = 0;
	/*ʮ�������ַ�תΪʮ������ֵ*/

	for(i=0;i<16;++i){
		switch(plaintext[i]){
			case '0':
				tDP[i] = 0;
				break;
			case '1':
				tDP[i] = 1;
				break;
			case '2':
				tDP[i] = 2;
				break;
			case '3':
				tDP[i] = 3;
				break;			
			case '4':
				tDP[i] = 4;
				break;
			case '5':
				tDP[i] = 5;
				break;			
			case '6':
				tDP[i] = 6;
				break;
			case '7':
				tDP[i] = 7;
				break;			
			case '8':
				tDP[i] = 8;
				break;
			case '9':
				tDP[i] = 9;
				break;			
			case 'A':
				tDP[i] = 10;
				break;
			case 'B':
				tDP[i] = 11;
				break;			
			case 'C':
				tDP[i] = 12;
				break;
			case 'D':
				tDP[i] = 13;
				break;			
			case 'E':
				tDP[i] = 14;
				break;
			case 'F':
				tDP[i] = 15;
				break;
		}
	} 
	//תΪ�������ַ�����
//	printf("%c\n%c\n%c\n%c\n",HX[14][0],HX[14][1],HX[14][2],HX[14][3]);
	count = 0;
	for(i=0;i<16;++i){
		temp = tDP[i];		
		for(j=0;j<4;++j){
			BP[count] = HX[temp][j];
			DV[count] = HX[temp][j]; //CFB�� 
			++count;
		}
	}
		/*��ʼIP�û�*/
	count = 0;
	for(i=0;i<8;++i){
		for(j=0;j<8;++j){
			temp = iIP[i][j];
			iPP[count] = BP[temp-1];
			++count; 
		}
	}
	/*����16�ֱ任*/
	//��ΪPL PR����ʮ��λ���� 
	for(i=0;i<64;++i){
		if(i<32){
			PL[i] = iPP[i]; 
		}
		else{
			PR[i-32] = iPP[i];
		}
	}
	//�ַ���Կ 
	KeySep(keytext);
	//����16�ּ��� 
	for(i=0;i<16;++i){
		if(i%2==0){
			memcpy(PL1,PR,sizeof(PR));
			PL1[32]='\0';
			Ftr = Ks[i];
			F = f_function(PR,Ftr);
			//�������F��Li��� �õ�Ri+1 
			for(j=0;j<32;++j){
				if(PL[j] == F[j]){
					PR1[j] = '0';
				}
				else{
					PR1[j] = '1';
				}
			}
			PR1[32] = '\0';						
		}
		else if(i%2==1){
			memcpy(PL,PR1,sizeof(PR1));
			PL[32]='\0';
			Ftr = Ks[i];
			F = f_function(PR1,Ftr);
			//�������F��Li��� �õ�Ri+1 
			for(j=0;j<32;++j){
				if(PL1[j] == F[j]){
					PR[j] = '0';
				}
				else{
					PR[j] = '1';
				}
			}
			PR[32] = '\0';			
		}
	}
	//���յõ����Ǽ��ܺ��PL��PR��
	//��LR��λΪRL�洢��ciph�У�
	for(i=0;i<64;++i){
		if(i<32){
			ciph[i] = PR[i];
		}
		else{
			ciph[i] = PL[i-32];
		}
	}
	ciph[64] = '\0';

	/*�����������û����õ�����ciphertext*/
	z=0;
	for(i=0;i<8;++i){
		for(j=0;j<8;++j){
			temp = niIP[i][j];
			Pciph[z] = ciph[temp-1];
			++z;
		}
	}
	Pciph[64] = '\0';
	*ciphertext = Pciph;
	printf("����������%s\n",Pciph);

}
//int main(){
//	char*ciphertext=(char)malloc(65);
//	DESEk(plaintext,keytext, &ciphertext);
//	printf("C:%s",ciphertext);
//	return 0;
//}
