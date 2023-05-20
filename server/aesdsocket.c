#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PORT 9000
#define BUFFER_SIZE 1024
#define PATH "/tmp/aesdsocketdata"

int server_socket;
int client_socket;

void signalHandler(int signal) {
    shutdown(server_socket, SHUT_RDWR);
    shutdown(client_socket, SHUT_RDWR);
    close(server_socket);
    close(client_socket);
    remove(PATH);
    exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        exit(1);
    }

    int reuseaddr = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        exit(1);
    }

    if (argc == 2 && !strcmp("-d", argv[1])) {
        daemon(0, 0);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            exit(1);
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

        int fd = open(PATH, O_RDWR | O_APPEND | O_CREAT, 0644);
        if (fd < 0) {
            break;
        }

        char buffer[BUFFER_SIZE];
        int bytes_read;
        while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {

            buffer[bytes_read] = 0;
            write(fd, buffer, bytes_read);

            if (buffer[bytes_read - 1] == '\n') {
                break;
            }
        }

        lseek(fd, 0, SEEK_SET);
        while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
        }

        close(fd);
        close(client_socket);
    }
    return 0;
}
