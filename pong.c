#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define STR 29
#define ROW 99

void frame(char field[STR][ROW]);                                       // Game field
int racket(char field[STR][ROW], int *left_racket, int *right_racket);  // Rackets physics
void ball(char field[STR][ROW], int left_racket, int right_racket, int *ball_i, int *ball_j, int *dir_x,
          int *dir_y);  // Ball physics
void goal(int *score, int *left_racket, int *right_racket, int *ball_i, int *ball_j, int *dir_x,
          int *dir_y);  // Update score, field to default
void draw_field(char field[STR][ROW]);

int main() {
    char field[STR][ROW];
    int left_racket = STR / 2, right_racket = STR / 2;  // Left and right rackets coordinates
    int ball_i = STR / 2, ball_j = ROW / 2;             // Ball coordinates
    int dir_y = 1, dir_x = 1;                           // Ball directions
    int left_score = 0, right_score = 0;
    char winner = 'n';
    char input = 'f';  // Start input value

    initscr();  // Init ncurses screen

    while (input != 'q') {
        nodelay(stdscr, true);

        frame(field);
        input = racket(field, &left_racket, &right_racket);
        ball(field, left_racket, right_racket, &ball_i, &ball_j, &dir_x, &dir_y);

        // Ball is at the racket level && not on the racket
        if (ball_j == 2 && ball_i != left_racket && ball_i != left_racket + 1 && ball_i != left_racket - 1)
            goal(&right_score, &left_racket, &right_racket, &ball_i, &ball_j, &dir_x, &dir_y);
        else if (ball_j == ROW - 3 && ball_i != right_racket && ball_i != right_racket + 1 &&
                 ball_i != right_racket - 1)
            goal(&left_score, &left_racket, &right_racket, &ball_i, &ball_j, &dir_x, &dir_y);

        clear();
        if (left_score == 5) {
            winner = 'l';
            break;
        } else if (right_score == 5) {
            winner = 'r';
            break;
        }
        usleep(50 * 1000);
        mvprintw(0, 0, "LEFT PLAYER SCORE = %d\n", left_score);
        mvprintw(0, 77, "RIGHT PLAYER SCORE = %d\n", right_score);

        draw_field(field);
        refresh();
    }

    endwin();  // End ncurses

    // Winner announcement
    if (winner == 'l') {
        printf("LEFT PLAYER WON!");
    } else if (winner == 'r') {
        printf("RIGHT PLAYER WON!");
    }

    return 0;
}

void frame(char field[STR][ROW]) {
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < ROW; j++) {
            if (i == 0 || i == STR - 1) {
                field[i][j] = '#';
            } else if (j == 0 || j == ROW - 1) {
                field[i][j] = '|';
            } else if (j == ROW / 2) {
                field[i][j] = '.';
            } else {
                field[i][j] = ' ';
            }
        }
    }
}

int racket(char field[STR][ROW], int *left_racket, int *right_racket) {
    char input = getch();

    // Update rackets coordinates
    switch (input) {
        case 's': {
            if ((*left_racket) > 2) (*left_racket)--;
            break;
        }
        case 'x': {
            if ((*left_racket) < 26) (*left_racket)++;
            break;
        }
        case 'j': {
            if ((*right_racket) > 2) (*right_racket)--;
            break;
        }
        case 'n': {
            if ((*right_racket) < 26) (*right_racket)++;
            break;
        }
    }

    // Change rackets positions
    field[*left_racket][1] = '[';
    field[*left_racket + 1][1] = '[';
    field[*left_racket - 1][1] = '[';

    field[*right_racket][ROW - 2] = ']';
    field[*right_racket + 1][ROW - 2] = ']';
    field[*right_racket - 1][ROW - 2] = ']';

    return input;
}

void ball(char field[STR][ROW], int left_racket, int right_racket, int *ball_i, int *ball_j, int *dir_x, int *dir_y) {
    int i = *ball_i, j = *ball_j;

    field[i][j] = 'o';

    if (i == 1 && (*dir_x) == 1 && (*dir_y) == -1)
        (*dir_y) *= -1;
    else if (i == 1 && (*dir_x) == -1 && (*dir_y) == -1)
        (*dir_y) *= -1;
    else if (i == STR - 2 && (*dir_x) == 1 && (*dir_y) == 1)
        (*dir_y) *= -1;
    else if (i == STR - 2 && (*dir_x) == -1 && (*dir_y) == 1)
        (*dir_y) *= -1;
    else if (j == 2 && (*dir_x) == -1 && (*dir_y) == 1 &&
             ((*ball_i) == left_racket || (*ball_i) == left_racket + 1 || (*ball_i) == left_racket - 1))
        (*dir_x) *= -1;
    else if (j == 2 && (*dir_x) == -1 && (*dir_y) == -1 &&
             ((*ball_i) == left_racket || (*ball_i) == left_racket + 1 || (*ball_i) == left_racket - 1))
        (*dir_x) *= -1;
    else if (j == ROW - 3 && (*dir_x) == 1 && (*dir_y) == 1 &&
             ((*ball_i) == right_racket || (*ball_i) == right_racket + 1 || (*ball_i) == right_racket - 1))
        (*dir_x) *= -1;
    else if (j == ROW - 3 && (*dir_x) == 1 && (*dir_y) == -1 &&
             ((*ball_i) == right_racket || (*ball_i) == right_racket + 1 || (*ball_i) == right_racket - 1))
        (*dir_x) *= -1;

    i += (*dir_y);
    j += (*dir_x);

    *ball_i = i;
    *ball_j = j;
}

void goal(int *score, int *left_racket, int *right_racket, int *ball_i, int *ball_j, int *dir_x, int *dir_y) {
    (*score)++;

    *left_racket = STR / 2;
    *right_racket = STR / 2;

    *ball_i = STR / 2;
    *ball_j = ROW / 2;
    (*dir_x) *= -1;
    (*dir_y) *= -1;
}

void draw_field(char field[STR][ROW]) {
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < ROW; j++) {
            printw("%c", field[i][j]);
        }
        printw("\n");
    }
}