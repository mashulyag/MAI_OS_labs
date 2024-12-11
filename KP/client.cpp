#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Ошибка при создании pipe." << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Ошибка при создании дочернего процесса." << std::endl;
        return 1;
    }

    if (pid == 0) {
        // Дочерний процесс (server.cpp)
        close(pipefd[0]);  // Закрываем конец pipe для чтения

        // Перенаправляем стандартный вывод в pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Запускаем server.cpp
        execlp("./server", "server", nullptr);

        // Если execlp вернул управление, значит произошла ошибка
        std::cerr << "Ошибка при запуске server.cpp." << std::endl;
        return 1;
    } else {
        // Родительский процесс (client.cpp)
        close(pipefd[1]);  // Закрываем конец pipe для записи

        std::string command;
        while (true) {
            std::cout << "> ";
            std::cin >> command;

            // Отправляем команду в pipe
            write(pipefd[0], command.c_str(), command.length());

            // Читаем ответ из pipe
            const int BUFFER_SIZE = 256;
            char buffer[BUFFER_SIZE];
            int bytesRead = read(pipefd[0], buffer, BUFFER_SIZE - 1);
            if (bytesRead == -1) {
                std::cerr << "Ошибка при чтении из pipe." << std::endl;
                break;
            } else if (bytesRead == 0) {
                std::cout << "Сервер завершил работу." << std::endl;
                break;
            } else {
                buffer[bytesRead] = '\0';
                std::cout << buffer << std::endl;
            }
        }

        // Ждем завершения дочернего процесса
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
