#include <iostream>
#include <tuple>
#include <array>
#include <string>
#include <random>


const std::tuple<int, int> up{
    std::make_tuple(-1, 0)
};

const std::tuple<int, int> down{
    std::make_tuple(1, 0)
};

const std::tuple<int, int> left{
    std::make_tuple(0, -1)
};

const std::tuple<int, int> right{
    std::make_tuple(0, 1)
};

class Snake
{
    std::vector<std::tuple<int, int>> body;
    std::tuple<int, int> tail;
    std::tuple<int, int> currentDirection;
    bool eatsTail;

    public:
        Snake(std::vector<std::tuple<int, int>>& initialBody, std::tuple<int, int> initialDirection)
            : body(initialBody),
              currentDirection(initialDirection)
        {
        }

    std::tuple<int, int> getHead() {
        return body[0];
    }

    std::tuple<int, int> getTail() {
        return body.back();
    }

    const std::vector<std::tuple<int, int>>& getBody() const {
        return body;
    }

    void move() {
        std::tuple<int, int> head = getHead();
        std::tuple<int, int> oldHead = getHead();
        std::get<0>(head) += std::get<0>(currentDirection);
        std::get<1>(head) += std::get<1>(currentDirection);
        body[0] = head;
        tail = body.back();

        std::tuple<int, int> moveToPos = oldHead;
        for (int i = 1; i < body.size(); i++) {
            std::tuple<int, int> oldPos = body[i];
            body[i] = moveToPos;
            moveToPos = oldPos;

            if (head == body[i]) {
                eatsTail = true;
            }
        }
    }

    void setDirection(std::tuple<int, int> newDirection) {
        if (std::get<0>(newDirection) + std::get<0>(currentDirection) == 0
            && std::get<1>(newDirection) + std::get<1>(currentDirection) == 0) {
            currentDirection = currentDirection;
        }
        else
        {
            currentDirection = newDirection;
        }
    }

    void addToBody() {
        body.push_back(tail);
        tail = body.back();
    }

    bool isEatingTail() {
        return eatsTail;
    }
};

class Apple
{
    std::tuple<int, int> currentPosition;

    public:
        Apple(std::tuple<int, int>& initialPosition)
            : currentPosition(initialPosition)
        {
        }

        std::tuple<int, int> getNewPosition(int height, int width) {
            int randomHeight = getRandomNumber(1, height - 2);
            int randomWidth = getRandomNumber(1, width - 2);

            return std::make_tuple(randomHeight, randomWidth);
        }

        std::tuple<int, int> getCurrentPosition() {
            return currentPosition;
        }

        void setPosition(std::tuple<int, int> newPosition) {
            currentPosition = newPosition;
        }

        int getRandomNumber(int minNumber, int maxNumber) {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<int> distribution(minNumber, maxNumber);

            return distribution(gen);
        }

        void move(int height, int width) {
            std::tuple<int, int> newPos = getNewPosition(height, width);
            setPosition(newPos);
        }
};

class Game
{
    private:
        Snake snake;
        Apple apple;
        int height;
        int width;
        std::vector<std::tuple<int, int>>& initialSnakeBody;
        const std::vector<std::tuple<int, int>>& currentSnakeBody;
        std::tuple<int, int> currentDirection;
        std::tuple<int, int>& initialApplePos;
        std::tuple<int, int> currentApplePos;
        bool gameOver;

    public:

        Game(int x, int y, std::vector<std::tuple<int, int>>& initialSnakeBody, std::tuple<int, int>& initialSnakeDirection, std::tuple<int, int>& initialApplePos)
            : snake(initialSnakeBody, initialSnakeDirection),
              height(x),
              width(y),
              initialSnakeBody(initialSnakeBody),
              currentSnakeBody(snake.getBody()),
              initialApplePos(initialApplePos),
              currentApplePos(apple.getCurrentPosition()),
              apple(initialApplePos),
              gameOver(false)
        {
        }

