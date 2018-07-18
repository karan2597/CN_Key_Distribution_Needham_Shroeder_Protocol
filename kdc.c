#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"crypto.h"

void error(const char *msg)
{	perror(msg);
	exit(1);
}

int main(int argc,char *argv[])
{	/*if(argc<2)
	{	printf("Port no.not provided.Program terminated\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[1024];

	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen;
	 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{	error("Error opening socket.");
	}

	bzero((char *) &serv_addr,sizeof(serv_addr));
	portno=atoi(argv[1]);

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
            error("Binding failed");

    	 listen(sockfd, 5);
	clilen=sizeof(cli_addr);

    	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd<0)
		error("Error on accept");
	
	
    	
	while(1)
	{
		bzero(buffer,1024);
		n=read(newsockfd,buffer,1024);
		if(n<0)
			error("error on reading");
		printf("Client:%s\n",buffer);

		bzero(buffer,1024);
		fgets(buffer,1024,stdin);
		n=write(newsockfd,buffer,strlen(buffer));
		if(n<0)
			error("error on reading");
		int i=strncmp("bye",buffer,3);
		if(i==0)
			break;
	}
	close(newsockfd);
	close(sockfd);
	return 0;*/

	if(argc<2)
	{	printf("Port no.not provided.Program terminated\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[1024];
	int ka,kb=4;
	int ks;
	char temp[10];
	char ida,idb;int nonce1;

	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen;
	 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{	error("Error opening socket.");
	}

	bzero((char *) &serv_addr,sizeof(serv_addr));
	portno=atoi(argv[1]);

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
            error("Binding failed");

    	 listen(sockfd, 5);
	clilen=sizeof(cli_addr);

    	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd<0)
		error("Error on accept");

			

	bzero(buffer,1024);
	n=read(newsockfd,buffer,1024);
	if(n<0)
		error("error on reading");
	FILE *fptr;
    fptr = (fopen("ka.txt", "r"));
    fscanf(fptr,"%d",&ka);
    fclose(fptr);
	printf("Message from A\n");
	printf("Decrypting...\n");
	decrypt(ka,buffer);
	printf("Packet received from A:[%c,%c,%c]\n",buffer[0],buffer[1],buffer[2]);
	ida=buffer[0];
	idb=buffer[1];
	temp[0]=buffer[2];
	nonce1=atoi(temp);
	printf("Enter Session key:");
	scanf("%d",&ks);

	/*fptr = (fopen("kb.txt", "r"));
    fscanf(fptr,"%d",&kb);
    fclose(fptr);*/
	
	printf("Encrypting...\n");
	bzero(buffer,1024);
	sprintf(temp,"%d",ks);
	buffer[0]=temp[0];
	buffer[1]=ida;
	buffer[2]=idb;
	bzero(temp,10);
	sprintf(temp,"%d",nonce1);
	buffer[3]=temp[0];
	bzero(temp,10);
	sprintf(temp,"%d",ks);
	encrypt(kb,temp);
	buffer[4]=temp[0];
	bzero(temp,10);
	temp[0]=ida;
	encrypt(kb,temp);
	buffer[5]=temp[0];

	encrypt(ka,buffer);
	n=write(newsockfd,buffer,strlen(buffer));
	if(n<0)
		error("error on transmitting");
	printf("Packet sent to A:[%c,%c,%c,%c,%c,%c]\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
		

	
	close(sockfd);
	return 0;

}

