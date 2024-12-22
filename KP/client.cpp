#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    // Создаем сокет
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        return 1;
    }

    // Указываем адрес и порт сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Порт сервера
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Локальный адрес

    // Подключаемся к серверу
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Ошибка при подключении к серверу." << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Подключено к серверу." << std::endl;

    std::string command;
    while (true) {
        // Выводим приглашение для ввода команды
        std::cout << "> ";
        std::getline(std::cin, command);

        // Если команда "exit", завершаем работу
        if (command == "exit") {
            break;
        }

        // Отправляем команду серверу
        write(clientSocket, command.c_str(), command.length());

        // Читаем ответ от сервера
        char buffer[256];
        int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            std::cout << "Сервер завершил работу." << std::endl;
            break;
        }
        buffer[bytesRead] = '\0';
        std::cout << buffer; // Выводим ответ сервера
    }

    // Закрываем сокет
    close(clientSocket);
    return 0;
}
