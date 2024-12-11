#include "tree.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <zmq.h>

char message[256]; // буфер

void killWithChildren(TNode *node, void * publisher) { // удаляем процессы
    if (node == NULL) {
        return;
    }
    // Отключаем текущий узел
    sprintf(message, "kill %d", node->data); 
    zmq_send(publisher, message, strlen(message), 0);
    memset(message, 0, sizeof(message));

    // Рекурсивно отключаем всех потомков
    killWithChildren(node->firstChild, publisher);
    killWithChildren(node->nextBrother, publisher);
}

int main() {
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://127.0.0.1:5555");
    
    char input[256];
    char command[100];
    char subcommand[100];
    int arg1 = 0, arg2 = 0;
    TNode *root = createNode(-1); // управляющий узел

    while (1) {
        memset(message, 0, sizeof(message));
        memset(command, 0, sizeof(command));

        if(fgets(input, sizeof(input), stdin) == NULL) { 
            break;
        }

        sscanf(input, "%s", command);
          if (strcmp(command, "create") == 0){ 
            sscanf(input, "%*s %d %d", &arg1, &arg2);
            TNode *node = find_node(root, arg2);
            TNode *node2 = find_node(root, arg1); 
            if (node == NULL){
                printf("Error: Parent not found \n");
            }
            else if(node2 != NULL){
                printf("Error: Already exist \n");
            }
            else{
                addChild(node, arg1); 
                if (arg2 == -1){ // если создаем в управляющем узле
                    pid_t id = fork();
                    if (id == 0){
                        char str1[sizeof(int)];
                        sprintf(str1, "%d", arg1);
                        execl("./user", "./user", str1, NULL); 
                        perror("execl");
                    }
                    printf("Ok: %d\n", id);

                }
                else{
                    sprintf(message, "create %d %d", arg1, arg2);
                    zmq_send(publisher, message, strlen(message), 0);
                }
            }
        }
        
        else if (strcmp(command, "exec") == 0) {
            sscanf(input, "%*s %d %s", &arg1, subcommand);
            TNode *searching = find_node(root, arg1);
            if (arg1 == -1) {
                printf("This is a command process\n");
            }
            else if (searching == NULL) {
                printf("Error:id Not found\n");
            }
            else if (searching->exist) {
                zmq_send(publisher, input, strlen(input), 0);
            }
            else {
                printf("You can't give an option to dead r3oot\n");
                continue;
            }
        }

        else if (strcmp(command, "kill") == 0) {
            sscanf(input, "%*s %d", &arg1);
            TNode *node4 = find_node(root, arg1);
            if (arg1 == -1)
                printf("Nope\n");
            else if(node4 == NULL){
                printf("This node is dead or doesn`t exist ever\n");
            }
            else{
                zmq_send(publisher, input, strlen(input), 0);
                killWithChildren(node4->firstChild, publisher);
                disableNode(node4->firstChild);
                disableOneNode(node4);
            }
        }
        else if (strcmp(command, "heartbit") == 0) {
            sscanf (input, "%*s %d", &arg1);
            int time = arg1 / 1000;
            Heartbit(root, time);
            printf("\n");
        }

        else if (strcmp(command, "ping") == 0) {
            sscanf(input, "%*s %d", &arg1);
            TNode* search_available_root = find_node(root, arg1);
            if (arg1 == -1) {
                printf("Control process is always available\n");
                printf("\n");
            }
            if (search_available_root == NULL) {
                printf("Error: id not found\n");
                continue;
            }
            if (search_available_root->exist) {
                printf("Ok: 1 // узел %d доступен\n", arg1);
                continue;
            }
            else {
                printf("Ok: 0 // узел %d недоступен\n", arg1);
                continue;
            }
        }
        
    }
    zmq_close(publisher);
    zmq_ctx_destroy(context);
}
