#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>

const int MAX_PLAYERS = 4; // Максимальное количество игроков в одной игре

struct Game {
    std::string name;
    int players;
};

std::vector<Game> games;

// Функция для проверки наличия числа в векторе
bool contains(const std::vector<int>& v, int num) {
    return std::find(v.begin(), v.end(), num) != v.end();
}

// Функция для генерации случайного числа
int generateRandomNumber() {
    std::vector<int> digits = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::random_shuffle(digits.begin(), digits.end()); //перемешивает случайным образом
    int number = 0;
    for (int i = 0; i < 4; ++i) {
        number = number * 10 + digits[i];
    }
    return number;
}

void createGame(const std::string& name, int players) {
    Game game;
    game.name = name;
    game.players = players;
    games.push_back(game);
    std::cout << "Игра \"" << name << "\" создана. Цель игры: угадать число. Игроков: " << players << std::endl;
}

void joinGame(std::string& name) {
    for (Game& game : games) {
        if (game.name == name) {
            if (game.players < MAX_PLAYERS) {
                game.players++;
                std::cout << "Игрок присоединился к игре \"" << name << "\". Игроков: " << game.players << std::endl;
            } else {
                std::cout << "Игра \"" << name << "\" уже полная." << std::endl;
            }
            return;
        }
    }
    std::cout << "Игра \"" << name << "\" не найдена." << std::endl;
}

void findGame() {
    for (const Game& game : games) {
        if (game.players < MAX_PLAYERS) {
            std::cout << "Найдена свободная игра: \"" << game.name << "\". Игроков: " << game.players << std::endl;
            return;
        }
    }
    std::cout << "Свободные игры не найдены." << std::endl;
}

void playGame(int guess){
    
        int number = generateRandomNumber();   
        
        int bulls = 0;
        int cows = 0;

        std::vector<int> digits(4); // Цифры загаданного числа
        for (int i = 3; i >= 0; --i) {
            digits[i] = number % 10;
            number /= 10;
        }

        std::vector<int> guessDigits(4); // Цифры предполагаемого числа
        for (int i = 3; i >= 0; --i) {
            guessDigits[i] = guess % 10;
            guess /= 10;
        }

        for (int i = 0; i < 4; ++i) {
            if (digits[i] == guessDigits[i]) {
                bulls++;
            } else if (contains(digits, guessDigits[i])) {
                cows++;
            }
        }

        std::cout << "Bulls: " << bulls << ", Cows: " << cows << std::endl;

        if (bulls == 4) {
            std::cout << "You won!" << std::endl;
        }
    
}


int main() {
    std::srand(std::time(nullptr)); // Инициализация генератора случайных чисел

    std::cout << "Сервер запущен." << std::endl;

    while (true) {
        std::string command;
        std::cout << "> ";
        std::cin >> command;

        if (command == "create") {
            std::string name;
            int players;
            std::cout << "Введите имя игры: ";
            std::cin >> name;
            std::cout << "Введите количество игроков: ";
            std::cin >> players;
            createGame(name, players);
        } else if (command == "join") {
            std::string name;
            std::cout << "Введите имя игры: ";
            std::cin >> name;
            joinGame(name);
        } else if (command == "find") {
            findGame();
        } else if (command == "play") {
            int number;
            std::cin >> number;
            playGame(number);
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Неверная команда." << std::endl;
        }
    }

    return 0;
}
