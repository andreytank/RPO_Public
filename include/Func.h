/*
 * Func.h
 *
 * File defining the functions for analysis.
 *
 *
 * @author ref CEC2013_RealParameterOptimization_TechnicalReport
 * @author Andrei Staradubets astarodubetc@mail.ru
 */

#ifndef P1_FUNC_H
#define P1_FUNC_H

#define INF 1.0e99
#define EPS 1.0e-14
#define E  2.7182818284590452353602874713526625
#define PI 3.1415926535897932384626433832795029

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "math.h"

using namespace std;

class Func {
    private:
    int _Dsize, func_num;
	double *OShift,*M,*y,*z,*x_bound;
    vector<int> _Dmax;
	
	void sphere_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Sphere */{
		int i;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		f[0] = 0.0;
		for (i=0; i<nx; i++)
		{
			f[0] += z[i]*z[i];
		}
	}

	void ellips_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Ellipsoidal */{
		int i;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		oszfunc (z, y, nx);
		f[0] = 0.0;
		for (i=0; i<nx; i++)
		{
			f[0] += pow(10.0,6.0*i/(nx-1))*y[i]*y[i];
		}
	}

	void bent_cigar_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Bent_Cigar */{
		int i;
		double beta=0.5;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		asyfunc (z, y, nx,beta);
		if (r_flag==1)
		rotatefunc(y, z, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		f[0] = z[0]*z[0];
		for (i=1; i<nx; i++)
		{
			f[0] += pow(10.0,6.0)*z[i]*z[i];
		}
	}

	void discus_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Discus */{
		int i;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		oszfunc (z, y, nx);

		f[0] = pow(10.0,6.0)*y[0]*y[0];
		for (i=1; i<nx; i++)
		{
			f[0] += y[i]*y[i];
		}
	}

	void dif_powers_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Different Powers */{
		int i;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		f[0] = 0.0;
		for (i=0; i<nx; i++)
		{
			f[0] += pow(fabs(z[i]),2+4*i/(nx-1));
		}
		f[0]=pow(f[0],0.5);
	}

	void rosenbrock_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Rosenbrock's */{
		int i;
		double tmp1,tmp2;
		shiftfunc(x, y, nx, Os);//shift
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]=y[i]*2.048/100;
		}
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);//rotate
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		for (i=0; i<nx; i++)//shift to orgin
		{
			z[i]=z[i]+1;
		}

		f[0] = 0.0;
		for (i=0; i<nx-1; i++)
		{
			tmp1=z[i]*z[i]-z[i+1];
			tmp2=z[i]-1.0;
			f[0] += 100.0*tmp1*tmp1 +tmp2*tmp2;
		}
	}

	void schaffer_F7_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Schwefel's 1.2  */{
		int i;
		double tmp;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];
		asyfunc (z, y, nx, 0.5);
		for (i=0; i<nx; i++)
			z[i] = y[i]*pow(10.0,1.0*i/(nx-1)/2.0);
		if (r_flag==1)
		rotatefunc(z, y, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		for (i=0; i<nx-1; i++)
			z[i]=pow(y[i]*y[i]+y[i+1]*y[i+1],0.5);
		f[0] = 0.0;
		for (i=0; i<nx-1; i++)
		{
		  tmp=sin(50.0*pow(z[i],0.2));
		  f[0] += pow(z[i],0.5)+pow(z[i],0.5)*tmp*tmp ;
		}
		f[0] = f[0]*f[0]/(nx-1)/(nx-1);
	}

	void ackley_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Ackley's  */{
		int i;
		double sum1, sum2;

		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		asyfunc (z, y, nx, 0.5);
		for (i=0; i<nx; i++)
			z[i] = y[i]*pow(10.0,1.0*i/(nx-1)/2.0);
		if (r_flag==1)
		rotatefunc(z, y, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		sum1 = 0.0;
		sum2 = 0.0;
		for (i=0; i<nx; i++)
		{
			sum1 += y[i]*y[i];
			sum2 += cos(2.0*PI*y[i]);
		}
		sum1 = -0.2*sqrt(sum1/nx);
		sum2 /= nx;
		f[0] =  E - 20.0*exp(sum1) - exp(sum2) +20.0;
	}

	void weierstrass_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Weierstrass's  */{
		int i,j,k_max;
		double sum,sum2, a, b;

		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]=y[i]*0.5/100;
		}
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		asyfunc (z, y, nx, 0.5);
		for (i=0; i<nx; i++)
			z[i] = y[i]*pow(10.0,1.0*i/(nx-1)/2.0);
		if (r_flag==1)
		rotatefunc(z, y, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		a = 0.5;
		b = 3.0;
		k_max = 20;
		f[0] = 0.0;
		for (i=0; i<nx; i++)
		{
			sum = 0.0;
			sum2 = 0.0;
			for (j=0; j<=k_max; j++)
			{
				sum += pow(a,j)*cos(2.0*PI*pow(b,j)*(y[i]+0.5));
				sum2 += pow(a,j)*cos(2.0*PI*pow(b,j)*0.5);
			}
			f[0] += sum;
		}
		f[0] -= nx*sum2;
	}

	void griewank_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Griewank's  */{
		int i;
		double s, p;

		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]=y[i]*600.0/100.0;
		}
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		for (i=0; i<nx; i++)
			z[i] = z[i]*pow(100.0,1.0*i/(nx-1)/2.0);


		s = 0.0;
		p = 1.0;
		for (i=0; i<nx; i++)
		{
			s += z[i]*z[i];
			p *= cos(z[i]/sqrt(1.0+i));
		}
		f[0] = 1.0 + s/4000.0 - p;
	}

	void rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Rastrigin's  */{
		int i;
		double alpha=10.0,beta=0.2;
		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]=y[i]*5.12/100;
		}

		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		oszfunc (z, y, nx);
		asyfunc (y, z, nx, beta);

		if (r_flag==1)
		rotatefunc(z, y, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		for (i=0; i<nx; i++)
		{
			y[i]*=pow(alpha,1.0*i/(nx-1)/2);
		}

		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		f[0] = 0.0;
		for (i=0; i<nx; i++)
		{
			f[0] += (z[i]*z[i] - 10.0*cos(2.0*PI*z[i]) + 10.0);
		}
	}

	void step_rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Noncontinuous Rastrigin's  */{
		int i;
		double alpha=10.0,beta=0.2;
		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]=y[i]*5.12/100;
		}

		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		for (i=0; i<nx; i++)
		{
			if (fabs(z[i])>0.5)
			z[i]=floor(2*z[i]+0.5)/2;
		}

		oszfunc (z, y, nx);
		asyfunc (y, z, nx, beta);

		if (r_flag==1)
		rotatefunc(z, y, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		for (i=0; i<nx; i++)
		{
			y[i]*=pow(alpha,1.0*i/(nx-1)/2);
		}

		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		f[0] = 0.0;
		for (i=0; i<nx; i++)
		{
			f[0] += (z[i]*z[i] - 10.0*cos(2.0*PI*z[i]) + 10.0);
		}
	}

	void schwefel_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Schwefel's  */{
		int i;
		double tmp;
		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]*=1000/100;
		}
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		for (i=0; i<nx; i++)
			y[i] = z[i]*pow(10.0,1.0*i/(nx-1)/2.0);

		for (i=0; i<nx; i++)
			z[i] = y[i]+4.209687462275036e+002;
		
		f[0]=0;
		for (i=0; i<nx; i++)
		{
			if (z[i]>500)
			{
				f[0]-=(500.0-fmod(z[i],500))*sin(pow(500.0-fmod(z[i],500),0.5));
				tmp=(z[i]-500.0)/100;
				f[0]+= tmp*tmp/nx;
			}
			else if (z[i]<-500)
			{
				f[0]-=(-500.0+fmod(fabs(z[i]),500))*sin(pow(500.0-fmod(fabs(z[i]),500),0.5));
				tmp=(z[i]+500.0)/100;
				f[0]+= tmp*tmp/nx;
			}
			else
				f[0]-=z[i]*sin(pow(fabs(z[i]),0.5));
		}
		f[0]=4.189828872724338e+002*nx+f[0];
	}

	void katsuura_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Katsuura  */{
		int i,j;
		double temp,tmp1,tmp2,tmp3;
		tmp3=pow(1.0*nx,1.2);
		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]*=5.0/100.0;
		}
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		for (i=0; i<nx; i++)
			z[i] *=pow(100.0,1.0*i/(nx-1)/2.0);

		if (r_flag==1)
		rotatefunc(z, y, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		f[0]=1.0;
		for (i=0; i<nx; i++)
		{
			temp=0.0;
			for (j=1; j<=32; j++)
			{
				tmp1=pow(2.0,j);
				tmp2=tmp1*y[i];
				temp += fabs(tmp2-floor(tmp2+0.5))/tmp1;
			}
			f[0] *= pow(1.0+(i+1)*temp,10.0/tmp3);
		}
		tmp1=10.0/nx/nx;
		f[0]=f[0]*tmp1-tmp1;

	}

	void bi_rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Lunacek Bi_rastrigin Function */{
		int i;
		double mu0=2.5,d=1.0,s,mu1,tmp,tmp1,tmp2;
		double *tmpx;
		tmpx=(double *)malloc(sizeof(double)  *  nx);
		s=1.0-1.0/(2.0*pow(nx+20.0,0.5)-8.2);
		mu1=-pow((mu0*mu0-d)/s,0.5);

		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]*=10.0/100.0;
		}

		for (i = 0; i < nx; i++)
		{
			tmpx[i]=2*y[i];
			if (Os[i] < 0.)
				tmpx[i] *= -1.;
		}

		for (i=0; i<nx; i++)
		{
			z[i]=tmpx[i];
			tmpx[i] += mu0;
		}
		if (r_flag==1)
		rotatefunc(z, y, nx, Mr);
		else
		for (i=0; i<nx; i++)
			y[i]=z[i];

		for (i=0; i<nx; i++)
			y[i] *=pow(100.0,1.0*i/(nx-1)/2.0);
		if (r_flag==1)
		rotatefunc(y, z, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		tmp1=0.0;tmp2=0.0;
		for (i=0; i<nx; i++)
		{
			tmp = tmpx[i]-mu0;
			tmp1 += tmp*tmp;
			tmp = tmpx[i]-mu1;
			tmp2 += tmp*tmp;
		}
		tmp2 *= s;
		tmp2 += d*nx;
		tmp=0;
		for (i=0; i<nx; i++)
		{
			tmp+=cos(2.0*PI*z[i]);
		}
		
		if(tmp1<tmp2)
			f[0] = tmp1;
		else
			f[0] = tmp2;
		f[0] += 10.0*(nx-tmp);
		free(tmpx);
	}

	void grie_rosen_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Griewank-Rosenbrock  */{
		int i;
		double temp,tmp1,tmp2;

		shiftfunc(x, y, nx, Os);
		for (i=0; i<nx; i++)//shrink to the orginal search range
		{
			y[i]=y[i]*5/100;
		}
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		for (i=0; i<nx; i++)//shift to orgin
		{
			z[i]=y[i]+1;
		}

		f[0]=0.0;
		for (i=0; i<nx-1; i++)
		{
			tmp1 = z[i]*z[i]-z[i+1];
			tmp2 = z[i]-1.0;
			temp = 100.0*tmp1*tmp1 + tmp2*tmp2;
			 f[0] += (temp*temp)/4000.0 - cos(temp) + 1.0;
		}
		tmp1 = z[nx-1]*z[nx-1]-z[0];
		tmp2 = z[nx-1]-1.0;
		temp = 100.0*tmp1*tmp1 + tmp2*tmp2;;
		 f[0] += (temp*temp)/4000.0 - cos(temp) + 1.0 ;
	}

	void escaffer6_func (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Expanded Scaffer��s F6  */{
		int i;
		double temp1, temp2;
		shiftfunc(x, y, nx, Os);
		if (r_flag==1)
		rotatefunc(y, z, nx, Mr);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		asyfunc (z, y, nx, 0.5);
		if (r_flag==1)
		rotatefunc(y, z, nx, &Mr[nx*nx]);
		else
		for (i=0; i<nx; i++)
			z[i]=y[i];

		f[0] = 0.0;
		for (i=0; i<nx-1; i++)
		{
			temp1 = sin(sqrt(z[i]*z[i]+z[i+1]*z[i+1]));
			temp1 =temp1*temp1;
			temp2 = 1.0 + 0.001*(z[i]*z[i]+z[i+1]*z[i+1]);
			f[0] += 0.5 + (temp1-0.5)/(temp2*temp2);
		}
		temp1 = sin(sqrt(z[nx-1]*z[nx-1]+z[0]*z[0]));
		temp1 =temp1*temp1;
		temp2 = 1.0 + 0.001*(z[nx-1]*z[nx-1]+z[0]*z[0]);
		f[0] += 0.5 + (temp1-0.5)/(temp2*temp2);
	}

	void cf01 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 1 */{
		int i,cf_num=5;
		double fit[5];
		double delta[5] = {10, 20, 30, 40, 50};
		double bias[5] = {0, 100, 200, 300, 400};
		
		i=0;
		rosenbrock_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/1e+4;
		i=1;
		dif_powers_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/1e+10;
		i=2;
		bent_cigar_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/1e+30;
		i=3;
		discus_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/1e+10;
		i=4;
		sphere_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],0);
		fit[i]=10000*fit[i]/1e+5;
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void cf02 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 2 */{
		int i,cf_num=3;
		double fit[3];
		double delta[3] = {20,20,20};
		double bias[3] = {0, 100, 200};
		for(i=0;i<cf_num;i++)
		{
			schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		}
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void cf03 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 3 */{
		int i,cf_num=3;
		double fit[3];
		double delta[3] = {20,20,20};
		double bias[3] = {0, 100, 200};
		for(i=0;i<cf_num;i++)
		{
			schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		}
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void cf04 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 4 */{
		int i,cf_num=3;
		double fit[3];
		double delta[3] = {20,20,20};
		double bias[3] = {0, 100, 200};
		i=0;
		schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/4e+3;
		i=1;
		rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/1e+3;
		i=2;
		weierstrass_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/400;
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void cf05 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 4 */{
		int i,cf_num=3;
		double fit[3];
		double delta[3] = {10,30,50};
		double bias[3] = {0, 100, 200};
		i=0;
		schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/4e+3;
		i=1;
		rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/1e+3;
		i=2;
		weierstrass_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/400;
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void cf06 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 6 */{
		int i,cf_num=5;
		double fit[5];
		double delta[5] = {10,10,10,10,10};
		double bias[5] = {0, 100, 200, 300, 400};
		i=0;
		schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/4e+3;
		i=1;
		rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/1e+3;
		i=2;
		ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/1e+10;
		i=3;
		weierstrass_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/400;
		i=4;
		griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=1000*fit[i]/100;
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);

	}

	void cf07 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 7 */{
		int i,cf_num=5;
		double fit[5];
		double delta[5] = {10,10,10,20,20};
		double bias[5] = {0, 100, 200, 300, 400};
		i=0;
		griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/100;
		i=1;
		rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/1e+3;
		i=2;
		schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/4e+3;
		i=3;
		weierstrass_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/400;
		i=4;
		sphere_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],0);
		fit[i]=10000*fit[i]/1e+5;
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void cf08 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) /* Composition Function 8 */{
		int i,cf_num=5;
		double fit[5];
		double delta[5] = {10,20,30,40,50};
		double bias[5] = {0, 100, 200, 300, 400};
		i=0;
		grie_rosen_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/4e+3;
		i=1;
		schaffer_F7_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/4e+6;
		i=2;
		schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/4e+3;
		i=3;
		escaffer6_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],r_flag);
		fit[i]=10000*fit[i]/2e+7;
		i=4;
		sphere_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],0);
		fit[i]=10000*fit[i]/1e+5;
		cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
	}

	void shiftfunc (double *x, double *xshift, int nx,double *Os){
		int i;
		for (i=0; i<nx; i++)
		{
			xshift[i]=x[i]-Os[i];
		}
	}

	void rotatefunc (double *x, double *xrot, int nx,double *Mr){
		int i,j;
		for (i=0; i<nx; i++)
		{
			xrot[i]=0;
				for (j=0; j<nx; j++)
				{
					xrot[i]=xrot[i]+x[j]*Mr[i*nx+j];
				}
		}
	}

	void asyfunc (double *x, double *xasy, int nx, double beta){
		int i;
		for (i=0; i<nx; i++)
		{
			if (x[i]>0)
			xasy[i]=pow(x[i],1.0+beta*i/(nx-1)*pow(x[i],0.5));
		}
	}

	void oszfunc (double *x, double *xosz, int nx){
		int i,sx;
		double c1,c2,xx;
		for (i=0; i<nx; i++)
		{
			if (i==0||i==nx-1)
			{
				if (x[i]!=0)
					xx=log(fabs(x[i]));
				if (x[i]>0)
				{	
					c1=10;
					c2=7.9;
				}
				else
				{
					c1=5.5;
					c2=3.1;
				}	
				if (x[i]>0)
					sx=1;
				else if (x[i]==0)
					sx=0;
				else
					sx=-1;
				xosz[i]=sx*exp(xx+0.049*(sin(c1*xx)+sin(c2*xx)));
			}
			else
				xosz[i]=x[i];
		}
	}

	void cf_cal(double *x, double *f, int nx, double *Os,double * delta,double * bias,double * fit, int cf_num){
	int i,j;
	double *w;
	double w_max=0,w_sum=0;
	w=(double *)malloc(cf_num * sizeof(double));
	for (i=0; i<cf_num; i++)
	{
		fit[i]+=bias[i];
		w[i]=0;
		for (j=0; j<nx; j++)
		{
			w[i]+=pow(x[j]-Os[i*nx+j],2.0);
		}
		if (w[i]!=0)
			w[i]=pow(1.0/w[i],0.5)*exp(-w[i]/2.0/nx/pow(delta[i],2.0));
		else
			w[i]=INF;
		if (w[i]>w_max)
			w_max=w[i];
	}

	for (i=0; i<cf_num; i++)
	{
		w_sum=w_sum+w[i];
	}
	if(w_max==0)
	{
		for (i=0; i<cf_num; i++)
			w[i]=1;
		w_sum=cf_num;
	}
	f[0] = 0.0;
    for (i=0; i<cf_num; i++)
    {
		f[0]=f[0]+w[i]/w_sum*fit[i];
    }
	free(w);
}
	
    public:
    Func(int nx, int constrfunc_num): _Dsize(nx), func_num(constrfunc_num){
		_Dmax.clear();
		_Dmax= vector<int>(_Dsize,200);
		int cf_num=10,i;
		ifstream fpt;
		y=new double[nx];
		z=new double [nx];
		x_bound=new double [nx];
		for (i=0; i<nx; i++)
			x_bound[i]=100.0;

		if (!(nx==2||nx==5||nx==10||nx==20||nx==30||nx==40||nx==50||nx==60||nx==70||nx==80||nx==90||nx==100))
		{
			cerr<<endl<<"Error: Test functions are only defined for D=2,5,10,20,30,40,50,60,70,80,90,100."<<endl;
			exit(-3);
		}
		char FileName[30];
		sprintf(FileName, "input_data/M_D%d.txt", nx);
		fpt.open(FileName);
		if (!fpt.is_open())
		{
			cerr<<endl<<"Error: Cannot open input file for reading"<<endl;
			exit(-4);
		}

		M=new double[cf_num*nx*nx];
		for (i=0; i<cf_num*nx*nx; i++)
		{
				fpt>>M[i];
		}
		fpt.close();
		

		fpt.open("input_data/shift_data.txt");
		if (!fpt.is_open())
		{
			cerr<<endl<<"Error: Cannot open input file for reading"<<endl;
			exit(-4);
		}
		OShift=new double[nx*cf_num];
		for(i=0;i<cf_num*nx;i++)
		{
				fpt>>OShift[i];
		}
		fpt.close();
    }
    
	~Func(){
		delete[] y;
		delete[] z;
		delete[] M;
		delete[] OShift;
		delete[] x_bound;
	}
	
	const int getNumParameters(){
        return _Dsize;
    }

    const int getWidthOfParameter(int indexParam){
        return _Dmax.at(indexParam)+1;
    }
	
    double fitness(const std::vector<int> &parameters) {
		double f;
		double*x =new double[_Dsize];
		for(int i=0;i<_Dsize;i++)
			x[i]=parameters[i]-100;
		int nx=_Dsize;
        switch(func_num)
		{
			case 1:	
				sphere_func(x,&f,nx,OShift,M,0);
				f+=-1400.0;
				break;
			case 2:	
				ellips_func(x,&f,nx,OShift,M,1);
				f+=-1300.0;
				break;
			case 3:	
				bent_cigar_func(x,&f,nx,OShift,M,1);
				f+=-1200.0;
				break;
			case 4:	
				discus_func(x,&f,nx,OShift,M,1);
				f+=-1100.0;
				break;
			case 5:
				dif_powers_func(x,&f,nx,OShift,M,0);
				f+=-1000.0;
				break;
			case 6:
				rosenbrock_func(x,&f,nx,OShift,M,1);
				f+=-900.0;
				break;
			case 7:	
				schaffer_F7_func(x,&f,nx,OShift,M,1);
				f+=-800.0;
				break;
			case 8:	
				ackley_func(x,&f,nx,OShift,M,1);
				f+=-700.0;
				break;
			case 9:	
				weierstrass_func(x,&f,nx,OShift,M,1);
				f+=-600.0;
				break;
			case 10:	
				griewank_func(x,&f,nx,OShift,M,1);
				f+=-500.0;
				break;
			case 11:	
				rastrigin_func(x,&f,nx,OShift,M,0);
				f+=-400.0;
				break;
			case 12:	
				rastrigin_func(x,&f,nx,OShift,M,1);
				f+=-300.0;
				break;
			case 13:	
				step_rastrigin_func(x,&f,nx,OShift,M,1);
				f+=-200.0;
				break;
			case 14:	
				schwefel_func(x,&f,nx,OShift,M,0);
				f+=-100.0;
				break;
			case 15:	
				schwefel_func(x,&f,nx,OShift,M,1);
				f+=100.0;
				break;
			case 16:	
				katsuura_func(x,&f,nx,OShift,M,1);
				f+=200.0;
				break;
			case 17:	
				bi_rastrigin_func(x,&f,nx,OShift,M,0);
				f+=300.0;
				break;
			case 18:	
				bi_rastrigin_func(x,&f,nx,OShift,M,1);
				f+=400.0;
				break;
			case 19:	
				grie_rosen_func(x,&f,nx,OShift,M,1);
				f+=500.0;
				break;
			case 20:	
				escaffer6_func(x,&f,nx,OShift,M,1);
				f+=600.0;
				break;
			case 21:	
				cf01(x,&f,nx,OShift,M,1);
				f+=700.0;
				break;
			case 22:	
				cf02(x,&f,nx,OShift,M,0);
				f+=800.0;
				break;
			case 23:	
				cf03(x,&f,nx,OShift,M,1);
				f+=900.0;
				break;
			case 24:	
				cf04(x,&f,nx,OShift,M,1);
				f+=1000.0;
				break;
			case 25:	
				cf05(x,&f,nx,OShift,M,1);
				f+=1100.0;
				break;
			case 26:
				cf06(x,&f,nx,OShift,M,1);
				f+=1200.0;
				break;
			case 27:
				cf07(x,&f,nx,OShift,M,1);
				f+=1300.0;
				break;
			case 28:
				cf08(x,&f,nx,OShift,M,1);
				f+=1400.0;
				break;
			default:
				printf("\nError: There are only 28 test functions in this test suite!\n");
				f = 0.0;
				break;
		}
		delete[]x;
		return -f;
    }
};


#endif //P1_FUNC_H
