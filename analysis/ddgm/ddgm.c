/*htmlにダイアグラムを表示する*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<unistd.h>
#include	<sys/times.h>

struct tms tms_buffer;
double ST, UT, st, ut, tt;
int hour, minute;
double second;
long ticks;

#define INIT_TIME \
ticks = sysconf(_SC_CLK_TCK);\
times (&tms_buffer);\
ST = (double) tms_buffer.tms_stime ;\
UT = (double) tms_buffer.tms_utime 

#define FORMAT_TIME_AND_PRINT_TIME \
times (&tms_buffer);\
st = ((double) tms_buffer.tms_stime - ST)/ ticks;\
ut = ((double) tms_buffer.tms_utime - UT)/ ticks;\
tt = st + ut;\
hour = (int) (tt / 3600.0);\
minute = (int) (tt / 60.0) - hour * 60;\
second =  (tt) - 60 * (int) (tt / 60.0);\
printf("(%dh %dm %fs)\n", hour, minute, second);

char	**MatF; 
int		Dm, Dn, Ds, *VecX, *VecC, *VecY, *VecR, *VecL, *VecS, **MatL;
double	*VecZ, **MatW, ValZ;

void readValue(char *fn1)
{
	FILE *fp;
	int   i, c;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	fscanf(fp, "%d %d", &Dm, &Dn);
	VecX = (int *)    malloc(sizeof(int)*Dn);
	VecY = (int *)    malloc(sizeof(int)*Dn);
	VecZ = (double *) malloc(sizeof(double)*Dn);
	
	for(i = 0; i < Dn; i++)
	{
		fscanf(fp, "%d %d %lf", &VecX[i], &VecY[i], &VecZ[i]);
		VecX[i]--; 
		VecY[i]--; 
		
		if(VecZ[i] > 0) 
			VecZ[i] = sqrt(VecZ[i]); 
	}
	
	fclose(fp);
	printf("%d %d\n", Dm, Dn);
//	for(i = 0; i < Dn; i++) printf("%d %d %e\n", VecX[i], VecY[i], VecZ[i]);
}


void readUid(char *fn1)
{
	FILE *fp;
	char  c; 
	int	  i, j, k;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	MatF = (char **) malloc(sizeof(char *)*Dm);
	
	for(i = 0; i < Dm; i++)
	{
		MatF[i] = (char *) malloc(sizeof(char)*1023);
		
		for(k = j = 0; k < 6; k++)
		{ 
			for( ; j < 1024; j++)4
			{
				if((MatF[i][j] = getc(fp)) == '\n') 
					break; 
			}
			
			MatF[i][j++] = ',';
		}
		
		MatF[i][j-1] = '\0'; 
	}
	
	fclose(fp);
	printf("%d\n", Dm);
}


void initData()
{
	int		j;
	
	VecC = (int *) malloc(sizeof(int)*Dm);
	for(j = 0; j < Dm; j++) 
		VecC[j] = -1; 
	
	VecR = (int *) malloc(sizeof(int)*Dn);
	VecL = (int *) malloc(sizeof(int)*Dn);
	VecS = (int *) malloc(sizeof(int)*Dn);
	
	MatW = (double **) malloc(sizeof(double *)*(Dm+3*Dn));
	for(j = 0; j < Dm+3*Dn; j++) 
		MatW[j] = (double *) malloc(sizeof(double)*2); 
	
	MatL = (int **) malloc(sizeof(int *)*(4*Dn));
	for(j = 0; j < 4*Dn; j++) 
		MatL[j] = (int *) malloc(sizeof(int)*2);; 
	
	printf("init OK\n");
}


void calValue()
{
	int	   i, j, k, u, v, x, y, z, cnt;
	double val;
	
	for(i = 0; i < Dn; i++)
	{ 
		x = VecX[i]; 
		y = VecY[i]; 
		u = VecC[x]; 
		v = VecC[y]; 
		VecC[x] = i;
		VecC[y] = i; 
/*
		for(k = 0; k < Dm; k++) 
			printf("%d ", VecC[k]); 
		printf("\n");
		
		if(u == v && u != -1) 
			printf("**Error%d %d %d %d %d\n", i, x, y, u, v); 
		printf("%d %d %d %d %d %d\n", i+1, x, y, u, v, Dc); 
*/
		if(u == -1 && v == -1)
		{ 
			VecL[i] = -(x+1); 
			VecR[i] = -(y+1); 
			VecS[i] = 2; 
		} 
		
		else if(u == -1 && v >= 0)
		{ 
			VecL[i] = v; 
			VecR[i] = -(x+1); 
			VecS[i] = 1+VecS[v];
			
			for(k = 0; k < Dm; k++)
			{
				if(VecC[k] == v) 
					VecC[k] = i; 
			}
		} 
		
		else if(u >= 0 && v == -1)
		{ 
			VecL[i] = u; 
			VecR[i] = -(y+1); 
			VecS[i] = VecS[u]+1; 
			for(k = 0; k < Dm; k++)
			{
				if(VecC[k] == u) 
					VecC[k] = i;
			}		
		} 
		
		else if(u != v)
		{ 
			if(VecS[u] >= VecS[v])
			{ 
				VecL[i] = u; 
				VecR[i] = v; 
			}
			
			else
			{
				VecL[i] = v; 
				VecR[i] = u;
			}
			
			VecS[i] = VecS[u] + VecS[v]; 
			
			for(k = 0; k < Dm; k++)
			{
				if(VecC[k] == u || VecC[k] == v) 
					VecC[k] = i; 
			}
		} 
	}
	
	printf("cal OK\n");
