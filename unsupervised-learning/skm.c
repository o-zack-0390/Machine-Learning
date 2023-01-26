/*k - means*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

char   **MatN, *VecN; 
int      N, M, K, *VecA, **MatA, *VecC, *VecE, *VecF;
double **MatX, *VecV, *VecW, **MatW, **MatM;

/* 引数はlbl.txt */
void readValue(char *fn1)
{
	FILE *fp;
	int	  i, j, k, h;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	fscanf(fp, "%d %d %d", &N, &M, &k); 
	VecA = (int *)     malloc(sizeof(int)*N);
	MatA = (int **)    malloc(sizeof(int *)*N);
	MatX = (double **) malloc(sizeof(double *)*N);
	
	for(i = 0; i < N; i++)
	{
		fscanf(fp, "%d", &VecA[i]);
		MatA[i] = (int *)    malloc(sizeof(int)*VecA[i]);
		MatX[i] = (double *) malloc(sizeof(double)*VecA[i]);
		
		for(j = 0; j < VecA[i]; j++)
		{ 
			fscanf(fp, "%d:%d", &k, &h);
			MatA[i][j] = k-1;
			MatX[i][j] = h;
		}
		
	}
	
	fclose(fp);
	printf("%d %d\n", N, M);
}


/* 引数はwid.txt */
void readName(char *fn1)
{
	FILE *fp;
	int   i, j, k;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{
		printf("Unknown File = %s\n", fn1);
		exit(1);
	}
	
	VecN = (char *)  malloc(sizeof(char)*4096);
	MatN = (char **) malloc(sizeof(char *)*M);
	
	for(i = 0; i < M; i++)
	{
		while((j = getc(fp)) != EOF)
		{
			if(j == ' ')
				break; 
		}
			
		for(j = 0; j < 4096; j++)
		{
			if((VecN[j] = getc(fp)) == ' ')
				break; 
		}
			
		MatN[i] = (char *) malloc(sizeof(char)*(j+1));
		for(k = 0; k < j; k++)
			MatN[i][k] = VecN[k]; 
		MatN[i][j] = '\0'; 
		
		while((j = getc(fp)) != EOF)
		{
			if(j == '\n')
				break; 
		}
	}
	
	fclose(fp);
	printf("%d\n", M);
}


void initData()
{
	int    i, j, k, h;
	double v;
	VecW = (double *) malloc(sizeof(double)*M);
	
	for(j = 0; j < M; j++) 
		VecW[j] = 0.0;
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < VecA[i]; j++) 
			VecW[MatA[i][j]] += 1.0;
	}
		
	for(j = 0, v = log(1.0*N); j < M; j++)
	{
		if(VecW[j] > 0.0)
			VecW[j] = v - log(VecW[j]);
	}
		
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < VecA[i]; j++) 
			MatX[i][j] *= VecW[MatA[i][j]];
		
		for(j = 0, v = 0.0; j < VecA[i]; j++) 
			v += MatX[i][j]*MatX[i][j];
		
		if(v == 0.0)
		{ 
			printf("error1 %d\n", i+1); 
			continue; 
		}
		
		for(j = 0, v = 1.0/sqrt(v); j < VecA[i]; j++)
			MatX[i][j] *= v;
		
	}
	
	MatW = (double **) malloc(sizeof(double *)*K);
	MatM = (double **) malloc(sizeof(double *)*K);
	VecC = (int *)     malloc(sizeof(int)*N); 
	
	for(i = 0; i < N; i++) 
		VecC[i] = i; 
	
	for(k = 0; k < K; k++)
	{
		MatW[k] = (double *) malloc(sizeof(double)*M);
		MatM[k] = (double *) malloc(sizeof(double)*M);
		
		for(j = 0; j < M; j++) 
			MatW[k][j] = 0.0;
		
		i       = rand()%(N-k); 
		h       = VecC[i]; 
		VecC[i] = VecC[N-k-1]; 
		
		for(j = 0; j < VecA[h]; j++) 
			MatW[k][MatA[h][j]] = MatX[h][j];
	}
	
	for(i = 0; i < N; i++) 
		VecC[i] = -1; 
	
	VecV = (double *) malloc(sizeof(double)*N);
	VecE = (int *)    malloc(sizeof(int)*K); 
	VecF = (int *)    malloc(sizeof(int)*K); 
	
	for(k = 0; k < K; k++) 
		VecF[k] = 0; 
	
	printf("init\n");
}


