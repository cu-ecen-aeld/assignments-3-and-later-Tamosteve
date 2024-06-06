#include <signal.h>
#include <sys/stat.h>
#include <stdio.h> // IWYU pragma: keep.
#include <stdlib.h> // IWYU pragma: keep.
#include <string.h> // IWYU pragma: keep.
#include <sys/syslog.h>
#include <unistd.h>
#include <errno.h> // IWYU pragma: keep.
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h> // IWYU pragma: keep.
#include <fcntl.h>

#define PORT 9000
#define BUFFER_SIZE 256000
#define FILE_PATH "/var/tmp/aesdsocketdata"

char buffer[BUFFER_SIZE];
int server_socket = -1;
int client_socket = -1;
pid_t pid;
int file_fd = -1;
int opt = 1;

void handle_signal(int signal)
{
    if(signal == SIGINT || signal == SIGTERM)
    {
        syslog(LOG_INFO, "Caught signal, exiting");
        shutdown(server_socket, SHUT_RDWR);
        if (server_socket != -1) close(server_socket);
        if (file_fd != -1) close(file_fd);
        shutdown(client_socket, SHUT_RDWR);
        if (client_socket != -1) close(client_socket);
        remove(FILE_PATH);
        closelog();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{

    
    struct sockaddr_in server_adrr, client_adrr;
    socklen_t client_adrrlen = sizeof(client_adrr);
    ssize_t data; 
    ssize_t nr;

    openlog("aesdsocket", LOG_PID, LOG_USER);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // create the socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( server_socket < 0)
    {
        syslog(LOG_ERR, "socket failed: %s", strerror(errno));
        return -1;
    }

    // bind socket to port 9000
    if (setsockopt(server_socket, SOL_SOCKET,
                SO_REUSEADDR | SO_REUSEPORT, &opt,
                sizeof(opt))) 
    {
        syslog(LOG_ERR, "setsockopt: %s", strerror(errno));
        return -1;
    }

    server_adrr.sin_family = AF_INET;
    server_adrr.sin_addr.s_addr = INADDR_ANY;
    server_adrr.sin_port = htons(PORT);
    memset(&(server_adrr.sin_zero), '\0', 8);

    if (bind(server_socket, (const struct sockaddr *)&server_adrr, sizeof(struct sockaddr)) < 0)
    {
        syslog(LOG_ERR, "bind failed: %s", strerror(errno));
        return -1;
    }

    // listen the connection
    if (listen(server_socket, 5) < 0)
    {
        syslog(LOG_ERR, "listen failed: %s", strerror(errno));
        return -1;
    }

    if(argc > 1 && strcmp(argv[1], "-d") == 0) 
    {
        pid = fork();
        if(pid > 0) 
        {   
            printf("aesdsocket running as daemon %d\n", pid);
            syslog(LOG_INFO, "aesdsocket running as daemon %d\n", pid);
            exit(EXIT_SUCCESS);  // Ensure the parent exits after forking the daemon
        } 
        else if(pid == 0) 
        {
            while(1)
            {
                // accept connection
                client_socket = accept(server_socket, (struct sockaddr*)&client_adrr, &client_adrrlen);
                if (client_socket < 0)
                {
                    syslog(LOG_ERR, "accept failed: %s", strerror(errno));
                    continue;
                }

                char s[INET_ADDRSTRLEN];
                inet_ntop(client_adrr.sin_family, &(client_adrr.sin_addr), s, sizeof(s));
                syslog(LOG_INFO, "Accepted connection from %s", s);

                file_fd = open(FILE_PATH, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IROTH );
                if (file_fd < 0)
                {
                    syslog(LOG_ERR, "failed to open file: %s", strerror(errno));
                    close(client_socket);
                    continue;
                    
                }

                while ((data = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) 
                {
                    if (write(file_fd, buffer, data) < 0) {
                        syslog(LOG_ERR, "write failed: %s", strerror(errno));
                        break;
                    }
                    if (buffer[data - 1] == '\n') {
                        lseek(file_fd, 0, SEEK_SET);
                        while ((nr = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
                            if (send(client_socket, buffer, nr, 0) < 0) {
                                syslog(LOG_ERR, "send failed: %s", strerror(errno));
                                break;
                            }
                        }
                        break;
                    }
                }

                close(file_fd);
                shutdown(client_socket, SHUT_RDWR);
                close(client_socket);
                syslog(LOG_INFO, "Closed connection from: %s", s);

            }

            closelog();
            shutdown(server_socket, SHUT_RDWR);
            close(server_socket);
            return 0;       

        } 
        else if (pid == -1) 
        {
            syslog(LOG_ERR, "fork failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else
    {    
        while(1)
        {
            // accept connection
            client_socket = accept(server_socket, (struct sockaddr*)&client_adrr, &client_adrrlen);
            if (client_socket < 0)
            {
                syslog(LOG_ERR, "accept failed: %s", strerror(errno));
                continue;
            }

            char s[INET_ADDRSTRLEN];
            inet_ntop(client_adrr.sin_family, &(client_adrr.sin_addr), s, sizeof(s));
            syslog(LOG_INFO, "Accepted connection from %s", s);

            file_fd = open(FILE_PATH, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IROTH );
            if (file_fd < 0)
            {
                syslog(LOG_ERR, "failed to open file: %s", strerror(errno));
                close(client_socket);
                continue;
                
            }

            while ((data = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) 
            {
                if (write(file_fd, buffer, data) < 0) {
                    syslog(LOG_ERR, "write failed: %s", strerror(errno));
                    break;
                }
                if (buffer[data - 1] == '\n') {
                    lseek(file_fd, 0, SEEK_SET);
                    while ((nr = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
                        if (send(client_socket, buffer, nr, 0) < 0) {
                            syslog(LOG_ERR, "send failed: %s", strerror(errno));
                            break;
                        }
                    }
                    break;
                }
            }

            
            close(file_fd);
            shutdown(client_socket, SHUT_RDWR);
            close(client_socket);
            syslog(LOG_INFO, "Closed connection from: %s", s);

        }

        closelog();
        shutdown(server_socket, SHUT_RDWR);
        close(server_socket);
        return 0;
    }

}