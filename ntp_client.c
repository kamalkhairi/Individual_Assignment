

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
   
#define PORT     123
#define MAXLINE 1024
   

int main() {
    int sockfd;
    char buffer[MAXLINE] = "Wrong Password!";
    char buffer2[MAXLINE] = "Wrong Timezone!"; 
    char password[100];
    struct sockaddr_in servaddr;

   
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("192.168.56.101");
       
    int n, len;
 
   
while(strcmp(buffer,"Wrong Password!") == 0 || strcmp(buffer,"Wrong Timezone!") == 0)
{	
// enter password and timezone
   printf("\n<password>;<timezone>:");
    scanf("%s", password);

   
    sendto(sockfd, (const char *)password, strlen(password),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,
            sizeof(servaddr));
    printf("\nPassword and timezone have been sent to server.");


//receive warning or time
 n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';
    printf("\nServer : %s", buffer);
}
	close(sockfd);
	
  
	




   
    
    return 0;
}