int	calValue(double *val)
{
	int    f, g, h, i, j, k;
	double w, max;
	
	for(k = 0; k < K; k++)
	{ 
		VecE[k] = VecF[k]; 
		VecF[k] = 1; 
	}
	
	for(k = 0; k < K; k++)
	{
		for(j = 0; j < M; j++) 
			MatM[k][j] = 0.0;
	}
		
	for(i = 0, *val = 0.0, f = 1; i < N; i++)
	{
		if((g = VecE[VecC[i]]) == 1)
		{ 
			h   = VecC[i]; 
			max = VecV[i]; 
		} 
		
		else
		{ 
			h   = -1; 
			max = -1.0; 
		}
		
		for(k = 0; k < K; k++)
		{ 
			if(g == 1 && VecE[k] == 1) 
				continue; 
			
			for(j = 0, w = 0.0; j < VecA[i]; j++) 
				w += MatX[i][j]*MatW[k][MatA[i][j]]; 
			
			if(w > max)
			{ 
				max = w; 
				h   = k;
			}
		}
		
		*val   += max; 
		VecV[i] = max; 
		
		if(VecC[i] != h)
		{ 
			VecF[VecC[i]] = 0;
			VecF[h]       = 0; 
			VecC[i]       = h; 
			f             = 0;
		}
		
		for(j = 0; j < VecA[i]; j++) 
			MatM[h][MatA[i][j]] += MatX[i][j];
	}
	
	for(k = 0; k < K; k++)
	{
		for(j = 0, w = 0.0; j < M; j++) 
			w += MatM[k][j] * MatM[k][j]; 
		
		if(w == 0.0)
		{ 
			printf("error2 %d\n", k+1); 
			continue; 
		}
		
		for(j = 0, w = 1.0/sqrt(w); j < M; j++) 
			MatW[k][j] = w * MatM[k][j];
	}
	
	if(f == 1) 
		return 1;
	
	return 0;
}


/* 引数はres010v1.txt */
void printValue(char *fn1)
{
	FILE	*fp;
	int     h, i, j, k, s;
	double  v, w; 
	
	for(k = 0; k < K; k++) 
		VecE[k] = 0; 
	
	for(i = 0; i < N; i++) 
		VecE[VecC[i]]++; 
	
	fp = fopen(fn1, "w"); 
	
	for(k = 0; k < K; k++)
	{
		fprintf(fp, "k:%2d (%d)\n", k+1, VecE[k]+1); 
		
		if(VecE[k] == 0) 
			continue; 
		
		for(i = 0; i < 100; i++)
		{ 
			for(j = 0, v = 0.0; j < M; j++)
			{
				if((w = MatW[k][j]) > v)
				{ 
					v = w; 
					s = j; 
				}
			}
				
			if(v == 0.0) 
				break; 
			
			fprintf(fp, "%d %s(%.2f)\n", i+1, MatN[s], v);
			MatW[k][s] = 0.0; 
		}
	}
	
	fclose(fp); 
}


/* 引数はcls010v1.txt */
void printValue2(char *fn1)
{
	FILE *fp;
	int	  i;
	fp = fopen(fn1, "w"); 
	for(i = 0; i < N; i++)
		fprintf(fp, "%d\n", VecC[i]+1);
	fclose(fp); 
}


int main(int argc, char **argv)
{
	int    i;
	double v;
	
	K = atoi(argv[1]);//9
	readValue(argv[2]);//lbl.txt
	readName(argv[3]);//wid.txt
	initData(); 
	
	for(i = 0; i < 1000; i++)
	{	
		if(calValue(&v) == 1) 
			break; 
		
		printf("No.%d %e\n", i, v);
	}	
	printf("No.%d %e\n", i, v);
	
	printValue2(argv[4]);//res010v1.txt
	printValue(argv[5]);//cls010v1.txt
	return 0;
}

/*
実行パス
cd reserch\AI\AI_models\k-means
gcc skm.c -lm -o skm
./skm 9 data/lbl.txt data/wid.txt result/cls010v1.txt result/res010v1.txt
*/