//	for(i = Dn-1; i >= 0; i--) printf("%d %d %d %d\n", i, VecL[i], VecR[i], VecS[i]); 
}


double	plotValue(int i, int s, int e)
{
	int	   h, j, k;
	double z, a, b; 
	
	z = VecZ[i]; 
	if((j = VecL[i]) >= 0)
	{
		a = plotValue(j, s, s+VecS[j]-1); 
		j = Dm+(3*j)+2; 
	}
	
	else
	{ 
		j          = -j-1;
		MatW[j][0] = a = s; 
		MatW[j][1] = z - ValZ; 
	}
	
	if((k = VecR[i]) >= 0)
	{
		b = plotValue(k, e-VecS[k]+1, e); 
		k = Dm+(3*k)+2; 
	}
	
	else
	{ 
		k          = -k-1; 
		MatW[k][0] = b = e; 
		MatW[k][1] = z - 0.5*ValZ; 
	}
	
	h             = Dm+(3*i); 
	MatW[h][0]    = a; 
	MatW[h][1]    = z;
	MatW[h+1][0]  = b; 
	MatW[h+1][1]  = z;
	MatW[h+2][0]  = (a+b)/2.0;
	MatW[h+2][1]  = z;
	MatL[Ds][0]   = h; 
	MatL[Ds++][1] = j;
	MatL[Ds][0]   = h; 
	MatL[Ds++][1] = h+2;
	MatL[Ds][0]   = h+1; 
	MatL[Ds++][1] = k;
	MatL[Ds][0]   = h+1; 
	MatL[Ds++][1] = h+2;
	
	return((a+b)/2.0); 
}


void printValue(char *fn1, int width)
{
	static char	*name[15] = {"#808000", "#ffff00", "#ff00ff", "#c0c0c0", "#00ffff", "#00ff00", "#ff0000", "#808080", "#0000ff", "#008000", "#800080", "#000000", "#000080", "#008080", "#800000"};
	FILE	    *fp;
	int		     i, j, k, x = 1250, y = 650;
	double	     v, w; 
	
	for(i = 0; i < Dm+3*Dn; i++)
	{
		if(i == 0) 
			v = w = MatW[i][0]; 
		
		else if(v > MatW[i][0]) 
			v = MatW[i][0]; 
		
		else if(w < MatW[i][0]) 
			w = MatW[i][0];
	}
	
	for(i = 0, w = w-v; i < Dm+3*Dn; i++) 
		MatW[i][0] = (0.98*x*((MatW[i][0]-v)/w))+(0.01*x);
	
	for(i = 0; i < Dm+3*Dn; i++) 
		MatW[i][1] = -MatW[i][1]; 
	
	for(i = 0; i < Dm+3*Dn; i++)
	{
		if(i == 0) 
			v = w = MatW[i][1]; 
		
		else if(v > MatW[i][1]) 
			v = MatW[i][1]; 
		
		else if(w < MatW[i][1]) 
			w = MatW[i][1];
	}
	
	for(i = 0, w = w-v; i < Dm+3*Dn; i++) 
		MatW[i][1] = (0.98*y*((MatW[i][1]-v)/w))+(0.01*y);
	
	fp = fopen(fn1, "w"); 
	fprintf(fp, "<?xml version=\"1.0\" standalone=\"no\"?>\n"); 
	fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n");
	fprintf(fp, "\"http://www.w3.org/TR/SVG/DTD/svg10.dtd\">\n\n");
	fprintf(fp, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", x, y);
	
	for(i = 0; i < 4*Dn; i++)
	{
		j = MatL[i][0]; 
		k = MatL[i][1];  
		fprintf(fp, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"%d\"/>\n", MatW[j][0], MatW[j][1], MatW[k][0], MatW[k][1], width); 
	}
	
	for(i = 0; i < Dm+3*Dn; i++)
	{ 
		if(i < Dm)
		{ 
			fprintf(fp, "<a xlink:href=\"\" xlink:title=\"%s\" xlink:show=\"new\">\n", MatF[i]); 
			fprintf(fp, "<circle cx=\"%f\" cy=\"%f\" r=\"6\" fill=\"%s\" opacity=\"1\"  stroke=\"%s\" stroke-opacity=\"1\" stroke-width=\"2\"/>\n", MatW[i][0], MatW[i][1], name[i%15], name[(i/15)%15]); 
		}
		
		else
		{ 
			fprintf(fp, "<a xlink:href=\"\" xlink:title=\"\" xlink:show=\"new\">\n"); 
			fprintf(fp, "<circle cx=\"%f\" cy=\"%f\" r=\"0.1\" fill=\"\" opacity=\"1\" />\n", MatW[i][0], MatW[i][1]); 
		}
		
		fprintf(fp, "</a>\n");
	}
	
	fprintf(fp, "</svg>\n");
	fclose(fp); 
}


int main(int argc, char **argv)
{
  int width; 
  INIT_TIME;
	readValue(argv[1]);
	readUid(argv[2]);
	initData();
	calValue();
	Ds = 0; ValZ = 0.01*(VecZ[Dn-1] - VecZ[0]); 
//	printf("%e %e %e\n", VecZ[0], VecZ[Dn-1], ValZ); 
	plotValue(Dn-1, 1, Dm); 
	printf("plot OK\n");
	width = atoi(argv[3]);
	printValue(argv[4], width);
  FORMAT_TIME_AND_PRINT_TIME
	return 0; 
}
