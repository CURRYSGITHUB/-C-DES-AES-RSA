#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#define MAX_SIZE 65536
//65536
int main(void)
{
	int i,M,b,j;
	int max;
    FILE *fp;//�����ļ�ָ��
    char str[]="";
	M = MAX_SIZE; //����i,M
	fp = fopen("plaintest.txt","w");//��ָ��Ŀ¼�´���.txt�ļ�
    srand(time(NULL)); //��ʼ��ʱ������
    max=(int)pow(2,7) - 6; //����������ֵ
    for(j=0;j<M;j++)
    {
		b=rand()%(16); //����(0~15)  rand����% ��b - a + 1��+ a
		if(b<2||b>7){
		 	--j; 
		 	continue;
		}
	    fprintf(fp,"%X",b); //�������д���ļ�
    }
    fclose(fp); //�ر��ļ�
    return 0;

} 

