/* ase_ace.h*/

#define NO 162

float weight [NO];
float elgb [NO];
float v [NO];
float xl [NO];
char failure = 0;
int iter = 0;
float DELTA = 0.9, delta = 0.9; 	/* trace delay rate */
float ALPHA = 1000, alpha = 1000; 	/* rate of change of weight */
float LAMBDA = 0.8, lambda = 0.8; 	/* trace delay rate of ACE */
float GAMMA = 0.95, gamma = 0.95; 	/* reinforcement weightage */
float BETA = 0.5, beta = 0.5; 		/* rate of change of weight of ACE */

skip_ws (FILE *fp)
{
	char ch;

	while ((ch = getc(fp)) == '\n' || ch == ' ' || ch == '\t' || ch == '\t')
		;
	ungetc(ch, fp);

	return;
}

decoder()
{
	int px, pth, pdx, pdth;
	int box_no, i;

	if (x > -2.4 && x <= -0.8)
		px = 1;
	else if (x > -0.8 && x <= 0.8)
		px = 2;
	else if (x > 0.8 && x < 2.4)
		px = 3;
	else
		return -1;

	if (theta > -12 && theta <= -6)
		pth = 1;
	else if (theta > -6 && theta <= -1)
		pth = 2;
	else if (theta > -1 && theta <= 0)
		pth = 3;
	else if (theta > 0 && theta <= 1)
		pth = 4;
	else if (theta > 1 && theta <= 6)
		pth = 5;
	else if (theta > 6 && theta <= 12)
		pth = 6;
	else
		return -1;

	if (dx <= -0.5)
		pdx = 1;
	else if (dx > -0.5 && dx <= 0.5)
		pdx = 2;
	else if (dx > 0.5)
		pdx = 3;
	else
		return -1; /* not reachable code! */


	if (dth <= -50)
		pdth = 1;
	else if (dth > -50 && dth <= 50)
		pdth = 2;
	else if (dth > 50)
		pdth = 3;
	else
		return -1; /* not reachable code! */

	box_no = 54*(px - 1) + 9*(pth - 1) + 3*(pdx - 1) + pdth - 1;

	/* Another order of mapping of parameters onto boxes */
	/*
	box_no = 54*(pdth - 1) + 18*(pdx - 1) + 3*(pth - 1) + px - 1;
	*/
	for (i = 0; i < 162; i++)
		if (i == box_no)
			X[i] = 1;
		else
			X[i] = 0;

	return box_no;
}

init_weight()
{
	FILE *wfp;
	FILE *vfp;

	int i, j;

	wfp = fopen("ase.wt", "r");
	if (wfp)
	{
		j = 0;
		while (!feof(wfp))
		{
			skip_ws(wfp);
			fscanf(wfp, "%f", &weight[j]);
			j++;
		}
		fclose(wfp);
	}
	else
	{
		for (i=0; i<NO; i++)
		{
			weight[i] = (float) (2-random(5));
			weight[i] = 0;
		}
	}

	vfp = fopen("ace.wt", "r");
	if (vfp)
	{
		j = 0;
		while (!feof(vfp))
		{
			skip_ws(vfp);
			fscanf(vfp, "%f", &v[j]);
			j++;
		}
		fclose(vfp);
	}
	else
	{
		for (i = 0; i < NO; i++)
		{
			v[i] = (float) (2 - random(5));
			v[i] = 0;
		}
	}

	for (i = 0; i < NO; i++)
	{
		xl[i] = elgb[i] = 0.0;
	}

	return;
}

ASE()
{
	int y;
	float noise();
	int threshold(float);
	float ACE();
	int i, j;

	float r = 0;	/* reinforcement */
	float sum = 0;

	for (i=0; i<NO; i++)
	{
		sum += weight[i] * X[i];
	}
	sum += noise();
	y = threshold(sum);

	r = ACE(); /* calculate reinforcement */

	for (i=0; i<NO; i++) /* adapt weights */
	{
		weight[i] += alpha * r * elgb[i];
	}
	for (i=0; i<NO; i++) /* calculate eligibility criteria */
	{
		elgb[i] = delta * elgb[i] + (1-delta) * y * X[i];
	}

	return y;
}

float noise()
{
	int i;
	float a = 0.0, v;
	float sigma = 0.01;

	for (i=0; i<12; i++)
	{
		a = a + (float) rand() / RAND_MAX; 
	}
	v = (a - 6.0) * sigma;

	return v;
}

int threshold (float sum)
{
	if (sum < 0)
		return -1;
	else
		return 1;
}

int reinforcement()
{
	if (failure)
		return -1;
	else
		return 0;
}


float ACE()
{
	float p = 0;
	static float p1 = 0;	/* Assuming to start from 0 0 0 0 */

	int r;
	float r1;
	int i;

	if (p1 == 0)
		p1 = v[76];

	for (i=0; i<NO; i++)
	{
		p += v[i]*X[i];
	}

	r = reinforcement();

	for (i=0; i<NO; i++)
	{
		v[i] += beta * (r + gamma*p - p1)*xl[i];
	}

	for (i=0; i<NO; i++)
	{
		xl[i] = lambda*xl[i] + (1 - lambda)*X[i];
	}

	r1 = r + gamma*p - p1;
	p1 = p;

	return r1;
}

write_to_file()
{
	FILE *wfp, *vfp;
	int i;

	wfp = fopen("ase.wt", "w");
	for (i=0; i<NO; i++)
		fprintf(wfp, "%f ", weight[i]);
	fclose(wfp);

	vfp = fopen("ace.wt", "w");
	for (i=0; i<NO; i++)
		fprintf(vfp, "%f ", v[i]);
	fclose(vfp);

	return;
}

afresh()
{
	cleardevice();
	gotoxy(2, 3);
	printf(" Fresh training makes the network forget past training. ");
	gotoxy(2, 4);
	printf(" Are you sure you want to start afresh? (Y/N)  N");
	gotoxy(53, 4);
	if (toupper(getche()) == 'Y')
	{
		unlink("ace.wt");
		unlink("ase.wt");
	}
	return;
}
