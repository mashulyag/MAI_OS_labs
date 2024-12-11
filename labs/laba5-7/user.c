#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <zmq.h>

int id;

struct Timer {
    clock_t total_time;
    time_t cur_time;
    int running;
};

struct Timer myTimer;

void startTimer(struct Timer* timer) {
    if (!timer->running) {
        timer->cur_time = time(NULL);
        timer->running = 1;
        printf("Ok:%d\n", id);
        return;
    }
}

void stopTimer(struct Timer* timer) {
    if (timer->running) {
        timer->total_time += time(NULL) - timer->cur_time;
        timer->running = 0;
        printf("Ok:%d\n", id);
        return;
    }
}

void getTotalSumTimer(struct Timer* timer) {
    if (timer->running) {
        printf("Ok:%d: %ld\n", id, (timer->total_time + (time(NULL) - timer->cur_time)) * 1000);
    }
    else {
        printf("Ok:%d: %ld\n", id, timer->total_time * 1000);
    }
    return; 
}

int main(int argc, char **argv) {
    sscanf(argv[1], "%d", &id);
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://127.0.0.1:5555");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

    char buffer[256];
    char command[256];
    char subcommand[256];
    int arg1 = 0, arg2 = 0, k = 0, c = 0, val = 0;
    pid_t id1;
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(command, 0, sizeof(command));

        zmq_recv(subscriber, buffer, sizeof(buffer), 0); // распаковываем сообщение в buffer
        sscanf(buffer, "%s", command);

        if (strcmp(command, "create") == 0){
            sscanf(buffer, "%*s %d %d", &arg1, &arg2);
            if (id == arg2){
                printf("Node %d: create child\n", id);
                pid_t id_child = fork();
                if (id_child == 0){
                    char str1[sizeof(int)];
                    sprintf(str1, "%d", arg1);
                    execl("./user", "./user", str1, NULL); 
                    perror("execl");
                }
                printf("Ok: %d\n", id_child);
            }
        }

        else if (strcmp(command, "exec") == 0) {
            sscanf(buffer, "%*s %d %s", &arg1, subcommand);
            time_t t = time(NULL);
            if (id == arg1) {
                if (strcmp(subcommand, "time") == 0) {
                    getTotalSumTimer(&myTimer);
                }
                else if (strcmp(subcommand, "start") == 0) {
                    startTimer(&myTimer);
                }
                else if (strcmp(subcommand, "stop") == 0) {
                    stopTimer(&myTimer);
                }
            }
            
        }
        else if (strcmp(command, "kill") == 0) {
            sscanf(buffer, "%*s %d", &arg1);
            if (id == arg1){
                printf("Node %d: has been killed\n", id);
                break;
            }  
        }
    }
    zmq_close(subscriber);
    zmq_ctx_destroy(context);
}
