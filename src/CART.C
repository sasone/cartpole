#include <stdio.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>

#include "defines.h"
#include "ase_ace.h"
#include "cp.h"
#include "cartsys.h"

/* #include "gr.h" */

#ifndef M_PI
#	define M_PI 3.14159265358979323846
#endif

FILE *opfile; /* output file */
char write_flag = 0;
FILE *plotfile; /* plot file */
int gd, gm;

int init_graphics()
{
	char ch;

	gd = DETECT;
	initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
	setcolor(color);
	setbkcolor(bkcolor);
	cleardevice();

	return 0;
}

int main (int ac, char * av[])
{
	int i, j;
	char choice;
	char done, beautify();

	if (ac < 2)
		write_flag = 0;
	else if (strncmp(av[1], "/file", 2) == 0)
		write_flag = 1;
	else
	opfile= fopen(av[1], "w"), write_flag = 1;


	init_graphics();

	do
	{
		failure = 0;
		iter = 0;
		done = 0;

		if (write_flag)
			fprintf(opfile, "  iter\t    x\t\t     O\t\t    box\t\t    F\t\t");
			/* NOTE: Data is missing in the printed project report. The above line may not be complete.
			 * This may cause the format the output file to be incorrect */

		fflush(stdin);

		choice = toupper(beautify());

		cleardevice();

		if (choice == 'U' /* || choice == '1' */)
		{
			gotoxy(2, 3);
			printf(" Give the values of X, O~, dx and dO~                       \n\t");
			scanf("%f%f%f%f", &x ,&theta, &dx, &dth);
			user_input();
		}
		else if (choice == 'S' || choice == '2')
		{
			setup();
		}
		else if (choice == 'R' || choice == '1')
		{
			train_random_values();
		}
		else if (choice == 'D' || choice == '4')
		{
			afresh();
		}
		else if (choice == 'E' || choice == '0')
		{
			done = 1;
		}

		if (write_flag)
			fprintf(opfile, "\n");
		cleardevice();
	} while (!done);

	closegraph();

	if (write_flag)
		fclose(opfile);

	printf("Hello TurboC world in the year 2024!\n");

	return 0;
}

char beautify()
{
	char ch;

	cleardevice();
	gotoxy(2, 1);
	printf("\n\t\t\t    CARTPOLE SYSTEM         \n\n");
	printf("   _________________________________________________\n");
	printf("\t\t\t1. Random input \n\n");
	printf("\t\t\t2. Setup parameters \n\n");
	printf("\t\t\t3. Defaut parameters \n\n");
	printf("\t\t\t4. Fresh training \n\n");
	printf("\t\t\t0. Exit \n\n");

	rectangle(10, 10, getmaxx()-10, getmaxy()-10);

	ch = getch();
	return ch;
}


/* ---- Temporary function ---- */

kolu_bilisu()
{
	x = th = dx = dth = 0.01;

	do
	{
		cart_pole_system();
		interface();
		draw_cart();
		draw_pole();
		delay(100);
	} while (fabs(theta) < 90);

	getch();

	return;
}
/* ---- kolu_bilisu() ends ---- */

user_input()
{
	int rv;

	th = (M_PI/180) * theta;

	init_screen();
	init_weight();
	init_plot(1);
	init_map();

	do
	{
		interface();
		draw_cart();
		draw_pole();

		rv = decoder();
		if (rv == -1)
			failure = 1;
		F = force * ASE();
		cart_pole_system();
		gotoxy(2, 3);

		/*
		 printf("x = %f, O = %f, dx = %f, dO = %f, iter = %d ", x, theta, d, dth, iter);
		 * */

		if (write_flag)
			fprintf(opfile, "%d\t%f\t%f\t%d\t\%d\n", iter, x, theta, rv, F);

		iter++;
		xthmap();
		erase_cart();
		erase_pole();
	} while (!failure && iter < MAXIT);

	draw_cart();
	draw_pole();

	write_to_file(); /* write the ASE and ACE weights to files */
	plot(2, 1, iter);

	return;
}


