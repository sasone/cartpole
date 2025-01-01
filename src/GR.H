/* gr.h */

#include <graphics.h>
#include <conio.h>
#include <stdio.h>

/*unsigned long MAXIT=10000;*/
char clearflag=1;

int clrline(int line)
{
	return line;
}
skip_ws(FILE *fp)
{
	char ch;

	while ( (ch = getc(fp)) == '\n'
			|| ch == ' ' || ch == '\t' || ch == '\r')
		;

	ungetc(ch, fp);

	return;
}

/* Plotting */

int color = EGA_LIGHTCYAN;
int bkcolor = EGA_LIGHTBLUE;
int grcolor = EGA_WHITE;
int fillcolor = EGA_LIGHTGRAY;
int count;
int gd, gm;

int px = 0, py = 0;
int left = 130, right = 510, top = 36, bottom = 416;

init_plot()
{
	char buf[20];
	int l;
	int avgx, avgy;
	char buffer[20];

	px = left;
	py = bottom;

	setcolor(color);
	setbkcolor(bkcolor);

	if (clearflag && (gd == VGA || gd == EGA))
	{
		rectangle(left, top, right, bottom);
		setfillstyle(SOLID_FILL, fillcolor);
		floodfill(left+1, top+1, color);
		outtextxy((right+left)/2-132, top-18, "Learning graph for ANN controller");
		outtextxy(left-15, bottom+15, "0");
		sprintf(buf, "%-ld", MAXIT);

		l = strlen(buf);
		outtextxy((left - (l+1)*8), top, buf);
		settextstyle(DEFAULT_FONT, VERT_DIR, 1);
		outtextxy(left-20, (top+bottom)/2 - 56, "Iterations >>");
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
		outtextxy((right+left)/2 - 44, bottom+15, "Trials >>");
		sprintf(buf, "%d", count);
		outtextxy(right-20, bottom+15, buffer);

		avgx = (right+left)/2;
		avgy = (top + bottom)/2;
		line(avgx, bottom, avgx, bottom-5);
		line(left, avgy, left+5, avgy);
		line(right, avgy, right-5, avgy);
	}
	return 0;
}

plot (int i, int iter)
{
	float ap, bp, cp, dp;

	unsigned long xmax = count, xmin = 0, ymax = MAXIT, ymin = 0;
	int xplot, yplot;

	ap = (float) (right - left) / (xmax - xmin);
	bp = right - (float) ap*(xmax - xmin);
	cp = (float)(top - bottom)/(ymax - ymin);
	dp = top - (float)cp*ymax;

	setcolor(grcolor);
	xplot = ap*i + bp;
	yplot = cp*iter + dp;
	line(px, py, xplot, yplot);
	px = xplot;
	py = yplot;

	return 0;
} /* plot */

initialise (char filename[])
{
	FILE *fp;
	int i;
	unsigned long limit;
	float mp, l;
	char buf[40];

	fp = fopen(filename, "r");
	if (fp)
	{
		skip_ws(fp);
		fscanf(fp, "%ld", &limit);
		if (limit)
			MAXIT = limit;
		skip_ws(fp);
		fscanf(fp, "%f", &mp);
		skip_ws(fp);
		fscanf(fp, "%f", &l);
		l *= 2;
		sprintf(buf, "Mass of pole : %5.2f Kg", mp);
		outtextxy(left-50, bottom-38, buf);
		sprintf(buf, "Length of pole : %5.2f m", l);
		outtextxy(left+190, bottom+38, buf);

		fseek(fp, -3L, 2);
		while (fgetc(fp) != '\n')
			fseek(fp, -2L, 1);
		skip_ws(fp);
		fscanf(fp, "%d", &i);
		count = i;
		fclose(fp);
		return 1;
	}
	return 0;
}

