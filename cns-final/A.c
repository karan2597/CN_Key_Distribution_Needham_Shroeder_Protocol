#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include"crypto.h"

void error(const char *msg)
{	perror(msg);
	exit(1);
}

int main(int argc,char *argv[])
{	
	/*int sockfd,portno,n;
	char buffer[1024];
	struct hostent *server;
	struct sockaddr_in serv_addr;
	
	if(argc<3)
	{	fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(1);
	}
	portno= atoi(argv[2]);
		 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{	error("Error opening socket.");
	}

	server=gethostbyname(argv[1]);
	if(server== NULL)
		fprintf(stderr,"Error,no such host");

	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *) server->h_addr,(char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Connection failed");

	while(1)
	{	bzero(buffer,1024);
		fgets(buffer,1024,stdin);
		n=write(sockfd,buffer,strlen(buffer));
		
		if(n<0)
			error("error on reading");
		
		bzero(buffer,1024);
		n=read(sockfd,buffer,1024);
		if(n<0)
			error("error on reading");
		printf("server:%s\n",buffer);
		
		int i=strncmp("bye",buffer,3);
		if(i==0)
			break;
	}
	close(sockfd);
	return 0;*/



	int sockfd,portno,n,i;
	char buffer[1024];
	struct hostent *server;
	struct sockaddr_in serv_addr;
	
	char ida,idb;
	int nonce1;
	int ka;
	

	if(argc<3)
	{	fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(1);
	}
	portno= atoi(argv[2]);
		 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{	error("Error opening socket.");
	}

	server=gethostbyname(argv[1]);
	if(server== NULL)
		fprintf(stderr,"Error,no such host");

	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *) server->h_addr,(char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Connection failed");

	ida='A';
	idb='B';
	printf("Enter the private key for A:");
	scanf("%d",&ka);
	char temp[10];
	
	printf("Nonce1:");
	scanf("%d",&nonce1);
 
	FILE *fptr;
    fptr = (fopen("ka.txt", "w"));
    fprintf(fptr,"%d",ka);
    fclose(fptr);

	//if(n<0)
	//	error("error on transmitting");
	printf("Network id of A:%c\nNetwork id of B:%c\nNonce1:%d\nPrivate key of A:%d\n",ida,idb,nonce1,ka);
	bzero(buffer,1024);
	buffer[0]=ida;
	buffer[1]=idb;
	bzero(temp,10);
	sprintf(temp,"%d",nonce1);
	buffer[2]=temp[0];
	encrypt(ka,buffer);
	printf("Encrypting ...");
	printf("\nPacket sent to KDC:[%c,%c,%c]\n",buffer[0],buffer[1],buffer[2]);
	n=write(sockfd,buffer,strlen(buffer));
	if(n<0)
		error("error on transmitting");

	bzero(buffer,1024);
	printf("Message from KDC\n");
	n=read(sockfd,buffer,1024);
	if(n<0)
		error("error on reading");
	printf("Decrypting...\n");
	decrypt(ka,buffer);
	printf("Packet received from KDC:[%c,%c,%c,%c,%c,%c]\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
	
	int ks;
	temp[0]=buffer[0];
	ks=atoi(temp);
	close(sockfd);
	printf("Session key:%d\n",ks);
	

  	//connecting to B
	printf("Portno:");
	scanf("%d",&portno);
		 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{	error("Error opening socket.");
	}
	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Connection failed");

	printf("Packet sent to B:[%c,%c]\n",buffer[4],buffer[5]);
	bzero(temp,10);
	temp[0]=buffer[4];
	temp[1]=buffer[5];
	bzero(buffer,1024);
	n=write(sockfd,temp,strlen(temp));
	if(n<0)
		error("error on transmitting");

	
	bzero(buffer,1024);
	printf("Message from B\n");
	n=read(sockfd,buffer,1024);
	if(n<0)
		error("error on reading");
	printf("Decrypting...\n");
	decrypt(ks,buffer);
	printf("Packet received from B:[%c]\n",buffer[0]);
	int nonce2;
	nonce2=atoi(buffer);
	nonce2+=1;
	
	bzero(buffer,1024);
	sprintf(buffer,"%d",nonce2);
	printf("Encrypting...\n");
	encrypt(ks,buffer);
	n=write(sockfd,buffer,strlen(buffer));
	if(n<0)
		error("error on transmitting");
	printf("enter your message\n");

	
	while(1)
	{	bzero(buffer,1024);
		fgets(buffer,1024,stdin);
		n=write(sockfd,buffer,strlen(buffer));
		
		if(n<0)
			error("error on reading");
		
		bzero(buffer,1024);
		n=read(sockfd,buffer,1024);
		if(n<0)
			error("error on reading");
		printf("server:%s\n",buffer);
		
		int i=strncmp("bye",buffer,3);
		if(i==0)
			break;
	}
	close(sockfd);
	return 0;
}
