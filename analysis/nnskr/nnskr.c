#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

char	*VecN, **MatN;
int		Dm, Dn, Dk, *VecA, **MatA, *VecB, **MatB, *VecC, *VecI, *VecJ;
double	**MatX, **MatY, *VecX, *VecZ, *VecV, *VecW;

void    readValue(char *fn1)
{
	FILE		*fp;
	int		i, j, k;
	double		v;
	if((fp = fopen(fn1, "r")) == NULL) { printf("Unknown File = %s\n", fn1); exit(1);}
	fscanf(fp, "%d %d %d", &Dm, &Dn, &k); 
	VecA = (int *) malloc(sizeof(int)*Dm);
	MatA = (int **) malloc(sizeof(int *)*Dm);
	MatX = (double **) malloc(sizeof(double *)*Dm);
	for(i = 0; i < Dm; i++){
		fscanf(fp, "%d", &VecA[i]);
		MatA[i] = (int *) malloc(sizeof(int)*VecA[i]);
		MatX[i] = (double *) malloc(sizeof(double)*VecA[i]);
		for(j = 0; j < VecA[i]; j++){ 
			fscanf(fp, "%d:%lf", &k, &v);
			MatA[i][j] = k-1;
			MatX[i][j] = v;
		}
	}
	fclose(fp);
	printf("%d %d\n", Dm, Dn);
}
void    readValue2(char *fn1)
{
	FILE	*fp;
	int		i, j, k;
	if((fp = fopen(fn1, "r")) == NULL){ printf("Unknown File = %s\n", fn1); exit(1);}
	VecC = (int *) malloc(sizeof(int)*Dm);
	VecN = (char *) malloc(sizeof(char)*4096);
	MatN = (char **) malloc(sizeof(char *)*Dm);
	for(i = 0; i < Dm; i++){
		fscanf(fp, "%d ", &VecC[i]);
		for(j = 0; j < 4096; j++) if((VecN[j] = getc(fp)) == '\n') break; 
		MatN[i] = (char *) malloc(sizeof(char)*(j+1));
		for(k = 0; k < j; k++) MatN[i][k] = VecN[k]; 
		MatN[i][j] = '\0'; 
	}
	fclose(fp);
	printf("%d\n", Dm);
}
void	mySort(int *ip, double *vp, int n)
{
	int		i, j, k, h;
	if((h = n/2) == 0) return;
	mySort(&ip[0], &vp[0], h);
	mySort(&ip[h], &vp[h], n-h); 
	for(i = j = 0, k = h; i < n; i++)
		if(j < h && (k == n || vp[j] <= vp[k])){ VecJ[i] = ip[j]; VecW[i] = vp[j++]; } 
		else { VecJ[i] = ip[k]; VecW[i] = vp[k++]; }
	for(i = 0; i < n; i++){ ip[i] = VecJ[i]; vp[i] = VecW[i]; }
}
void	initData()
{
	int		i, j;
	VecB = (int *) malloc(sizeof(int)*Dn);
	MatB = (int **) malloc(sizeof(int *)*Dn);
	MatY = (double **) malloc(sizeof(double *)*Dn);
	for(j = 0; j < Dn; j++) VecB[j] = 0; 
	for(i = 0; i < Dm; i++) for(j = 0; j < VecA[i]; j++) VecB[MatA[i][j]]++;
	for(j = 0; j < Dn; j++){ 
		MatB[j] = (int *) malloc(sizeof(int)*VecB[j]); 
		MatY[j] = (double *) malloc(sizeof(double)*VecB[j]); 
	}
	VecI = (int *) malloc(sizeof(int)*Dm);
	VecJ = (int *) malloc(sizeof(int)*Dm);
	VecV = (double *) malloc(sizeof(double)*Dm);
	VecW = (double *) malloc(sizeof(double)*Dm);
	VecZ = (double *) malloc(sizeof(double)*Dm);
	VecX = (double *) malloc(sizeof(double)*Dn);
}
void	calTFIDF()
{
	int		i, j;
	double	v;
	for(j = 0, v = log(1.0*Dm); j < Dn; j++) VecX[j] = v;
	for(j = 0; j < Dn; j++) VecX[j] -= log(1.0*VecB[j]);
	for(i = 0; i < Dm; i++){
		for(j = 0; j < VecA[i]; j++) MatX[i][j] *= VecX[MatA[i][j]];
		for(j = 0, v = 0.0; j < VecA[i]; j++) v += MatX[i][j]*MatX[i][j];
		for(j = 0, v = 1.0/sqrt(v); j < VecA[i]; j++) MatX[i][j] *= v;
	}
}
void	calInverted()
{
	int		i, j, k;
	for(j = 0; j < Dn; j++) VecB[j] = 0; 
	for(i = 0; i < Dm; i++)
		for(j = 0; j < VecA[i]; j++){
			k = MatA[i][j]; 
			MatB[k][VecB[k]] = i;
			MatY[k][VecB[k]++] = MatX[i][j];
		}
}
void	calValue()
{
	FILE	*fp;
	int		i, j, k, h;
	double	v;
	for(i = 0; i < Dm; i++){ VecI[i] = i; VecV[i] = 0.0; }
	for(i = 0; i < Dm; i++){
		for(j = 0; j < Dm; j++) VecZ[j] = 0.0; 
		for(j = 0; j < VecA[i]; j++){
			h = MatA[i][j]; v = MatX[i][j]; 
			for(k = 0; k < VecB[h]; k++) VecZ[MatB[h][k]] += v*MatY[h][k]; 
		}
		for(k = 0; k < Dk; k++){ 
			for(j = h = 0, v = 0.0; j < Dm; j++) if(j != i && VecZ[j] > v){ v = VecZ[j]; h = j;}
			VecV[i] += v; VecZ[h] = 0.0; 
		}
		VecV[i] = VecV[i]/Dk; 
	}
}
void	printValue(char *fn1)
{
	FILE	*fp;
	int		h, i;
	mySort(VecI, VecV, Dm); 
	fp = fopen(fn1, "w"); 
	for(i = 0; i < Dm; i++){ h = VecI[i]; fprintf(fp, "%d %d %e %s\n", i+1, h+1, VecV[i], MatN[h]); }
	fclose(fp); 
}
void	main(int argc, char **argv)
{
	readValue(argv[1]);
	readValue2(argv[2]);
	initData(); 
	calTFIDF(); 
	calInverted(); 
	Dk = atoi(argv[3]);
	calValue(); 
	printValue(argv[4]); 
}
