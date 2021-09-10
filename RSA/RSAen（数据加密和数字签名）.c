#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include<stdlib.h>
#include<time.h> 
typedef uint8_t byte;
typedef uint32_t word;
typedef uint64_t LL;

char HX[16][4] = {'0','0','0','0',
				  '0','0','0','1',
				  '0','0','1','0',
				  '0','0','1','1',
				  '0','1','0','0',
				  '0','1','0','1',
				  '0','1','1','0',
				  '0','1','1','1',
				  '1','0','0','0',
				  '1','0','0','1',
				  '1','0','1','0',
				  '1','0','1','1',
				  '1','1','0','0',
				  '1','1','0','1',
				  '1','1','1','0',
				  '1','1','1','1'};
byte MaxN[64] = {0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf}; 
int division(const byte*PM,const byte*N,const int n1,const int n3,byte*answer,byte*mod){
	//PM/N��n1,n3�ֱ�ΪPM��N�Ĵ�С��Ҳ����Ӧ����Ч��������answerΪ�̣�modΪ������
	//����mod����Ч������ 
	if(n1<n3){
		puts("������С");
		memset(answer,0x0,64);
		memset(mod,0x0,64);
		memcpy(mod+64-n1,PM,n1);
		return n1; 
	}
	if(n1==n3){
		puts("������Ŀ���ڱ�������Ŀ"); 
	}
	byte count[1] = {0x01};//������
	int i,j,k = 0;
	byte answ[n1-n3+1];
	byte temp[64];//���� 
	byte temp1[64];//���� 
	//������0 
	for(i=0;i<64;++i){
		temp[i] = 0x00;
		temp1[i] = 0x00;
	}
	for(i=0;i<n1-n3+1;++i){
		answ[i] = 0x00;
	}
	//���뻺��
	for(i=0;i<n3;++i){
		temp[63-i] = PM[n3-1-i];
		temp1[63-i] = PM[n3-1-i];
	} 
	//��ʼ����
//	printf("%x",temp[63]);
	int efn3 = n3;//temp��Ч��λ���� (��λ0);
	int s,t = 0;
	k=0;
	for(i=n3-1;i<n1;++i){
		if(efn3==n3){//������Чλ��ͬ�� 
			j = compare0x(temp,N,64,n3);//temp�ܴ�С����64����ЧλͬN��Ϊn3�� 
			if(j==1){//temp�� 
				while(1){
					++count[0];
					t = xmodn(N,count,MaxN,temp,n3,1,64);//�˴�temp������� N*count ���ֵ��tΪtemp��ʱ����Чλ
					s = 64 - t;//temp�ĸ�λ0����(sһ��С�ڵ���efn3) 
					j = compare0x(temp,temp1+s,64,t);//temp1Ϊû��ʼN*countǰ��tempֵ 
					if(j==1){//N*count > temp1 
						count[0]--;
						answ[k] = count[0];//�������ˣ���һ�ּ�Ϊ����ֵ 
						++k;

						t = xmodn(N,count,MaxN,temp,n3,1,64);
						s = 64 - t;//��λ0�ĸ���						
						memcpy(temp,temp+s,t);
						memset(temp+t,0x0,s);
						
						efn3 = sub0x(temp1,temp,efn3,64,t);//temp1һֱ�ݴ���tempǰһ�β�����ֵ 
						//memcpy������λȡֵ���� 
						memcpy(temp,temp1+1,63);
						temp[63] = PM[n3-1+k];
						memcpy(temp1,temp,64);
						++efn3;
						count[0]=0x01;
						s = 0; 
						break;
					}
					else if(j==0){
						continue;
					}
					else if(j==-1){
						puts("ҲҪע��");
						answ[k] = count[0];
						++k;
						memset(temp,0x0,64);
						temp[63] = PM[n3-1+k];
						memcpy(temp1,temp,64);
						efn3 = 1;
						count[0]=0x01;
						break; 
					}
				}		
			}
			else if(j==0){//tempС
				answ[k] = 0x0;
				++k;
				memcpy(temp1,temp+1,63);
				temp1[63] = PM[n3-1+k];
				memcpy(temp,temp1,64);
				++efn3;
			}
			else if(j==-1){//���
				puts("ע��"); 
				answ[k] = 0x1;
				++k;
				memset(temp,0x0,64);
				temp[63] = PM[n3-1+k];
				memcpy(temp1,temp,64);
				efn3 = 1;
			}
		}
		else if(efn3>n3){
			j = n3;
			while(j<efn3){
				++count[0];
				j = xmodn(N,count,MaxN,temp,n3,1,64);//�˴�temp������� N*count ���ֵ��jΪtemp��ʱ����Чλ
			}
			if(j==efn3){
				s = 64 - j;
				j = compare0x(temp1,temp+s,64,j);//temp�ܴ�С����64����ЧλͬN��Ϊn3�� 
				if(j==1){//temp1�� 
					while(1){
						++count[0];
						t = xmodn(N,count,MaxN,temp,n3,1,64);//�˴�temp������� N*count ���ֵ��tΪtemp��ʱ����Чλ
						s = 64 - t;
						j = compare0x(temp,temp1+s,64,t);//temp1Ϊû��ʼN*countǰ��tempֵ 
						if(j==1){//N*count > temp1 
							count[0]--;
							answ[k] = count[0];//�������ˣ���һ�ּ�Ϊ����ֵ 
							++k;
	
							t = xmodn(N,count,MaxN,temp,n3,1,64);
							s = 64 - t;//��λ0�ĸ���						
							memcpy(temp,temp+s,t);
							memset(temp+t,0,s);
							
							efn3 = sub0x(temp1,temp,efn3,64,t);//temp1һֱ�ݴ���tempǰһ�β�����ֵ 
							//memcpy������λȡֵ���� 
							memcpy(temp,temp1+1,63);
							temp[63] = PM[n3-1+k];
							memcpy(temp1,temp,64);
							++efn3;
							count[0]=0x01;
							s = 0; 
							break;
						}
						else if(j==0){
							continue;
						}
						else if(j==-1){
							puts("ҲҪע���");
							answ[k] = count[0];
							++k;
							memset(temp,0x0,64);
							temp[63] = PM[n3-1+k];
							memcpy(temp1,temp,64);
							efn3 = 1;
							count[0]=0x01;
							break; 
						}
					}		
				}
				else if(j==0){//temp1С
					count[0]--;
					answ[k] = count[0];//�������ˣ���һ�ּ�Ϊ����ֵ 
					++k;

					t = xmodn(N,count,MaxN,temp,n3,1,64);
					s = 64 - t;//��λ0�ĸ���						
					memcpy(temp,temp+s,t);
					memset(temp+t,0,s);
					
					efn3 = sub0x(temp1,temp,efn3,64,t);//temp1һֱ�ݴ���tempǰһ�β�����ֵ 
					//memcpy������λȡֵ���� 
					memcpy(temp,temp1+1,63);
					temp[63] = PM[n3-1+k];
					memcpy(temp1,temp,64);
					++efn3;
					count[0]=0x01;
					s = 0; 
				}
				else if(j==-1){//���
					puts("Ҳע��"); 
					answ[k] = count[0];
					++k;
					memset(temp,0x0,64);
					temp[63] = PM[n3-1+k];
					memcpy(temp1,temp,64);
					efn3 = 1;
					count[0] = 0x01;
				}	
			}
								
			else if(j>efn3){
				--count[0];
				answ[k] = count[0];//�������ˣ���һ�ּ�Ϊ����ֵ 
				++k;
				j = xmodn(N,count,MaxN,temp,n3,1,64);
				s = 64 - j;//��λ0�ĸ���						
				memcpy(temp,temp+s,j);
				memset(temp+j,0,s);
				efn3 = sub0x(temp1,temp,efn3,64,j);//temp1һֱ�ݴ���tempǰһ�β�����ֵ 
				//memcpy������λȡֵ���� 
				memcpy(temp,temp1+1,63);
				temp[63] = PM[n3-1+k];
				memcpy(temp1,temp,64);
				++efn3;
				count[0]=0x01;
				s = 0;	
			}
		}
		else if(efn3<n3){
			answ[k] = 0x0;
			++k;
			memcpy(temp1,temp+1,63);
			temp1[63] = PM[n3-1+k];
			memcpy(temp,temp1,64);
			++efn3;
		}
		
	}
	
	//����а�����PM�����һ��β�� 
	memcpy(temp+1,temp,63);
	temp[0] = 0x00;
	efn3--; //��������Ч��λ 
	
	memcpy(answer,answ,n1-n3+1);
	memcpy(mod,temp,64);
	return efn3;
}
int compare0x(const byte*output,const byte*N,const int n1,const int n3){
	//�Ƚ�output��N�Ĵ�С��n3Ϊ���ߵĸ���(��Ч)
	//��output���򷵻�1����outputС���򷵻�0,����ȣ��򷵻�-1 
	int i,j,k=0;
	for(i=0;i<n3;++i){
		if(output[n1-n3+i]>N[i]){
			return 1;
		}
		if(output[n1-n3+i]<N[i]){
			return 0;
		}
	}
	return -1; 
}
int sub0x(byte*output,const byte*N,const int count,const int n1, const int n3){
	//output��N������output����Чλ������countΪoutput��Чλ������n3ΪN�Ĵ�С,n1Ϊoutput�Ĵ�С�� 
	//Ĭ��output�Ǵ��һ������count>n3
	if(count<n3){
		puts("�������");
		return; 
	}

	int cn = count;//��Чλ 
	int ca = n1;//�ܴ�С 
	byte out[ca];
	int i,j,k = 0;
//	printf("%d\n",ca);

	//����,��ʼ���洢�������out 
	for(i=0;i<ca;++i){
		out[i]=0x00;
	}
	//��ʼ����
	byte A = 0x00;//��λ 
	byte C = 0x00;//��λ
	int ff = 0;
	int n = n3-1;
	for(i=ca-1;i>ca-cn-1;--i){
		if(output[i]!=0){
			output[i] = output[i] - C;			
		}
		else if(output[i]==0){
			if(C==1){
				output[i] = 0x10 - C;//�˴���λ�����Ϊf���Ǽ���ֵ�� 
				ff = 1; 
			}
		}
		if(output[i]<N[n]){
			C=0x01;//��λ 
			A = output[i]+0x10-N[n];
//			printf("%x\n",A);
		}
		if(output[i]>=N[n]){
			if(ff==1){
				C=0x01;
				ff=0;
			}
			else if(ff==0){
				C=0x00;
			}
			A = output[i]-N[n];
		}
		--n;
		if(n<0){//������������ 
			out[ca-1] = A;
			ca = i;//��¼�����м�����λ 
			break;
		}
		out[ca-1] = A;
		--ca;
	}
	while(output[ca-1]==0){
		if(C==0){
			break;
		}
		if(C==1){
			A = 0x10 - C;
//			printf("%x\n",A);
			out[ca-1] = A;
			--ca;
			C=0x01;
		}
	}
	output[ca-1] = output[ca-1] - C;
	for(i=ca-1;i>-1;--i){
		A = output[i];
//		printf("%x\n",A);
		out[i] = A;
	}
	int flag=0;//��¼��λ�ж����� 
	for(i=0;i<n1;++i){
		if(out[i]==0x00){
			++flag;
		}
		else{
			break;
		}
	}
	cn = n1 - flag;//��Ч��λ;
	
//	for(i=0;i<ca;++i){
//		printf("%x",out[i]);
//	}
	
	//��out��ֵ��output
	for(i=0;i<n1;++i){
		output[i] = out[i];
	}
	return cn ;
}
int xmodn(const byte*P,const byte*M,const byte*N, byte*output, const int n1,const int n2,const int n3){//Ĭ��n1>=n2 
// ��P��M����mod n�ĳ˷����㣬NΪģ�������߶���ʮ��������ֵ���������output(��СΪn3)�n1Ϊp��ʮ������λ����n2ΪM��λ��,n3Ϊģ����λ�������շ���output����Чλ����
	int count = n1+n2;
	byte out[count+1];
	int i,j,k=0;
	//����,��ʼ���洢�˻�������out 
	for(i=0;i<count+1;++i){
		out[i]=0x00;
	}
	//��ʼ�˷� 
	byte A = 0x00;//��λ 
	byte C = 0x00;//��λ
	byte T = 0x00;//�����˻�
	count = n1+n2;
	byte A1 = 0x00;//��λ 
	byte C1 = 0x00;//��λ
	int flag = 0;//ָʾ���� 
	int temp = 0;//����countֵ 
	for(i=n2-1;i>-1;--i){
		for(j=n1-1;j>-1;--j){
			T = P[j]*M[i];
//			printf("%x\n",T);
			A = T%0x10 + C;
//			printf("%x\n",A);
			C = T/0x10 + A/0x10;
//			printf("%x\n",C);
			A = A%0x10;
//			printf("%x\n",A);
			out[count] += A;
			temp = count; 
			while(out[count]/0x10 != 0x00){//������λ 
//				printf("%x\n",out[count]);
				A1 = out[count]%0x10;
				C1 = out[count]/0x10;
				out[count] = A1;
				--count;
				out[count] += C1; 				
			}
			count = temp; 
			--count; 
		}
		++flag;
		 
		out[count] += C;
//		for(k=0;k<5;++k){
//			printf("%x",out[k]);
//		}
//		puts("");
		while(out[count]/0x10 != 0x00){//������λ 
			A1 = out[count]%0x10;
			C1 = out[count]/0x10;
			out[count] = A1;
			--count;
			out[count] += C1; 				
		}
						
		A = 0x00;//��λ 
		C = 0x00;//��λ
		T = 0x00;//�����˻�
		count = n1+n2 - flag;
	}
	//�õ�ԭʼ�˻�out

	i = 0;
	flag = 0;
	while(out[i]==0x00){
		++flag;
		++i;
	}
	//�õ���λ0�ĸ���flag��
	count = n1 + n2 + 1 - flag;//�õ�ԭʼ�˻�����Чλ����
	temp = count;
	byte outs[count];
	for(i=0;i<count;++i){
		outs[i] = out[flag];
		++flag;
	}
	
//	for(k=0;k<temp;++k){
//		printf("%x ",outs[k]);
//	}
//	puts("");

	//���������ڵ���ģN�ĸ���,��ʼ�ж��Ƿ����ģ���� 
	while(count>n3){
		count = sub0x(outs,N,count,temp,n3);//tempΪouts�Ĵ�С��countΪouts����Чλ��
//		for(k=0;k<temp;++k){
//			printf("%x ",outs[k]);
//		} 
		puts("go");
	}
	

	while(count==n3){
		i = compare0x(outs,N,temp,n3);//tempΪouts�Ĵ�С��countΪouts����Чλ���˴���Чλͬn3 
		if(i==1){//out�� 
			count = sub0x(outs,N,count,temp,n3);//��ȥһ��N�����������outs�У�����outs����Ч����
		}
		if(i==0){//outС
			break; 
		}
		if(i==-1){//���
			puts("error"); 
		}		
	}
	
	if(count<=n3){
		for(i=0;i<count;++i){
			output[n3-1-i]=outs[temp-1-i];
		}
		return count;
	} 
}


