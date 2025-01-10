#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[WIDTH * HEIGHT];
    int length;
    int direction;
} Snake;

typedef struct {
    Point pos;
} Food;

void init_game(Snake *snake, Food *food);
void draw_borders();
void draw_snake(Snake *snake);
void draw_food(Food *food);
void update_snake(Snake *snake);
int check_collision(Snake *snake);
void spawn_food(Food *food, Snake *snake);

enum { UP = 0, DOWN, LEFT, RIGHT };

int main() {
    Snake snake;
    Food food;
    int key;

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    init_game(&snake, &food);

    while(1) {
        clear();
        draw_borders();
        draw_snake(&snake);
        draw_food(&food);
        refresh();

        if ((key = getch()) != ERR) {
            switch (key) {
                case KEY_UP:
                    if (snake.direction != DOWN) snake.direction = UP;
                    break;
                case KEY_DOWN:
                    if (snake.direction != UP) snake.direction = DOWN;
                    break;
                case KEY_LEFT:
                    if (snake.direction != RIGHT) snake.direction = LEFT;
                    break;
                case KEY_RIGHT:
                    if (snake.direction != LEFT) snake.direction = RIGHT;
                    break;
                case 'q':
                case 'Q':
                    endwin();
                    return 0;
            }
        }

        update_snake(&snake);
        if (snake.body[0].x == food.pos.x && snake.body[0].y == food.pos.y) {
            snake.length++;
            spawn_food(&food, &snake);
        }

        if (check_collision(&snake)) {
            mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER!");
            refresh();
            napms(2000);
            break;
        }

        napms(100);
    }

    endwin();
    return 0;
}

void init_game(Snake *snake, Food *food) {
    snake->length = 3;
    snake->body[0].x = WIDTH / 2;
    snake->body[0].y = HEIGHT / 2;
    snake->direction = RIGHT;

    for (int i = 1; i < snake->length; i++) {
        snake->body[i].x = snake->body[0].x - i;
        snake->body[i].y = snake->body[0].y;
    }

    spawn_food(food, snake);
}

void draw_borders() {
    for (int i = 0; i < WIDTH; i++) {
        mvprintw(0, i, "#");
        mvprintw(HEIGHT - 1, i, "#");
    }
    for (int i = 0; i < HEIGHT; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, WIDTH - 1, "#");
    }
}

void draw_snake(Snake *snake) {
    for (int i = 0; i < snake->length; i++)
        mvprintw(snake->body[i].y, snake->body[i].x, "O");
    
}

void draw_food(Food *food) {
    mvprintw(food->pos.y, food->pos.x, "@");
}

void update_snake(Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--)
        snake->body[i] = snake->body[i - 1];

    switch (snake->direction) {
        case UP:
            snake->body[0].y--;
            break;
        case DOWN:
            snake->body[0].y++;
            break;
        case LEFT:
            snake->body[0].x--;
            break;
        case RIGHT:
            snake->body[0].x++;
            break;
    }
}

int check_collision(Snake *snake) {
    if (snake->body[0].x == 0 || snake->body[0].x == WIDTH - 1 || snake->body[0].y == 0 || snake->body[0].y == HEIGHT - 1) return 1;

    for (int i = 1; i < snake->length; i++) 
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) return 1;

    return 0;
}

void spawn_food(Food *food, Snake *snake) {
    int valid = 0;
    while (!valid) {
        food->pos.x = rand() % (WIDTH - 2) + 1;
        food->pos.y = rand() % (HEIGHT - 2) + 1;

        valid = 1;
        for (int i = 0; i < snake->length; i++) {
            if (food->pos.x == snake->body[i].x && food->pos.y == snake->body[i].y) {
                valid = 0;
                break;
            }
        }
    }
}