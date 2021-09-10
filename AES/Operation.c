#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
typedef uint8_t byte;
typedef uint32_t word;
uint8_t HEX[17]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,'\0'};

	 
//��ʮ��������ֵת��Ϊ�������ַ�����8bit 
char* hex2b(byte hex){
	char bchar[9]={'\0'};
	char temp[10];
	int i = 0;
	sprintf(temp,"%x",hex);
	if(strlen(temp)==1){
		strcat(bchar,"0000");
	}
	for(i=0;i<2;++i){
		switch(temp[i]){
			case '0':
				strcat(bchar,"0000");
				break;
			case '1':
				strcat(bchar,"0001");
				break;
			case '2':
				strcat(bchar,"0010");
				break;
			case '3':
				strcat(bchar,"0011");
				break;			
			case '4':
				strcat(bchar,"0100");;
				break;
			case '5':
				strcat(bchar,"0101");
				break;			
			case '6':
				strcat(bchar,"0110");
				break;
			case '7':
				strcat(bchar,"0111");
				break;			
			case '8':
				strcat(bchar,"1000");
				break;
			case '9':
				strcat(bchar,"1001");
				break;			
			case 'a':
				strcat(bchar,"1010");
				break;
			case 'b':
				strcat(bchar,"1011");
				break;			
			case 'c':
				strcat(bchar,"1100");
				break;
			case 'd':
				strcat(bchar,"1101");
				break;			
			case 'e':
				strcat(bchar,"1110");
				break;
			case 'f':
				strcat(bchar,"1111");
				break;
		}
	}
	return bchar; 
}
//�������ֽ�������� 
char* bxor(char *a,char *b){
	char c[9];
	int i = 0;
	for(i=0;i<8;++i){
		if(a[i]==b[i]){
			c[i] = '0';
		}
		else{
			c[i] = '1';
		}
	}
	c[8] = '\0';
	return c; 
}


