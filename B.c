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
{	if(argc<2)
	{	printf("Port no.not provided.Program terminated\n");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[1024];
	int kb=4;
	int ks,nonce2;
	char temp[10],ida,idb;
	idb='B';

	/*printf("Enter the private key for B:");
	scanf("%d",&kb);

	FILE *fptr;
    fptr = (fopen("kb.txt", "w"));
    fprintf(fptr,"%d",kb);
    fclose(fptr);
	*/
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
	printf("Message from A\n");
	printf("Decrypting...\n");
	decrypt(kb,buffer);
	printf("Packet received from A:[%c,%c]\n",buffer[0],buffer[1]);
	printf("Session key:%c\nNetwork id of A:%c\n",buffer[0],buffer[1]);
	temp[0]=buffer[0];
	ks=atoi(temp);
	ida=buffer[1];
	
	printf("Enter nonce2:");
	scanf("%d",&nonce2); 
	
	printf("Encrypting...\n");
	bzero(temp,10);
	sprintf(temp,"%d",nonce2);
	buffer[0]=temp[0];
	encrypt(ks,buffer);
	n=write(newsockfd,buffer,strlen(buffer));
	if(n<0)
		error("error on transmitting");
	
	bzero(buffer,1024);
	n=read(newsockfd,buffer,1024);
	if(n<0)
		error("error on reading");
	printf("Message from A\n");
	printf("Decrypting...\n");
	decrypt(ks,buffer);
	printf("Packet received from A:[%c]\n",buffer[0]);
	printf("Nonce2 + 1:%c\n",buffer[0]); 
	
	//if((nonce2+1)==(atoi(buffer)))
		printf("Connection established\n");
//	else
	//{	close(newsockfd);
		//close(sockfd);
		//return 0;
//	}
	
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
	return 0;
}

