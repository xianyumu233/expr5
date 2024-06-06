#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#define SERVPORT 3333
#define BACKLOG 10
#define MAXDATASIZE 100
int main() {
	int sockfd,client_fd;  //�����socket��ʶ  �ͻ���socket��ʶ
	struct sockaddr_in my_addr;  //�����׽��ֱ���
	struct sockaddr_in remote_addr;
	int sin_size;
	int recvbytes;
	int i;
	int answer;
	char *delim=",";
	char *p;
	char *str[3];
	char buf[MAXDATASIZE];
	//����˴���socket
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) {
		perror("error in create socket");
		exit(1);
	}
	//��ʼ���׽��ֱ���
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	//������˱�ʶ���׽��ְ�
	if(bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr))==-1) {
		perror("error in bind");
		exit(1);
	}
	//��������֪ͨ���ģ�׼���ý������Կͻ�������
	if(listen(sockfd,BACKLOG)==-1) {
		perror("error in listen");
		exit(1);
	}

	while(1) {
		sin_size=sizeof(struct sockaddr_in);
		if((client_fd=accept(sockfd,(struct sockaddr *)&remote_addr,&sin_size))==-1)
//�������˽���һ�οͻ��˷�������ӣ�����һ���ͻ��˱�ʶ
		{
			perror("error in accept");
			continue;
		}
		printf("received a connection from %s\n",inet_ntoa(remote_addr.sin_addr));
//IP��ַת��
		if(!fork()) {
			if(send(client_fd,"Server can calculate.Please enter :\n",40,0)==-1)
//��ͻ��˷�����Ϣ
			{
				perror("error in send");
				exit(1);
			}
			if((recvbytes=recv(client_fd,buf,MAXDATASIZE,0))==-1)
//���տͻ��˷�������Ϣ
			{
				perror("error in recive");
				exit(1);
			}
			//�ֽڴ���
			buf[recvbytes]='\0';
			str[0]=strtok(buf,delim);
			for(i=1; i<3; i++)
				str[i]=strtok(NULL,delim);

			if(strcmp("+",str[0])==0) { //������ж�
				answer=atoi(str[1])+atoi(str[2]);  //����
				//���ؽ��
				if(send(client_fd,&answer,sizeof(answer),0)==-1) {
					perror("error in send");
				}
			} else if(strcmp("-",str[0])==0) {
				answer=atoi(str[1])-atoi(str[2]);
				if(send(client_fd,&answer,sizeof(answer),0)==-1) {
					perror("error in send");
				}
			} else if(strcmp("*",str[0])==0) {
				answer=atoi(str[1])*atoi(str[2]);
				if(send(client_fd,&answer,sizeof(answer),0)==-1) {
					perror("error in send");
				}
			} else if(strcmp("/",str[0])==0) {
				answer=atoi(str[1])/atoi(str[2]);
				if(send(client_fd,&answer,sizeof(answer),0)==-1) {
					perror("error in send");
				}
			}
		}
		close(client_fd);  //�رտͻ�������
	}
}

