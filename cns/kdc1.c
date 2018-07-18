



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/time.h>

#define TRUE 1
#define FALSE 0

void error(const char *msg)
{	perror(msg);
	exit(1);
}

int main(int argc,char *argv[])
{	if(argc<2)
	{	printf("Port no.not provided.Program terminated\n");
		exit(1);
	}

	int opt = TRUE;  
    	int addrlen ,client_socket[30] ,max_clients = 30 , activity, i , valread , sd;  
   	int max_sd;  

	int sockfd,newsockfd,portno,n;
	char buffer[1024];
	fd_set readfds;  
        
	 	
    	for (i = 0; i < max_clients; i++)  
    	{  
    	    client_socket[i] = 0;  
    	}  
        

	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen;
	 
	sockfd= socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{	error("Error opening socket.");
	}
	
	 if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )  
    	      error("setsockopt");  
    
	bzero((char *)&serv_addr,sizeof(serv_addr));
	portno=atoi(argv[1]);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(portno);
	
	if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
            error("Binding failed");
    	 listen(sockfd, 5);
	clilen= sizeof(cli_addr);  
	//clilen=sizeof(cli_addr);

 while(TRUE)  
    {  
         
        FD_ZERO(&readfds);  
    
        //add master socket to set 
        FD_SET(sockfd, &readfds);  
        max_sd = sockfd;  
            
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
    
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
            
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(sockfd, &readfds))  
        {  
            if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&clilen)<0))  
            {  
                error("accept");  
            }  
            
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s " , newsockfd,inet_ntoa(cli_addr.sin_addr)); 
	printf("port : %d \n",ntohs(cli_addr.sin_port));
	 for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = newsockfd;  
                    printf("Adding to list of sockets as %d\n" , i);  
                        
                    break;  
		}
            }  
         }    
          

	   for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
                
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&cli_addr,(socklen_t*)&clilen);  
                    printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(cli_addr.sin_addr) , ntohs(cli_addr.sin_port));  
                        
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }  
                    
                //Echo back the message that came in 
                else
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    printf("Client %d: %s\n",i,buffer); 
		    bzero(buffer,1024);
	            fgets(buffer,1024,stdin);
		    valread=write(sd,buffer,strlen(buffer));
			if(valread<0) 
                 		error("Error on writing");
		}

            }  
          
    	}
    }  
        
    return 0;  
}  






















    /*	if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&clilen)<0))
    	{
    	    error("accept");
    	}
	/*while(1)
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
}*/
