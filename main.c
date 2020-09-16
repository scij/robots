/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";
#endif // not lint

# include	"robots.h"
# include	<signal.h>
# include	<ctype.h>
# include   <stdlib.h>
# include   <unistd.h>

int main(int argc, char** argv)
{
	register char	*sp;
	register bool	bad_arg;
	register bool	show_only;
	extern char	*Scorefile;
	extern int	Max_per_uid;
	extern char	*rindex();

	show_only = FALSE;
	if (argc > 1) {
		bad_arg = FALSE;
		for (++argv; argc > 1 && *argv[0]; argv++, argc--)
			if (argv[0][0] != '-')
				if (isdigit(argv[0][0]))
					Max_per_uid = atoi(argv[0]);
				else {
					setuid(getuid());
					setgid(getgid());
					Scorefile = argv[0];
# ifdef	FANCY
					sp = rindex(Scorefile, '/');
					if (sp == NULL)
						sp = Scorefile;
					if (strcmp(sp, "pattern_roll") == 0)
						Pattern_roll = TRUE;
					else if (strcmp(sp, "stand_still") == 0)
						Stand_still = TRUE;
					if (Pattern_roll || Stand_still)
						Teleport = TRUE;
# endif
				}
			else
				for (sp = &argv[0][1]; *sp; sp++)
					switch (*sp) {
					  case 's':
						show_only = TRUE;
						break;
					  case 'r':
						Real_time = TRUE;
						break;
					  case 'a':
						Start_level = 4;
						break;
					  case 'j':
						Jump = TRUE;
						break;
					  case 't':
						Teleport = TRUE;
						break;
					  default:
						fprintf(stderr, "robots: uknown option: %c\n", *sp);
						bad_arg = TRUE;
						break;
					}
		if (bad_arg) {
			exit(1);
			/* NOTREACHED */
		}
	}

	if (show_only) {
		show_score();
		exit(0);
		/* NOTREACHED */
	}

	initscr();
	signal(SIGINT, quit);
	crmode();
	noecho();
	nonl();
	if (LINES != Y_SIZE || COLS != X_SIZE) {
		if (LINES < Y_SIZE || COLS < X_SIZE) {
			endwin();
			printf("Need at least a %dx%d screen\n", Y_SIZE, X_SIZE);
			exit(1);
		}
		delwin(stdscr);
		stdscr = newwin(Y_SIZE, X_SIZE, 0, 0);
	}

	srand(getpid());
	if (Real_time)
		signal(SIGALRM, move_robots);
	do {
		init_field();
		for (Level = Start_level; !Dead; Level++) {
			make_level();
			play_level();
		}
		move(My_pos.y, My_pos.x);
		printw("AARRrrgghhhh....");
		refresh();
		score();
	} while (another());
	quit(FALSE);
}

/*
 * quit:
 *	Leave the program elegantly.
 */
void quit(int is_sig)
{
	extern int	_putchar();

	mvcur(0, COLS - 1, LINES - 1, 0);
	/*
	if (CE) {
		tputs(CE, 1, _putchar);
		endwin();
	}
	else {
		endwin();
		putchar('\n');
	}
	*/
	clrtoeol();
	endwin();
	exit(0);
	/* NOTREACHED */
}

/*
 * another:
 *	See if another game is desired
 */
int another()
{
	register int	y;

#ifdef	FANCY
	if ((Stand_still || Pattern_roll) && !Newscore)
		return TRUE;
#endif

	if (query("Another game?")) {
		if (Full_clear) {
			for (y = 1; y <= Num_scores; y++) {
				move(y, 1);
				clrtoeol();
			}
			refresh();
		}
		return TRUE;
	}
	return FALSE;
}
