/* defines.h */

/* Common data */
float g = 9.8;			/* Acceleration due to gravity */
float MC = 1.0, mc = 1.0; 	/* Mass of the cart */
float MP = 0.1, mp = 0.1;	/* Mass of pole */
float uc = 0.000005; 		/* Friction between cart and track */
float up = 0.000002; 		/* Friction between pole and cart */
float L = 0.5, l = 0.5;		/* Half length of the pole */
float max = 0, min = 0, MAX = 0, MIN = 0;	/* training xvalue limit */
char plot_flag = 1, map_flag = 1, cp_flag = 1, msg_flag = 0, file_flag = 1;
char mesg[] = "On training... Please do not switch off!";

/* Data shared between : cartpole.c and cp.h */
float x = 0.0;
float dx = 0.0;
float th = 0.0;
float dth = 0.0; 
float F = 0.0;
short int X[162];
int NO = 162;
float force = 10;
long MAXIT = 20000;

/* Data shared between : ase_ace.h and cp.h */
float theta = 0.0;
int skip_ws();