int main(){
	byte p[24] = {0x6,0x3,0x7,0x2,0x7,0x9,0x7,0x0,0x7,0x4,0x6,0xf,0x6,0x7,0x7,0x2,0x6,0x1,0x7,0x0,0x6,0x8,0x7,0x9};//63727970746F677261706879
//	printf("%d",sizeof(p));
	byte d[26] = {0x6,0x3,0xC,0x3,0x2,0x6,0x4,0xA,0x0,0xB,0xF,0x3,0xA,0x4,0xF,0xC,0x0,0xF,0xF,0x0,0x9,0x4,0x0,0x9,0x3,0x5};//63C3264A0BF3A4FC0FF0940935
	byte n[26] = {0x7,0x3,0x2,0x9,0x9,0xB,0x4,0x2,0xD,0xB,0xD,0x9,0x5,0x9,0xC,0xD,0xB,0x3,0xF,0xB,0x1,0x7,0x6,0xB,0xD,0x1}; //73299B42DBD959CDB3FB176BD1 
	unsigned long long e = 0x10001;
	char nd[104];
	int i,j,s,t= 0;
	int choice;
	int tp;
	uint64_t k = 0x0;
//	byte p[3] = {0x0,0x3,0x1};

	byte out[64];
	for(i=0;i<64;++i){
		out[i]=0x00;
	}
	//��˽ԿdתΪ���������У�׼��ʹ��ģƽ���˷� 
	for(i=0;i<26;++i){
		tp = d[i];
		for(j=0;j<4;++j){
			nd[4*i+j] = HX[tp][j];
		}
	}
	
	byte tempp[26];
	memset(tempp,0x00,26);
	i = xmodn(p,p,MaxN,out,24,24,64);
	s = i;
	memcpy(out,out+64-i,i);
	memset(out+i,0x0,64-i);
	t = sizeof(n);
	byte answer[64];
	memset(answer,0x0,64);
	byte mod[64];
	memset(mod,0x0,64);
	printf("��ѡ����Ӧ���ܣ�����1��Ϊ���ݼ��ܣ�����2��Ϊ����ǩ������ : ");
	scanf("%d",&choice);
	puts("");
	
	if(choice == 2){
		//����ǩ��(ģƽ���˷� )
		//a=p[]
		//temmp������һ�ֵ�mod 
		for(k=0;k<104;++k){
			printf("��%d��\n",k); 
			if(nd[k]=='1'){
				if(k==0){
					memcpy(tempp,p,24);
					i = 24;
				}
				else{//tempp^2*p 
					//temppƽ�� 
					s = i;//s����temp����Чλ 
					i = xmodn(tempp,tempp,MaxN,out,s,s,64);//i����out����Чλ 
					memcpy(out,out+64-i,i);
					memset(out+i,0x0,64-i);
					//��out��mod
					i = division(out,n,i,t,answer,mod);//i����mod����Чλ
		//			printf("%x",mod[63]);
					memset(tempp,0x0,26);
					memcpy(tempp,mod+64-i,i);//tempp����mod 
					memset(answer,0x0,64);
					memset(mod,0x0,64);
					memset(out,0x0,64);
					//*p
	
					i = xmodn(p,tempp,MaxN,out,24,i,64);//i����out����Чλ
					memcpy(out,out+64-i,i);
					memset(out+i,0x0,64-i);
					i = division(out,n,i,t,answer,mod);//i����mod����Чλ 
					memset(tempp,0x0,26);
					memcpy(tempp,mod+64-i,i);//tempp����mod 
	
					memset(answer,0x0,64);
					memset(mod,0x0,64);
					memset(out,0x0,64);	
					//i Ϊ tempp����Чλ 
				}
			}
			else if(nd[k]=='0'){
				if(k==0){
					tempp[0] = 0x1;
					i = 1;
				}
				else{//tempp^2
					//temppƽ�� 
					s = i;//s����tempp����Чλ 
					i = xmodn(tempp,tempp,MaxN,out,s,s,64);//i����out����Чλ 
					memcpy(out,out+64-i,i);
					memset(out+i,0x0,64-i);
					//��out��mod
					i = division(out,n,i,t,answer,mod);//i����mod����Чλ 
					memset(tempp,0x0,26);
					memcpy(tempp,mod+64-i,i);//tempp����mod 
					memset(answer,0x0,64);
					memset(mod,0x0,64);
					memset(out,0x0,64);
					//i Ϊ tempp����Чλ	
				}
			}
		}
		printf("���ģ�");
		for(j=0;j<24;++j){
			printf("%x",p[j]);
		}
		printf("\n˽Կ��  d = ");
		for(j=0;j<26;++j){
			printf("%x",d[j]);
		}
		printf("\n ������ n = ");
		for(j=0;j<26;++j){
			printf("%x",n[j]);
		}	 
		printf("\n����ǩ�����Ϊ��  "); 
		for(j=0;j<i;++j){
			printf("%x",tempp[j]);
		} 	
	} 

		
		
	else if(choice==1){
		//���ݼ��� ��δ��ģ�ظ�ƽ������ 
		for(k=0x0;k<e-0x1;++k){
			printf("��%d��\n",k);
			i = division(out,n,s,t,answer,mod);
			if(k == 0xffff){
				break;
			}
			memset(tempp,0x0,26);
			memcpy(tempp,mod+64-i,i);
			memset(out,0x0,64);
			s = i;
			i = xmodn(tempp,p,MaxN,out,s,24,64);
			s = i;
			memcpy(out,out+64-i,i);
			memset(out+i,0x0,64-i);
			memset(answer,0x0,64);
			memset(mod,0x0,64);
		}
		s = i;
//		printf("%d\n",i);
//		puts("");
//		for(j=0;j<s-t+1;j++){
//			printf("%x ",answer[j]);
//		}
//		puts("");
		memcpy(mod,mod+64-i,i);
		memset(mod+i,0x0,64-i);
		printf("���ģ�");
		for(j=0;j<24;++j){
			printf("%x",p[j]);
		}
		printf("\n��Կ��e = %I64x",e);
		printf("   n = ");
		for(j=0;j<26;++j){
			printf("%x",n[j]);
		}	
		printf("\n���ݼ��ܵĽ��Ϊ��"); 
		for(j=0;j<i;++j){
			printf("%x",mod[j]);
		} 	
	}

	return 0; 
}
