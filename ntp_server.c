#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#define PORT     123
#define MAXLINE 1024
   
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char hello[MAXLINE];
	char *replyHello = "Hello client, please enter the password first";
	char *wrongPass = "Wrong Password!";
	char *wrongTime = "Wrong Timezone!";
    struct sockaddr_in servaddr, cliaddr;
	char password[100]= "NTPpa55";
	char *token; 
	
	
	char h[20];
	char m[20];
	
	int hours, minutes, seconds, day, month, year;
	
	
	char timezone[27][40] = {
                         "UTC-12:00","UTC-11:00","UTC-10:00","UTC-09:00","UTC-08:00","UTC-07:00","UTC-06:00","UTC-05:00","UTC-04:00","UTC-03:00","UTC-02:00","UTC-01:00"
						 ,"UTC+00:00","UTC+01:00","UTC+02:00","UTC+03:00","UTC+04:00","UTC+05:00","UTC+06:00","UTC+07:00","UTC+08:00","UTC+09:00","UTC+10:00","UTC+11:00","UTC+12:00","UTC+13:00","UTC+14:00"
                     };

       
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
       
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	
	printf("\nBind Successfully\n");
       
	   
    int len, n;
   
    len = sizeof(cliaddr);  
	
	
	while(1)
	{	
		// recieve first hello from client
		
						
				    //receive password and timzone from client
					n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
								MSG_WAITALL, ( struct sockaddr *) &cliaddr,
								&len);
					buffer[n] = '\0';
					
					//check password and timezone
					int i = 0;
					token = strtok(buffer, ";");
					while(i < 2)
					{
						
						if(i == 0)
						{
							if(strcmp(token,password) != 0)
							{
								sendto(sockfd, (const char *)wrongPass, strlen(wrongPass), 
								MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
								len);
								//sent warning to client
								char* ipString = inet_ntoa(cliaddr.sin_addr);
								printf("\nWrong password from client-%s\n", ipString);
								break;
							}
							
							else
							{		
								token = strtok(NULL, ";");			
							}
								
						}

						else
						{
							
							for(int j = 0; j < 27; j++)
							{
																		
								if(strcmp(timezone[j],token) == 0)
								{
									
									
										memcpy(h,&token[4],2);
										memcpy(m,&token[7],2);
										
										
										int a = atoi(h); 
										int b = atoi(m);
										
										time_t now;							
										time(&now);
										struct tm *local = localtime(&now);
										
									
										if(token[3]=='+')
										{
											
											if(local->tm_hour + a >= 24)
												{
													hours = local->tm_hour + a - 24;
													day = local->tm_mday + 1;
												}
												
											else
												{
													hours = local->tm_hour + a;
													day = local->tm_mday;  
												}
												
											
											
										
											
											minutes = local->tm_min + b;       
											seconds = local->tm_sec;        // get seconds passed after a minute (0-59)	
											month = local->tm_mon + 1;      // get month of year (0 to 11)
											year = local->tm_year + 1900;   // get year since 1900
											
											sprintf(hello,"Time is %02d:%02d:%02d, Date is: %02d/%02d/%d\n", hours, minutes, seconds, day, month, year);
											
											sendto(sockfd, (const char *)hello, strlen(hello), 
											MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
													len);
													//sent time to client
											char* ipString = inet_ntoa(cliaddr.sin_addr);
											printf("\nCurrent time has been sent to the client-%s.\n", ipString); 
											break;
										}
										
										else
										{
										
											if(local->tm_hour - a < 0)
											{
												hours = local->tm_hour - a + 24;
												day = local->tm_mday - 1;
											}
											
											else
											{
												hours = local->tm_hour - a;
												day = local->tm_mday;  
											}
											
											
					
											
											minutes = local->tm_min + b;       
											seconds = local->tm_sec;        // get seconds passed after a minute (0-59)
											month = local->tm_mon + 1;      // get month of year (0 to 11)
											year = local->tm_year + 1900;   // get year since 1900
											
											sprintf(hello,"Time is %02d:%02d:%02d, Date is: %02d/%02d/%d\n", hours, minutes, seconds, day, month, year);
											
										 
											
											sendto(sockfd, (const char *)hello, strlen(hello), 
												MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
													len);
													//sent time to client
											char* ipString = inet_ntoa(cliaddr.sin_addr);
											printf("\nCurrent time has been sent to the client-%s.\n", ipString); 
											break;
										}	
								}
								
								else if(j == 26)
								{
									if(strcmp(timezone[27],token) != 0)
									{
										sendto(sockfd, (const char *)wrongTime, strlen(wrongPass), 
										MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
										len);
										//sent warning to client
										char* ipString = inet_ntoa(cliaddr.sin_addr);
										printf("\nWrong timezone from client-%s\n", ipString);
								
						
									}
								}
							
							}
							
						}
							
						i++;
					}	
	}	
				 
    return 0;
}
