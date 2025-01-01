/* cp.h */

#include <graphics.h>

const int TOPY = 149, BOTY = 181, ENDX1 = 30, ENDX2 =  610;
int CENTER = 319;
int prev_center = 319;
int X2, Y2;
float pole_len = 100;
int color, bkcolor, gd;
char BGI_PATH[] = "C:\\TURBOC3\\BGI";

draw_cart()
{
	int startx = CENTER - 50;

	startx = (startx <= ENDX1)? ENDX1 :
			(startx + 100 >= ENDX2)? ENDX2 - 100 : startx;

	CENTER = startx + 50;

	setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
	rectangle(startx, TOPY, startx+100, BOTY);
	setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
	circle(startx+25, BOTY, 8);
	circle(startx+75, BOTY, 8);
	rectangle(startx+45, TOPY-10, startx+55, TOPY);
	xthmap();

	return;
}

draw_pole()
{
	static int Y1;

	Y1 = TOPY - 5;
	X2 = (int) (CENTER + sin((M_PI/180) * theta) * pole_len);
	Y2 = (int) (Y1 - cos((M_PI/180) * theta) * pole_len);
	line(CENTER, Y1, X2, Y2);

	return;
}

init_screen()
{
	int gm;

	gd = DETECT;

	initgraph(&gd, &gm, BGI_PATH);

	if (gd == CGA || gd == HERCMONO)
	{
		color = WHITE;
		bkcolor = BLACK;
	}
	else if (gd == VGA || gd == EGA)
	{
		color = EGA_WHITE;
		bkcolor = EGA_BLACK;
		window(1, 14, 80, 25);
	}

	setcolor(color);
	setbkcolor(bkcolor);
	cleardevice();

	if (cp_flag)
	{
		rectangle(20, 20, 620, 199);
		outtextxy(200, 4, "ANN controlled Cart-Pole System ");
		line(21, 169, 28, 169);
		moveto(28, 169);
		linerel(0, 21);
		linerel(584, 0);
		linerel(0, -21);
		linerel(8, 0);

		if (gd == VGA || gd == EGA)
		{
			setfillstyle(HATCH_FILL, EGA_RED);
			floodfill(618, 196, EGA_WHITE);
		}
	}

	return;
}

erase_cart()
{
	setcolor(bkcolor);
	draw_cart();
	setcolor(color);

	return;
}

erase_pole()
{
	static int Y1;

	setcolor(bkcolor);
	Y1 = TOPY - 5;
	X2 = (int) (CENTER + sin((M_PI/180) * theta) * pole_len);
	Y2 = (int) (Y1 - cos((M_PI/180) * theta) * pole_len);
	line(CENTER, Y1, X2, Y2);
	setcolor(color);

	return;
}

interface()
{
	int a = 100, b = 320;

	CENTER = (int) (a * x + b);
	theta = th * (180/M_PI);

	return;
}

xthmap()
{
	float a = 31.25, b = 125.0, c = 6.25, d = 375.0;
	float xmap, thmap;
	char i;
	static float lastx[5]={125, 125, 125, 125, 125},
				 lasty[5]={375, 375, 375, 375, 375};

	if (gd == VGA || gd == EGA || gd == HERCMONO)
	{
		xmap = a * x + b;
		thmap = c * theta + d;

		if(!(xmap < 50) && !(xmap > 200) && !(thmap < 300) && !(thmap > 450))
			putpixel(xmap, thmap, EGA_BLACK); /* only if within range */


		for (i=0; i<5; i++)
		{
			if (!(lastx[i] < 50) && !(lastx[i] > 200) && 
					!(lasty[i] < 100) && !(lasty[i] > 450))
			{
				if (i < 4)
					putpixel(lastx[i], lasty[i], EGA_BLACK);
				else
					putpixel(lastx[i], lasty[i], EGA_WHITE);
			}
		}

		for (i=4; i>0; i--)
			lastx[i] = lastx[i-1], lasty[i] = lasty[i-1];

		lastx[0] = xmap, lasty[0] = thmap;
	}
	return;
}

init_map()
{
	if (gd == VGA || gd == EGA || gd == HERCMONO)
	{
		outtextxy(77, 275, "State Graph ");
		rectangle(50, 300, 200, 450);
		setfillstyle(SOLID_FILL, EGA_BLACK);
		floodfill(125, 375, EGA_WHITE);
		outtextxy(34, 460, "-2.4");
		outtextxy(184, 460, "2.4");
		outtextxy(18, 300, "-12");
		outtextxy(18, 450, "+12");
		outtextxy(125, 460, "X");
		outtextxy(34, 375, "O");
	}

	return;
}

/* Plotting */

int px = 0, py = 0;
int left = 400, right = 550, top = 300, bottom = 450;

init_plot(int count)
{
	char buf[20];
	int l;

	px = left;
	py = bottom;

	if (gd == VGA || gd == EGA)
	{
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
		outtextxy(411, 275, "Learning Graph ");
		rectangle(left, top, right, bottom);
		setfillstyle(SOLID_FILL, EGA_BLACK);
		floodfill(475, 375, color);
		outtextxy(370, 460, "0");
		sprintf(buf, "%-ld", MAXIT);

		l = strlen(buf);
		outtextxy((left - (l+1)*8), top, buf);
		settextstyle(DEFAULT_FONT, VERT_DIR, 1);
		outtextxy(370, 340, "#Iter ->");
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
		outtextxy(430, 460, "Trials ->");
		sprintf(buf, "%-d", count);
		outtextxy(550, 465, buf);


		line(475, 300, 475, 305);
		line(475, 450, 475, 445);
		line(400, 375, 405, 375);
		line(550, 375, 545, 375);
		/*
		avgx = (right+left)/2;
		avgy = (top + bottom)/2;
		line(avgx, bottom, avgx, bottom-5);
		line(left, avgy, left+5, avgy);
		line(right, avgy, right-5, avgy);
		*/
	}
	return 0;
}

plot (int count, int i, int iter)
{
	float ap, bp, cp, dp;

	char buffer[20];
	// int left=400, right=550, top=300, bottom=450;
	int xmax = count, xmin = 0, ymax = MAXIT, ymin = 0;
	int xplot, yplot;

	ap = (float) (right - left) / (xmax - xmin);
	bp = right - (float) ap*(xmax - xmin);
	cp = (float)(top - bottom)/(ymax - ymin);
	dp = top - (float)cp*ymax;

	xplot = ap*i + bp;
	yplot = cp*iter + dp;
	setcolor(EGA_RED);
	circle(xplot, yplot, 1);
	setcolor(color);
	line(px, py, xplot, yplot);
	px = xplot;
	py = yplot;

	return 0;
} /* plot */


setup()
{
	char choice;

	while (1)
	{
		cleardevice();
		gotoxy(2,1);
		printf(" 1. Mass of the cart ( %f)\n", mc);
		printf(" 2. Mass of the pole ( %f)\n", mp);
		printf(" 3. Half lenth of the pole ( %f)\n", l);
		printf(" 4. Select maps on the screen \n");
		printf(" 5. Set neural parameters \n");
		printf(" 0. Exit \n");
		printf("These menu options are being implemted\n");

		choice = getche();


		return 0;
	}
}
