// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 80

int nobody_UID = 65534;

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";

    if(argc == 1)
    {
      printf("PID=%d\n", getpid());
      printf("execve=0x%p\n", execve);

      // Creating socket file descriptor
      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
      {
        perror("socket failed");
        exit(EXIT_FAILURE);
      }

      // Attaching socket to port 80
    
      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)))
      {
        perror("setsockopt");
        exit(EXIT_FAILURE);
      }

      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY;
      address.sin_port = htons( PORT );

      // Forcefully attaching socket to the port 80
      if(bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
      {
        perror("bind failed");
        exit(EXIT_FAILURE);
      }
      if (listen(server_fd, 3) < 0)
      {
        perror("listen");
        exit(EXIT_FAILURE);
      }

      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      /*********************************************************************/
    
      pid_t child_process = fork();  

      /***********************  Rexec child  *******************************/
    
      if(child_process == 0)
      {
        char server_fd_string[10];
        sprintf(server_fd_string, "%d", new_socket);
        char* exec_args[] = {(char *)argv[0], server_fd_string, NULL};

        if(execvp(exec_args[0], exec_args) < 0)
        {
          perror("execvp"); 
          exit(EXIT_FAILURE); 
        }
      }

      else
      {
        /*make the parent wait until child exits. Else, the child will be 
          adopted by init process */
         int child_status = 0;
         while ((wait(&child_status) > 0));
      }
    } /*end of  if(argc == 1)*/

    else
    {
    /************************  Privelege Seperation  **********************/
        printf("Child PID = %d and Parent PID =%d\n",getpid(),getppid()); 

        if(setuid(nobody_UID)==-1){ /* Setting UID to nobody user's UID  */
            perror("Cannot set UID for child process");
            exit(EXIT_FAILURE);
        }
        printf("child_process UID=%d\n", getuid());
        sscanf(argv[1], "%d", &new_socket);
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("\nHello message sent\n");
        return 0;
    }
    /*********************************************************************/

}
