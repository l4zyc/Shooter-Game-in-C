#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define width 30
#define height 30
#define max_bullets 300

char map[height][width];
bool gameOver = true;
bool bulletActive = false;
int bulletCount = 0;
int score = 0;

void k_input();
void clear();
void createPlayer();
void update();
void start();
void generateMap();
void createBullet();
void shootBullet();

typedef struct {
    char character;
    int x;
    int y;
    bool alive;
} Player;

typedef struct {
    char character;
    int x;
    int y;
    bool alive;
} Enemy;

typedef struct {
    int x;
    int y;
    bool active;
} Bullet;

Player player;
Bullet bullets[max_bullets];

Enemy enemies[15][30];

void clear() {
    system("clear");
}

int generateNumbers(int MIN, int MAX) {
    return rand() % (MAX - MIN + 1) + MIN;
}

void generateMap() {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            map[y][x] = '*';
        }
    }
}

void createPlayer() {
    player.x = 15;
    player.y = height / 1.2;
    player.alive = true;
    player.character = 'A';
}

void createEnemy() {
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 30; j++) {
            Enemy enemy;
            enemy.x = j;
            enemy.y = i;
            enemy.alive = true;
            enemy.character = generateNumbers(1,3) + '0';
            enemies[i][j] = enemy;
        }
    }
}

void createBullet() {
    for(int i = 0; i < max_bullets; i++) {
        if(!bullets[i].active) {
            bullets[i].x = player.x;
            bullets[i].y = player.y - 1;
            bullets[i].active = true;
            bulletActive = true;
            bulletCount++;
            break;
        }
    }
}

void shootBullet() {
    for(int i = 0; i < max_bullets; i++) {
        if(bullets[i].active) {
            bullets[i].y--;
            if(bullets[i].y <= 0) {
                bulletActive = false;
                bullets[i].active = false;;
            } else {
                for(int j = 0; j < 15; j++) {
                    for(int k = 0; k < 30; k++) {
                        if(enemies[j][k].alive && bullets[i].x == k && bullets[i].y == j) {
                            switch(enemies[j][k].character) {
                                case '1' :
                                    enemies[j][k].alive = false;
                                    score += 10;
                                    break;
                                case '2' :
                                    enemies[j][k].character = 1 + '0';
                                    break;
                                case '3' :
                                    enemies[j][k].character = 2 + '0';
                                    break;
                            }
                            bullets[i].active = false;
                            bulletActive = false;
                            break;
                        }
                    }
                }
            }
        }
    }
}


void draw() {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if(x == 0 || y == 0 || x == width - 1 || y == width - 1) {
                printf("%c", map[y][x]);
            } else if(player.alive && player.x == x && player.y == y) {
                printf("%c", player.character);
            }
            else {
                bool printed = false;

                for(int i = 0; i < 15; i++) {
                    for(int j = 0; j < 30; j++) {
                        if(enemies[i][j].alive && enemies[i][j].x == x && enemies[i][j].y == y) {
                            printf("%c", enemies[i][j].character);
                            printed = true;
                            break;
                        }
                    }
                }

                for(int i = 0; i < max_bullets; i++) {
                    if(bullets[i].active && bullets[i].y == y && bullets[i].x == x) {
                        printf("|");
                        printed = true;
                    }
                }

                if(!printed) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void start() {
    clear();
    createPlayer();
    createEnemy();
    generateMap();
    update();
}

void update() {
    while(gameOver) {
        clear();
        printf("Score: %d\n", score);
        printf("Count: %d\n", bulletCount);
        draw();
        k_input();
        shootBullet();

        if(player.x <= 0) {
            player.x++;
        } else if(player.x >= width - 1) {
            player.x--;
        }
    }
}

void k_input() {
    char control;
    printf(">> ");
    scanf("%[^\n]", &control); getchar();
    printf("\n");

    switch(control) {
        case 'a':
            player.x--;
            break;
        case 'd':
            player.x++;
            break;
        case 'x':
            exit(0);
            break;
        case 'e':
            if(!bulletActive && bulletCount < max_bullets) {
                createBullet();
            }
            break;
        default:
            return;
            break;
    }
}


int main() {
    start();

    return 0;
}