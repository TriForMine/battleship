#include "main.h"
#include "multiplayer/socket.h"

int main(void) {
    char message[4096];
    char message2[4096];
    int sockfd = socket_create(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1234);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (socket_connect(sockfd, &serv_addr) < 0) {
        perror("Error connecting to server");
        socket_close(sockfd);
        exit(1);
    }

    socket_recv(sockfd, message, 4096, 0);
    printf("Message from server: %s\n", message);

    printf("Enter message to send to server: ");
    fgets(message, 4096, stdin);
    socket_send(sockfd, message, strlen(message), 0);

    socket_recv(sockfd, message2, 4096, 0);
    printf("Message from server: %s\n", message2);

    /*
    int i;
    bool help = false;
    bool interactive = false;
    char* filename = NULL;
    long benchmark = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help = true;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
            interactive = true;
        } else if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && i + 1 < argc) {
            filename = argv[i + 1];
        } else if ((strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--benchmark") == 0) && i + 1 < argc) {
            benchmark = strtol(argv[i + 1], NULL, 10);
        }
    }

    if (help) {
        printf("Usage: battleship [-h] [-i] [file]");
    } else if (interactive) {
        handleInteractiveGame();
    } else if (filename != NULL) {
        parseFile(filename);
    } else if (benchmark > 0) {
        startBenchmark(benchmark);
    } else {
        parseSTDIN();
    }*/

    return 0;
}