//ʮ�������ֽڳ˷� (���ص���ʮ����)
byte bytexbyte(byte a,byte b){
	// ת��Ϊ������
	char *Ba = NULL;
	char TBa[10] = {'\0'};
	char *Bb = NULL;
	char TBb[10] = {'\0'};
	char *tBa = NULL;
	char *tBb = NULL;
	int i,j,flag = 0;
	int x[9];//8bit��Ӧλ��Ҫxtime�Ĵ��� 
	int c[9]={100,100,100,100,100,100,100,100,100};//��¼x[9]�еĲ�Ϊ���λ�ã� 100Ϊ�ձ�� 
	if(a>=b){
		flag = 0;
	}
	else{
		flag = 1;
	}
	Ba = hex2b(a);
	memcpy(TBa,Ba,8);
	Bb = hex2b(b);
	memcpy(TBb,Bb,8);
	tBa = malloc(sizeof(char)*16);
	tBb = malloc(sizeof(char)*16);
	memcpy(tBa,TBa,8);
	memcpy(tBb,TBb,8);
	j = 0;
	if(flag==0){//a>=b
	 	for(i=0;i<8;++i){
	 		if(TBb[i]=='0'){
	 			x[i] = 0;
			}
			else{
				x[i] = 7-i;
				c[j] = i;
				++j;
			}
		}
	}
	else{//b>a
		j=0;
	 	for(i=0;i<8;++i){
	 		if(TBa[i]=='0'){
	 			x[i] = 0;
			}
			else{
				x[i] = 7-i;
				c[j] = i;
				++j;
			}
		}
	}
	i = 0;
	j = 0;
	
	//��ʼx�˷����ۼ���� 
	int xtime = 0;//x�˵Ĵ��� 
	int temp = 0;
	int u,v,w = 0;
	char IB[9] = {'0','0','0','1','1','0','1','1','\0'};
	char answer[9] = {'\0'};
	int answerflag = 0;
	char *tanswer=NULL;
	char *TBaptr = NULL;
	char *TBbptr = NULL;
	while(c[i]!=100){
		temp = c[i];
		xtime = x[temp];
		
		if(flag==0){//a>=b
			while(xtime!=0){
				//����һ��x��
				if(TBa[0]=='1'){//��λΪ1 
					for(u=0;u<7;++u){
						TBa[u] = TBa[u+1];
					}
					TBa[u] = '0';
					TBaptr = bxor(IB,TBa);
					memcpy(TBa,TBaptr,8);						
				}
				else{//��λΪ0 
					//������0
					for(u=0;u<7;++u){
						TBa[u] = TBa[u+1];
					}
					TBa[u] = '0';
				}
				--xtime;
			}
			if(answerflag == 0){
				memcpy(answer,TBa,8);
				answer[8] = '\0';
				memcpy(TBa,tBa,8);//��ԭBa 
				answerflag = 1; 
			}
			else{
				tanswer = bxor(answer,TBa);
				memcpy(answer,tanswer,8);
				answer[8] = '\0';
				memcpy(TBa,tBa,8);
			}
		}
		
		if(flag==1){//b>a
			while(xtime!=0){
				//����һ��x��
				if(TBb[0]=='1'){//��λΪ1 
					for(u=0;u<7;++u){
						TBb[u] = TBb[u+1];
					}
					TBb[u] = '0';
					TBbptr = bxor(IB,TBb);
					memcpy(TBb,TBbptr,8);						
				}
				else{//��λΪ0 
					//������0
					for(u=0;u<7;++u){
						TBb[u] = TBb[u+1];
					}
					TBb[u] = '0';
				}
				--xtime;
			}
			if(answerflag == 0){
				memcpy(answer,TBb,8);
				answer[8] = '\0';
				memcpy(TBb,tBb,8);//��ԭBb 
				answerflag = 1; 
			}
			else{
				tanswer = bxor(answer,TBb);
				memcpy(answer,tanswer,8);
				answer[8] = '\0';
				memcpy(TBb,tBb,8);
			}
		}
		++i;
	}
	//���յõ��ֽ���˽��answer��Ϊ��λ�������ַ����С�
	//���������ַ�����ת��Ϊʮ��������ֵ
	byte answerHex;
	u = 0;
	v = 0;
	w = 0;
	for(u=0;u<8;++u){
		if(u<4){
			if(answer[u]=='1'){
				v += pow(2,3-u);
			}
		}
		else{
			if(answer[u]=='1'){
				w += pow(2,7-u);
			}
		}
	}
	answerHex = HEX[v]*0x10 + HEX[w];
	free(tBa);
	free(tBb);
	return answerHex;
}


/*int main(){
	char xx0[9]={'\0'};
	char xx1[9]={'\0'};
	char xx2[9]={'\0'};
	char xx3[9]={'\0'};
	byte a = 0x90;
	byte b = 0x90;
	byte c = 0xab;
	byte x0 = bytexbyte(0x02,0x9e);
	byte x1 = bytexbyte(0x03,0x09);
	byte x2 = bytexbyte(0x01,0x0d);
	byte x3 = bytexbyte(0x01,0x0b);	 
//	byte c = bytexbyte(a,b);
	char *x0ptr = hex2b(x0);
	strcpy(xx0,x0ptr);
	char *x1ptr = hex2b(x1);
	strcpy(xx1,x1ptr);
	char *x2ptr = hex2b(x2);
	strcpy(xx2,x2ptr);
	char *x3ptr = hex2b(x3);
	strcpy(xx3,x3ptr);
	
	x0ptr = bxor(xx0,xx1);
	strcpy(xx0,x0ptr);
	x1ptr = bxor(xx2,xx3);
	strcpy(xx1,x1ptr);
	x2ptr = bxor(xx0,xx1);
	x2ptr = strcpy(&a,x2ptr);
//	printf("%x\n%x\n%x\n%x\n",x0,x1,x2,x3);
	printf("%s\n%c",x2ptr,a);
	return 0;
}
*/
