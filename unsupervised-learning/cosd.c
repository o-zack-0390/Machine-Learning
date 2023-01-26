/*群平均法 ウォード法*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<unistd.h>

int	     Dm, Dn, Dc, *VecA, **MatA, *VecX, *VecY, *VecB, **MatB, *VecC, *VecD;
double **MatX, **MatY, *VecW, *VecZ, **MatE;


/* 引数はlbl.txt */
void readValue(char *fn1)
{
	FILE *fp;
	int	  i, j, k, h;
	
	if((fp = fopen(fn1, "r")) == NULL){ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	fscanf(fp, "%d %d %d", &Dm, &Dn, &h); 
	VecA = (int *)     malloc(sizeof(int)*Dm);
	MatA = (int **)    malloc(sizeof(int *)*Dm);
	MatX = (double **) malloc(sizeof(double *)*Dm);
	
	for(i = 0; i < Dm; i++)
	{
		fscanf(fp, "%d", &VecA[i]);
		MatA[i] = (int *)    malloc(sizeof(int)*VecA[i]);
		MatX[i] = (double *) malloc(sizeof(double)*VecA[i]);
		
		for(j = 0; j < VecA[i]; j++){ 
			fscanf(fp, "%d:%d", &k, &h);
			MatA[i][j] = k-1;
			MatX[i][j] = h;
		}
	}
	
	fclose(fp);
	printf("%d\n", Dm);
}


/* 引数はcls010v1.txt */
void readValue2(char *fn1)
{
	FILE *fp;
	int	  i;
	
	if((fp = fopen(fn1, "r")) == NULL){ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	VecC = (int *) malloc(sizeof(int)*Dm);
	fp   = fopen(fn1, "r"); 
	
	for(i = 0; i < Dm; i++) 
		fscanf(fp, "%d", &VecC[i]); 
	
	fclose(fp);
	printf("%d\n", Dm);
}


double	calValue(int x, int y)
{
	int	   k;
	double s, u, v, w; 
	
	for(k = 0, u = v = w = 0.0; k < Dn; k++){ 
		s  = MatY[x][k] + MatY[y][k]; 
		u += s * s; 
		v += MatY[x][k] * MatY[x][k]; 
		w += MatY[y][k] * MatY[y][k]; 
	}
	
	return( sqrt(v) + sqrt(w) - sqrt(u) ); 
}


void initData()
{
	int	   h, i, j, k;
	double v, w;
	VecW = (double *) malloc(sizeof(double)*Dn);
	
	for(j = 0; j < Dn; j++) 
		VecW[j] = 0.0;
	
	for(i = 0; i < Dm; i++) 
		for(j = 0; j < VecA[i]; j++) 
			VecW[MatA[i][j]] += 1.0;
		
	for(j = 0, v = log(1.0*Dm); j < Dn; j++) 
		if(VecW[j] > 0.0) 
			VecW[j] = v-log(1.0*VecW[j]);
		
	for(i = 0; i < Dm; i++)
	{
		for(j = 0; j < VecA[i]; j++) 
			MatX[i][j] *= VecW[MatA[i][j]];
		
		for(j = 0, v = 0.0; j < VecA[i]; j++) 
			v += MatX[i][j] * MatX[i][j];
		
		for(j = 0, v = 1.0/sqrt(v); j < VecA[i]; j++) 
			MatX[i][j] *= v;
		
	}
	
	for(i = Dc = 0; i < Dm; i++)
	{ 
		if(Dc < VecC[i]) 
			Dc = VecC[i]; 
		
		VecC[i]--; 
	}
	
	VecD = (int *)     malloc(sizeof(int)*Dc);
	MatY = (double **) malloc(sizeof(double *)*Dc);
	
	for(i = 0; i < Dc; i++)
	{ 
		VecD[i] = 0; 
		MatY[i] = (double *) malloc(sizeof(double)*Dn);
		
		for(j = 0; j < Dn; j++) 
			MatY[i][j] = 0.0; 
	}
	
	for(i = 0; i < Dm; i++)
	{ 
		k = VecC[i]; 
		VecD[k] += 1; 
		
		for(j = 0; j < VecA[i]; j++) 
			MatY[k][MatA[i][j]] += MatX[i][j];
	}
	
//	for(i = 0; i < Dc; i++) for(j = 0; j < Dn; j++) MatY[i][j] = MatY[i][j]/VecD[i]; 
	Dm   = Dc; 
	VecB = (int *) malloc(sizeof(int)*Dm);
	
	for(j = 0; j < Dm; j++) 
		VecB[j] = 0; 
	
	MatB = (int **)    malloc(sizeof(int *)*Dm);
	MatE = (double **) malloc(sizeof(double *)*Dm);
	
	for(i = 0; i < Dm; i++)
	{ 
		MatB[i]            = (int *)    malloc(sizeof(int)*Dm); 
		MatE[i]            = (double *) malloc(sizeof(double)*Dm);
		MatB[i][VecB[i]++] = i; 
		
		for(j = i+1; j < Dm; j++) 
			MatE[i][j] = calValue(i, j); 
	}
	
	VecX = (int *)    malloc(sizeof(int)*Dm);
	VecY = (int *)    malloc(sizeof(int)*Dm);
	VecZ = (double *) malloc(sizeof(double)*Dm);
	
//	printf("init\n");
}


void	calMST()
{
	int	   i, j, k, x, y; 
	double min; 
	
	for(i = 0; i < Dm-1; i++)
	{ 
		for(j = 0, min = -1.0; j < Dm-1; j++)
		{
			if(VecB[j] == 0) 
				continue;
			
			for(k = j+1; k < Dm; k++)
			{
				if(VecB[k] == 0) 
					continue; 
				
				if(min > 0 && MatE[j][k] >= min) 
					continue; 
				
				x   = j; 
				y   = k; 
				min = MatE[j][k]; 
			}
		}
//		printf("%d %d %d %e\n", i+1, x+1, y+1, min); 
//		for(k = 0; k < Dn; k++) MatY[x][k] = (VecD[x]*MatY[x][k])+(VecD[y]*MatY[y][k]); 

		for(k = 0, VecD[x] += VecD[y]; k < Dn; k++) 
			MatY[x][k] += MatY[y][k]; 
		
		for(k = 0; k < VecB[y]; k++) 
			MatB[x][VecB[x]++] = MatB[y][k]; 
		
		VecB[y] = 0; 
		VecX[i] = x; 
		VecY[i] = y; 
		VecZ[i] = min; 
		
		if(i == Dm-2) 
			break; 
		
		for(j = 0; j < x; j++) 
			if(VecB[j] > 0) 
				MatE[j][x] = calValue(j, x); 
		
		for(j = x+1; j < Dm; j++) 
			if(VecB[j] > 0) 
				MatE[x][j] = calValue(x, j); 
	}
}


/* 引数はcos010v1.txt */
void printValue(char *fn1)
{
	FILE *fp;
	int	  i;
	
	fp = fopen(fn1, "w");
	fprintf(fp, "%d %d\n", Dm, Dm-1); 
	
	for(i = 0; i < Dm-1; i++)
		fprintf(fp, "%d %d %e\n", VecX[i]+1, VecY[i]+1, VecZ[i]); 
	
	fclose(fp); 
}


int	main(int argc, char **argv)
{
	readValue(argv[1]);//lbl.txt
	readValue2(argv[2]);//cls010v1.txt
	initData();
	calMST();
	printValue(argv[3]);//cos010v1.txt
	return 0; 
}

/*
実行パス
cd reserch\AI\AI_models\ward
gcc cosd.c -lm -o cosd
./cosd data/lbl.txt data/cls010v1.txt result/cos010v1.txt
*/
