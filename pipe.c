#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
	int d1[2];  //�ܵ���������P1-P2
	int d2[2];  //�ܵ���������P2-P3
	int d3[2];  //�ܵ���������P2-P4
	int r,j,k;   //�洢�ܵ������ķ���ֵ
	char buff[200];  //�û�������ַ���
	printf("Please input a string: ");
	scanf("%s",buff);
	//�����ܵ�
	r=pipe(d1);
	if(r==-1) {
		printf("�����ܵ�ʧ��1\n");
		exit(1);
	}

	r=pipe(d2);
	if(r==-1) {
		printf("�����ܵ�ʧ��2\n");
		exit(1);
	}
	r=pipe(d3);
	if(r==-1) {
		printf("�����ܵ�ʧ��3\n");
		exit(1);
	}
	//P1�����ӽ���P2
	r=fork();
	if(r) { //��P2ִ��
		close(d1[1]);       //P1����ر�д
		read(d1[0],buff,sizeof(buff));    //P2��pipe1�ж���Ϣ
		if(strlen(buff)%2 == 1) { //�ж��ַ������� ��Ϊ����
			j=fork();  //�����ӽ���P3
			if(j) { //P3ִ��
				close(d2[1]);  //�ر�P2��д
				read(d2[0],buff,sizeof(buff));  //P3��pipe2�ж�
				printf("P3 pipe2 odd length string��%s\n",buff);
				close(d2[0]);  //�ر�P2�Ķ�
				exit(0);
			} else { //P2ִ��
				close(d2[0]);  //�ر�P3�Ķ�
				write(d2[1],buff,strlen(buff));  //P2д��pipe2
				printf("P2 finishes writing to pipe2.\n");
				close(d2[1]);  //�ر�д
				exit(0);
			}
		} else { //�ַ�������Ϊż��
			k=fork();  //����P4
			if(k) { //P4ִ��
				close(d3[1]);  //�ر�P2��д
				read(d3[0],buff,sizeof(buff));  //��pipe2�ж�
				printf("P4 pipe3 even length string��%s\n",buff);
				close(d3[0]);  //�رն�
				exit(0);
			} else { //P2ִ��
				close(d3[0]);  //�ر�P4�Ķ�
				write(d3[1],buff,strlen(buff));  //P2д��pipe3
				printf("P2 finishes writing to pipe3.\n");
				close(d3[1]);  //�ر�д
				exit(0);
			}
		}
	} else { //P1ִ��
		close(d1[0]);  //�ر�pipe1 P2�Ķ�
		write(d1[1],buff,strlen(buff));  //���û�������ַ���д��pipe1
		close(d1[1]);  //�ر�д
		exit(0);
	}
}