gen_random_values(int trial)
{
	static float xval;

	if (!(trial % 10))
		xval = min + (max - min) * (float) random(1001)/1000;
	theta = 6 - 12 * (float) random(1001)/1000;
	dx = 0.25 - 0.5 * (float) random(1001)/1000;
	dth = 0.25 - 0.5 * (float) random(1001)/1000;
	x = xval + noise();

	dx = dth = theta = 0.0; /* ??? */
	return;
}


/* Temp function to fix linking error */
main1()
{
	/* Do nothing for now */
	return 0;
}

train_random_values()
{
	time_t time1;
	int count, i, rv;
	char ch;

	init_screen();
	init_weight();

	if (file_flag)
	{
		plotfile = fopen("plot.out", "w");
		fprintf(plotfile, "%ld\n%f\n%f\n", MAXIT, mp, 1);
	}

	if (map_flag)
		init_map();

	if (msg_flag)
	{
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
		outtextxy(55, 220, mesg);
	}
	gotoxy(2, 2);
	printf("\n How many times should the training be done? :: ");
	fflush(stdin);
	scanf("%d", &count);
	gotoxy(2, 3);
	printf("                                                   ");

	if (plot_flag)
	{
		init_plot(count);
	}
	else
	{
		main1();
	}

	if (!msg_flag)
	{
		gotoxy(50, 2);
		printf("Mass of the pole    :: %5.2f kg", mp);
		gotoxy(50, 3);
		printf("Lenght of the pole  :: %5.2f m", 2*l);
	}

	for (i = 0; i < count; i++)
	{
		time1 = time(NULL);
		randomize();
		if (cp_flag)
		{
			erase_cart();
			erase_pole();
		}
		gen_random_values(i);
		th = (M_PI/180) * theta;
		failure = 0;
		iter = 0;

		init_weight();

		do
		{
			interface();
			if(cp_flag)
			{
				erase_cart();
				erase_pole();
			}

			rv = decoder();
			if (rv == -1)
				failure = 1;

			F = force * ASE();
			cart_pole_system();
			gotoxy(2, 3);
			if (write_flag)
				fprintf(opfile, "%d\t%f\t%f\t%d\t\%d\n", iter, x, theta, rv, F);

			iter++;

			if (map_flag)
				xthmap();

			if (kbhit())
			{
				if ((ch = getch()) == 27)	/* Interrupt on ESC key */
				{
					if (prompt())
						return;
				}
				else
				{
					if (toupper(ch) == 'T')
						update_time(time1, iter);
				}
			}

			if (cp_flag)
			{
				erase_cart();
				erase_pole();
			}
		} while (!failure && iter < MAXIT);


		update_time(time1, iter);

		if (cp_flag)
		{
			draw_cart();
			draw_pole();
		}
		delay(200);

		write_to_file(); /* write the ASE and ACE weights to files */

		if (msg_flag)
		{
			gotoxy(2, 3);
			printf("Trial    :: %d", i+1);
		}

		if (plot_flag)
			plot(count, i+1, iter);
		if (file_flag)
			fprintf(plotfile, "%d\t%d\n", i+1, iter);
		if (write_flag)
			fprintf(opfile, "------------------------------\n");
	}

	if (file_flag)
		fclose(plotfile);
	getch();
	return;
}

prompt()
{
	gotoxy(2, 4);
	printf("User Interrupt. Suspend Operation? (Y/N)   N  ");
	gotoxy(46, 3);

	if (toupper(getch()) == 'Y')
	{
		gotoxy(2, 4);
		printf("                                                           ");
		return 1;
	}

	gotoxy(2, 4);
	printf("                                                           ");
	return 0;
}

update_time(time_t time1, int iter)
{
	time_t time2;
	long dtime, min, sec;

	time2 = time(NULL);
	dtime = (long)difftime(time2, time1);
	sec = dtime % 60;
	min = dtime / 60;

	gotoxy(2, 2);
	if (!msg_flag)
		printf("Iterations   :: %d", iter);

	gotoxy(2, 3);
	if (!msg_flag)
		printf("Time elapsed :: %ld mins, %ld sec ", min, sec);

	return;
}