draw(char filename[])
{
	FILE *fp;
	int iter;
	int i = 0;
	unsigned long dummy;
	float mpdum;

	fp = fopen(filename, "r");
	if (fp)
	{
		init_plot();
		skip_ws(fp);
		fscanf(fp, "%ld", &dummy);
		skip_ws(fp);
		fscanf(fp, "%ld", &dummy);
		skip_ws(fp);
		fscanf(fp, "%ld", &dummy);

		while(!feof(fp))
		{
			skip_ws(fp);
			fscanf(fp, "%d", &i);
			skip_ws(fp);
			fscanf(fp, "%d", &iter);
			plot(i-1, iter);
		}
		fclose(fp);
		return 1;
	}
	return 0;
}

choice()
{
	char ch;
	char filename[256];

	gotoxy(70, 20);
	printf("1. File");
	gotoxy(70, 21);
	printf("2. Color");
	gotoxy(70, 22);
	printf("3. Backgrnd");
	gotoxy(70, 23);
	printf("4. cLear");
	gotoxy(70, 24);
	printf("5. Exit");

	ch = toupper(getch());
	if (ch == '4' || ch == 'L')
	{
		clrline(1);
		gotoxy(2,1);
		printf(" Clear the graphs (Y/N) : N");
		gotoxy(28, 1);
		ch = getche();
		clrline(1);
		if (toupper(ch) == 'Y')
		{
			cleardevice();
			clearflag = 1;
		}
		return 1;
	}

	if (ch =='1' || ch == 'F')
	{
		clrline(1);
		gotoxy(1, 1);
		printf(" Give input filename :: ");
		fflush(stdin);
		gets(filename);
		clrline(1);
		if (initialise(filename))
		{
			draw(filename);
			clearflag = 0;
		}
		else
		{
			clrline(1);
			gotoxy(1, 1);
			printf("\aUnable to open %s. Press a key. ", filename);
			getch();
			clrline(1);
		}
		return 1;
	}

	if (ch == 'C' || ch == '2')
	{
		char i;
		int X;
		char buf[4] = {0, 0, 0, 0};
		int cho;

		X = getmaxx();
		for (i=0; i<16; i++)
		{
			sprintf(buf, "%d", i);
			setcolor(i);
			outtextxy(X-25, 50+i*12, buf);
		}
		clrline(1);

		do
		{
			gotoxy(3, 1);
			printf(" Select color :: ");
			scanf("%d", &cho);
		}
		while (cho < 0 || cho > 15);

		clrline(1);
		for (i=0; i<16; i++)
		{
			gotoxy(77, i+3);
			printf("   ");
		}
		grcolor = cho;
		setcolor(grcolor);
	}

	if (ch == 'B' || ch =='3')
	{
		int i, j;
		char buf[4] = {0, 0, 0, 0};
		int cho, X;

		X = getmaxx();
		for (i=0; i<16; i++)
		{
			sprintf(buf, "%d", i);
			setcolor(i);
			outtextxy(X-25, 50+i*12, buf);
		}

		clrline(1);

		do
		{
			gotoxy(3, 1);
			printf(" Select color :: ");
			scanf("%d", &cho);
		}
		while (cho < 0 || cho > 15);

		clrline(1);
		for(i=0; i<16; i++)
		{
			gotoxy(77, i+3);
			printf("   ");
		}

		for (i=left+1; i<right; i++)
		{
			for (j=top+1; j<bottom; j++)
			{
				if (getpixel(i, j) == fillcolor)
				{
					putpixel(i, i, cho);
				}
			}
		}
		fillcolor = cho;
	}

	if (ch == 'E' || ch == '5' || ch == 27)
	{
		clrline(1);
		gotoxy(2, 1);
		printf(" Exit from program (Y/N) : N");
		gotoxy(30, 1);
		ch = getche();
		clrline(1);
		if (toupper(ch) == 'Y')
			return 0;
		else
			return 1;
	}
	return 1;
}

main1()
{
	char ch;

	gd = DETECT;
	initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
	setcolor(color);
	setbkcolor(bkcolor);
	cleardevice();
	while (choice() != 0)
		;
	closegraph();

	return 0;
}