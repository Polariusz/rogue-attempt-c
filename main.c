#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/*
 * Used in the struct Mob to know the position of the mobs/player.
*/
struct Position {
	int y;
	int x;
};

/*
 * Used in the struct GameState to know where the end and the beginning of the main window is.
*/
struct Border {
	int h;
	int w;
	int sh;
	int sw;
};

/*
 * Used as enemy mobs and as a player.
*/
struct Mob {
	struct Position pos;
	char repr;
};

/*
 * It's the whole game_state that is passed into multiple methods that initialise the member variables, and into functions that for example display the mob list on the ncurses window.
*/
struct GameState {
	/*
	 * Used to know where the end and the beginning of the main window is.
	*/
	struct Border border;

	/*
	 * Used to know the position of the player and the character that represent the player.
	*/
	struct Mob player;

	/*
	 * Used to display mobs.
	*/
	struct Mob *mob_list;

	/*
	 * It's the length of the member variable mob_list.
	*/
	int mob_list_len;
};

/*
 * Initialises the ncurses and hides echo and cursor.
*/
void init_ncurses(void);

/*
 * Initialises the border for the argument that will be used for the game border and the window
*/
void init_border(struct GameState *game_state);

/*
 * Initialises player for the argument by giving the Mob struct position and a char representation.
*/
void init_player(struct GameState *game_state);

/*
 * Initialises mobs for the argument by randomly placing it inside the border.
*/
void init_mob_list(struct GameState *game_state);

/*
 * Uses ncurses to show the window and place the mobs as well as the player.
*/
void show_on_window(struct GameState *game_state);

/*
 * That's the user input loop: It checks for the input of the user.
 * Keys:
 *     'q' - Quits the loop
 *     'w' - Moves the player up
 *     's' - Moves the player down
 *     'a' - Moves the player left
 *     'd' - Moves the player right
*/
void user_loop(struct GameState *game_state);

/*
 * Ends the ncurses by calling the endwin(); method.
*/
void end_program(struct GameState *game_state);

/*
 * Prints the whole state of the argument out.
 * It uses printf, so the output will be visible once the the ncurses is ended.
*/
void print_state_out(struct GameState *game_state);

/*
 * Returns 0 if there is no collision with the border and 1 if there is.
*/
int is_colliding_with_border(struct GameState *game_state, struct Position *pos);

/*
 * Moves the player into the new position. It clears the character the player was previously in.
*/
void move_player(struct GameState *game_state, struct Position *new_position);

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
		case 'w': {
				struct Position new_pos = {game_state->player.pos.y-1, game_state->player.pos.x};
				if(!is_colliding_with_border(game_state, &new_pos)) {
					move_player(game_state, &new_pos);
				}
				break;
			}
		case 's': {
				struct Position new_pos = {game_state->player.pos.y+1, game_state->player.pos.x};
				if(!is_colliding_with_border(game_state, &new_pos)) {
					move_player(game_state, &new_pos);
				}
				break;
			}
		case 'a': {
				struct Position new_pos = {game_state->player.pos.y, game_state->player.pos.x-1};
				if(!is_colliding_with_border(game_state, &new_pos)) {
					move_player(game_state, &new_pos);
				}
				break;
			}
		case 'd': {
				struct Position new_pos = {game_state->player.pos.y, game_state->player.pos.x+1};
				if(!is_colliding_with_border(game_state, &new_pos)) {
					move_player(game_state, &new_pos);
				}
				break;
			}
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

int is_colliding_with_border(struct GameState *game_state, struct Position *pos)
{
	// Border collision
	if( pos->y >= game_state->border.h-1 ||
		pos->y <= game_state->border.sh ||
		pos->x >= game_state->border.w-1 ||
		pos->x <= game_state->border.sw ) {
			return 1;
		}
	return 0;
}

void move_player(struct GameState *game_state, struct Position *new_position)
{
	mvaddch(game_state->player.pos.y, game_state->player.pos.x, ' ');
	game_state->player.pos = *new_position;
	mvaddch(game_state->player.pos.y, game_state->player.pos.x, '@');

	return;
}