    char** createMatrix() {
        int rows = height;
        int columns = width;

        char** vector = new char* [rows];

        for (int i = 0; i < rows; i++) {
            vector[i] = new char[columns];
        }

        return vector;
    }

    void freeMatrix(char** matrix) {
        for (int i = 0; i < height; i++) {
            delete[] matrix[i];
        }

        delete[] matrix;
    }

    void populateMatrix(char** matrix) {
        populateBorder(matrix);

        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                matrix[i][j] = ' ';
            }
        }
    }

    void populateBorder(char** matrix) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i == 0 || i == height - 1) {
                    if (j == 0 || j == width - 1) {
                        matrix[i][j] = '+';
                    }
                    else
                    {
                        matrix[i][j] = '-';
                    }
                }
                else if (j == 0 || j == width - 1) {
                    matrix[i][j] = '|';
                }
            }
        }
    }

    std::vector<std::tuple<int, int>> getBorder() {
        std::vector<std::tuple<int, int>> border; 

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i == 0 || i == height - 1) {
                    border.push_back(std::make_tuple(i, j));
                }
                if (j == 0 || j == width - 1) {
                    border.push_back(std::make_tuple(i, j));
                }
            }
        }

        return border;
    }

    void printMatrix(char** matrix) {

        std::tuple<int, int> snakeHead = currentSnakeBody[0];
        std::vector<std::tuple<int, int>> snakeBody = currentSnakeBody;
        std::tuple<int, int> applePos = apple.getCurrentPosition();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                bool isSnakeBody = false;
                bool isSnakeHead = false;
                bool isApple = false;

                for (const auto& tuple : snakeBody) {
                    if (i == std::get<0>(tuple) && j == std::get<1>(tuple)) {
                        if (i == std::get<0>(snakeHead) && j == std::get<1>(snakeHead)) {
                            isSnakeHead = true;
                        }
                        else
                        {
                            isSnakeBody = true;
                        }
                        break;
                    }
                }

                if (i == std::get<0>(applePos) && j == std::get<1>(applePos)) {
                    isApple = true;
                }

                std::cout << (matrix[i][j] = isSnakeHead ? 'X' : isSnakeBody ? '0' : isApple ? '*' : matrix[i][j]);
            }
            std::cout << std::endl;
        }
    }

    void render() {
        char** board = createMatrix();
        populateMatrix(board);
        printMatrix(board);
        freeMatrix(board);
    }

    void moveSnake(std::string input) {
        
        if (input.length() == 1) {
            char inputChar = input[0];

            switch (inputChar) {
            case 'w':
                snake.setDirection(up);
                break;
            case 'a':
                snake.setDirection(left);
                break;
            case 's':
                snake.setDirection(down);
                break;
            case 'd':
                snake.setDirection(right);
                break;
            }
        }

        snake.move();

        if (isGameOver()) {
            gameOver = true;
        }

        if (apple.getCurrentPosition() == snake.getHead()) {
            apple.move(height, width);
            currentApplePos = apple.getCurrentPosition();
            snake.addToBody();
        }
    }

    bool isGameOver() {
        return snake.isEatingTail() == true || hasCrashedWithBorder() == true;
    }

    bool hasCrashedWithBorder() {
        for (const auto& tuple : getBorder()) {
            if (snake.getHead() == tuple) {
                return true;
            }
        }
    }
};

int main()
{
    std::vector<std::tuple<int, int>> initialSnakeBody = { std::make_tuple(2, 4), std::make_tuple(2, 3), std::make_tuple(2, 2) };
    std::tuple<int, int> initialSnakeDirection = right;
    std::tuple<int, int> initialApplePos = std::make_tuple(5, 5);

    Game game(10, 20, initialSnakeBody, initialSnakeDirection, initialApplePos);
    game.render();

    std::string input;
    while (true) {
        std::getline(std::cin, input);

        if (input.empty()) {
            input = 'e';
        }
        else if (input[0] == 'q') {
            break;
        }

        game.moveSnake(input);
        game.render();

        if (game.isGameOver() == true) {

            std::cout << "Game over";
            break;
        }
    }
}