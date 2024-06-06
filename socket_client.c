#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define SERVPORT 3333
#define MAXDATASIZE 100
int main(int argc,char *argv[]) {
	int sockfd,recvbytes;  //�ͻ����׽��ֱ�ʶ��������Ϣ��ʶ
	char buf[MAXDATASIZE];  //������Ϣ������
	struct hostent *host;
	struct sockaddr_in serv_addr;  //�����׽���
	while(1) {
		if(argc<2) {
			fprintf(stderr,"Please enter :\n");
			exit(1);
		}

		if((host=gethostbyname(argv[1]))==NULL) {
			herror("error in gethostbyname");
			exit(1);
		}
//�����ͻ���Socket
		if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) {
			perror("error in create socket");
			exit(1);
		}
		//��ʼ���ͻ����׽���
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_port=htons(SERVPORT);
		serv_addr.sin_addr=*((struct in_addr *)host->h_addr);
		bzero(&(serv_addr.sin_zero),8);
		//�ͻ��������˷���һ������
		if(connect(sockfd,(struct sockaddr *) &
		           serv_addr,sizeof(struct sockaddr ) )== -1) {
			perror("error in connect");
			exit(1);
		}
		//�ӷ���˽���������Ϣ
		if((recvbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1) {
			perror("error in recive");
			exit(1);
		}
		//�ֽ�ת��
		buf[recvbytes]='\0';
		printf("Recive:%s",buf);
		scanf("%s",buf);
		buf[strlen(buf)]='\0';
		//�ͻ��˷��ͼ������ͼ�����
		if(send(sockfd,buf,MAXDATASIZE,0)==-1) {
			perror("error in send");
			exit(1);
		}
		//���ռ�����
		int result=0;
		if((recvbytes=recv(sockfd,&result,sizeof(result),0))==-1) {
			perror("error in recive");
			exit(1);
		}
		if(buf[0]=='q') break;
		printf("The answer is :%d\n",result);
		close(sockfd);  //�رտͻ����׽���
	}
}
