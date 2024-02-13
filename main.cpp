#include <iostream>
#include <random>
#include <unistd.h> // for sleep
#include <ncurses.h> //for screen
using namespace std;

bool gameOver;
const int width = 50;
const int height = 20;
int score = 0;
int tailX[100], tailY[100];
int tail;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> disX(1, width - 3);
uniform_int_distribution<> disY(1, height - 3);

enum enumDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enumDirection direction;

int x, y, foodX, foodY;

void Create() {
    gameOver = false;
    direction = STOP;
    x = width / 2;
    y = height / 2;

    foodX = disX(gen);
    foodY = disY(gen);
    score = 0;
    tail = 0;
}

void Draw() {
    clear();

    for (int i = 0; i < width + 2; i++) {
        mvprintw(0, i, "#");
        mvprintw(height + 1, i, "#");
    }
    for (int j = 1; j < height + 1; j++) {
        mvprintw(j, 0, "#");
        mvprintw(j, width + 1, "#");
    }

    mvprintw(y, x, "O");

    mvprintw(foodY, foodX, "F");

    for (int k = 0; k < tail; k++) {
        if(tailY[k]==y && tailX[k]==x){
            gameOver = true;
        }
        mvprintw(tailY[k], tailX[k], "o");
    }

    mvprintw(height + 3, 0, "Score: %d", score);
    refresh();
}

void Input() {
    int c = getch();
    int lastC = c;
    switch (c) {
        case KEY_UP:
            direction = UP;
            lastC = c;
            break;
        case KEY_DOWN:
            direction = DOWN;
            break;
        case KEY_LEFT:
            direction = LEFT;
            break;
        case KEY_RIGHT:
            direction = RIGHT;
            break;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < tail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction) {
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        default:
            break;
    }

    if (x > width || x < 1 || y > height || y < 1) {
        gameOver = true;
    }

    if (x == foodX && y == foodY) {
        score += 10;
        foodX = disX(gen);
        foodY = disY(gen);
        tail++;
    }
}

int main() {
    initscr(); // Initialize ncurses
    noecho(); // Don't echo characters to the screen
    curs_set(0); // Hide cursor
    keypad(stdscr, TRUE); // Enable keypad mode

    Create();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(100000); // Sleep for 100 milliseconds
    }

    endwin(); // Terminate ncurses
    return 0;
}
