

/* cartsys.h */

float d2th = 0; 		/* Angular accl. of pole dO/dt */
float ff(float, float);	/* equation for dO/dt */
float gf(float); 		/* equation for dx/dt */

rk_cart_pole_systen()
{
	float z, b, k1, l1, k2, l2, s1, t1, s2, t2;
	static float h=0.01;

	z = dth;
	d2th = ff(dth, th);
	k1 = h * ff(z, th);
	l1 = h * z;
	k2 = h * ff(z+k1, th+l1);
	l2 = h * (z + k1);
	th = th + (l1 + l2)/2;
	dth = dth + (k1 + k2)/2;

	b = dx;
	s1 = h * gf(b);
	t1 = h * b;
	s2 = h * gf(b + s1);
	t2 = h * (b + s1);
	x = x + (t1 + t2)/2;
	dx = dx + (s1 + s2)/2;

	return;
}

cart_pole_system()
{
	float z, b, z1, b1;
	static float h = 0.005;

	z = dx;
	b = dth;
	d2th = ff(dth, th);

	z1 = z + h * gf(dx);
	b1 = b + h * ff(b, th);

	x = x + h*z;
	th = th + h*b;
	dx = z1;
	dth = b1;

	return;
}


float ff(float z, float th)
{
	float n, d;
	int sn(float);

	n = cos(th)
		* ( -F -mp*l*z*z*sin(th) + uc * sn(dx) / (mc + mp));
	n = n - ((up*z) / (mp*l));
	d = l * (4/3 - (mp * cos(th)*cos(th)) / (mc + mp));

	return ((n + g * sin(th)) / d);
}


float gf(float dx)
{
	float n, d;
	int sn(float);
	n = F + mp*l*(dth*dth*sin(th) - d2th*cos(th)) - uc * sn(dx);
	d = mc + mp;

	return (n / d);
}


int sn(float x)
{
	if (x < 0)
		return -1;
	else
		return 1;
}