#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

struct Position {
	int y;
	int x;
};

struct Border {
	int h;
	int w;
	int sh;
	int sw;
};

struct Mob {
	struct Position pos;
	char repr;
};

struct GameState {
	struct Mob player;
	struct Border border;
	struct Mob *mob_list;
	int mob_list_len;
};

void init_ncurses(void);
void init_border(struct GameState *game_state);
void init_player(struct GameState *game_state);
void init_mob_list(struct GameState *game_state);
void show_on_window(struct GameState *game_state);
void user_loop(struct GameState *game_state);
void end_program(struct GameState *game_state);
void print_state_out(struct GameState *game_state);

int main(void)
{
	struct GameState game_state;
	init_ncurses();

	init_border(&game_state);
	init_player(&game_state);
	init_mob_list(&game_state);
	show_on_window(&game_state);
	user_loop(&game_state);
	end_program(&game_state);
	print_state_out(&game_state);

	printf("Just Monika.\n");
	return 0;
}

void init_ncurses(void)
{
	// Initialise the ncurses screen
	initscr();
	// Any typed by user keys won't be displayed
	noecho();
	// Cursor is invisible
	curs_set(0);


	return;
}

void init_border(struct GameState *game_state)
{
	struct Border border = {40, 80, 0, 0};
	game_state->border = border;

	return;
}

void init_player(struct GameState *game_state)
{
	struct Position position = {1, 1};
	struct Mob player = {position, '@'};
	game_state->player = player;

	return;
}

void init_mob_list(struct GameState *game_state)
{
	const int MOB_CNT = 400;
	struct Mob *mob_list = malloc(MOB_CNT * sizeof(struct Mob));

	for(int i = 0; i < MOB_CNT; i++) {
		int h = 1 + rand() % (game_state->border.h - 2);
		int w = 1 + rand() % (game_state->border.w - 2);

		struct Position pos = {h, w};
		struct Mob mob = {pos, 'r'};

		mob_list[i] = mob;
	}

	game_state->mob_list = mob_list;
	game_state->mob_list_len = MOB_CNT;

	return;
}

void show_on_window(struct GameState *game_state)
{
	// let's make a window
	WINDOW *main_window = newwin(game_state->border.h, game_state->border.w, game_state->border.sh, game_state->border.sw);

	// let's define how the window looks
	box(main_window, 0, 0);

	refresh();
	wrefresh(main_window);

	(void) game_state;

	for(int i = 0; i < game_state->mob_list_len; i++) {
		mvaddch(game_state->mob_list[i].pos.y, game_state->mob_list[i].pos.x, game_state->mob_list[i].repr);
	}

	mvaddch(game_state->player.pos.y, game_state->player.pos.x, game_state->player.repr);

	return;
}

void user_loop(struct GameState *game_state)
{
	(void) game_state;

	char user_input;
	while((user_input = getch()) != 'q') {
		switch (user_input) {
			case 'w':
				break;
			case 's':
				break;
			case 'a':
				break;
			case 'd':
				break;
		}
	}
}

void end_program(struct GameState *game_state)
{
	(void) game_state;

	// Shutdown the ncurses screen
	endwin();
}

void print_state_out(struct GameState *game_state)
{
	printf("Player: \n");
	printf("    y:%d\n", game_state->player.pos.y);
	printf("    x:%d\n", game_state->player.pos.x);
	printf("    repr:%c\n", game_state->player.repr);
	printf("\n");

	printf("Border: \n");
	printf("    h:%d\n", game_state->border.h);
	printf("    w:%d\n", game_state->border.w);
	printf("    sh:%d\n", game_state->border.sh);
	printf("    sw:%d\n", game_state->border.sw);
	printf("\n");

	printf("Mob list: \n");
	for(int i = 0; i < game_state->mob_list_len; i++) {

		printf("    Mob %d: \n", i);
		printf("        y:%d\n", game_state->mob_list[i].pos.y);
		printf("        x:%d\n", game_state->mob_list[i].pos.x);
		printf("        repr:%c\n", game_state->mob_list[i].repr);
	}
	printf("\n");

	printf("Mob list len: %d\n", game_state->mob_list_len);
	printf("\n");
}
