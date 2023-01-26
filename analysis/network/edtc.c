/* コーディングを研究室で指定されてる書き方に統一 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

char	**MatN, **MatL;
int		Dm, Dn, Dc, Dh, Dk, *VecA, **MatA, **MatX, *VecY, *VecB, *VecC, *VecI, *VecJ, **MatS, **MatT;
double	*VecV, *VecW, *VecX;

void    readValue1(char *fn1)
{
	FILE		*fp;
	int		i, j, k, h;
	if((fp = fopen(fn1, "r")) == NULL) { printf("Unknown File = %s\n", fn1); exit(1);}
	fscanf(fp, "%d %d %d", &Dm, &Dn, &Dc); 
	VecA = (int *) malloc(sizeof(int)*Dm);
	MatA = (int **) malloc(sizeof(int *)*Dm);
	MatX = (int **) malloc(sizeof(int *)*Dm);
	for(i = 0; i < Dm; i++){
		fscanf(fp, "%d", &VecA[i]); 
		MatA[i] = (int *) malloc(sizeof(int)*VecA[i]);
		MatX[i] = (int *) malloc(sizeof(int)*VecA[i]);
		for(j = 0; j < VecA[i]; j++){ 
			fscanf(fp, "%d:%d", &k, &h);
			MatA[i][j] = k-1;
			MatX[i][j] = h;
		}
		while((j = getc(fp)) != EOF) if(j == '\n') break; 
	}
	fclose(fp);
	printf("%d %d %d\n", Dm, Dn, Dc);
}
void	readValue2(char *fn1)
{
	FILE	*fp;
	int		i, j;
	if((fp = fopen(fn1, "r")) == NULL){ printf("Unknown File = %s\n", fn1); exit(1);}
	MatL = (char **) malloc(sizeof(char *)*Dn);
	for(i = 0; i < Dn; i++){
		while((j = getc(fp)) != EOF) if(j == ' ' || j == '\t') break; 
		MatL[i] = (char *) malloc(sizeof(char)*1023);
		for(j = 0; j < 1024; j++) if((MatL[i][j] = getc(fp)) == ' ') break; 
		MatL[i][j] = '\0'; 
		while((j = getc(fp)) != EOF) if(j == '\n') break; 
	}
	fclose(fp);
	printf("%d\n", Dn);
}
void    readValue3(char *fn1)
{
	FILE	*fp;
	int		i, j;
	if((fp = fopen(fn1, "r")) == NULL){ printf("Unknown File = %s\n", fn1); exit(1);}
	VecY = (int *) malloc(sizeof(int)*Dm);
	for(i = 0; i < Dm; i++){ 
		fscanf(fp, "%d", &VecY[i]);  
		while((j = getc(fp)) != EOF) if(j == '\n') break; 
	}
	fclose(fp);
	printf("%d\n", Dm);
//	for(i = 0; i < Dm; i++) printf("%f\n", VecY[i]);
}
void	initValue()
{
	int		i; 
	VecB = (int *) malloc(sizeof(int)*Dn);
	VecC = (int *) malloc(sizeof(int)*Dn);
	VecI = (int *) malloc(sizeof(int)*Dm);
	VecJ = (int *) malloc(sizeof(int)*Dm);
	VecV = (double *) malloc(sizeof(double)*Dn);
	VecW = (double *) malloc(sizeof(double)*Dn);
	VecX = (double *) malloc(sizeof(double)*Dn);
	for(i = Dc = 0; i < Dm; i++){ VecI[i] = i; if(VecY[i] == Dh) Dc += 1; }
	MatS = (int **) malloc(sizeof(int *)*Dn);
	MatT = (int **) malloc(sizeof(int *)*Dn);
}
double	calScore(int k)
{
	int		h, i, j, p, q, r, s, n;
	double	u, v, w, x, z; 
	for(h = MatS[k][0], p = q = 0; h < MatS[k][1]; h++) if(VecY[VecI[h]] == Dh) p += 1; else q += 1; 
	MatS[k][5] = p; 
	if(p == 0 || q == 0){ VecW[k] = 0.0; return(0.0); }
	for(j = 0; j < Dn; j++) VecB[j] = VecC[j] = 0; 
	for(h = MatS[k][0]; h < MatS[k][1]; h++) 
		for(i = VecI[h], j = 0; j < VecA[i]; j++){ n = MatA[i][j]; VecB[n] += 1; if(VecY[i] == Dh) VecC[n] += 1; }
	n = MatS[k][1]-MatS[k][0]; v = (n*log(1.0*n))-(p*log(1.0*p))-(q*log(1.0*q)); VecW[k] = v; 
	for(j = 0, x = 0.0; j < Dn; j++){ 
		if((r = VecB[j]) == 0 || (s = (n-r)) == 0) continue; 
		u = v-(r*log(1.0*r))-(s*log(1.0*s)); if((w = VecC[j]) > 0) u += w*log(w); 
		if((z = (p-w)) > 0) u += z*log(z); if((z = (r-w)) > 0) u += z*log(z); if((z = ((n+w)-(p+r))) > 0) u += z*log(z);
		if(u > x){ x = u; MatS[k][3] = j; if(n*w >= p*r) MatS[k][4] = 0; else MatS[k][4] = 1; }
	}
	return(x); 
}
int		calSprit(int k)
{	
	int		h, i, j, r, s;
	for(h = r = s = MatS[k][0]; h < MatS[k][1]; h++){
		for(i = VecI[h], j = 0; j < VecA[i]; j++) if(MatA[i][j] == MatS[k][3]) break; 
		if(j < VecA[i]) VecI[r++] = i; else VecJ[s++] = i; 
	}
	for(h = r, j = MatS[k][0]; h < MatS[k][1]; h++, j++) VecI[h] = VecJ[j]; 
	return(r); 
}
void	calValue()
{
	int		g, h, i, j, k, r; 
	double	v; 
	MatS[0] = (int *) malloc(sizeof(int)*7);
	MatS[0][0] = 0; MatS[0][1] = Dm; MatS[0][2] = 0; MatS[0][6] = 0; 
	VecV[0] = calScore(0); 
	for(k = 1; k < Dk; k++){
		for(i = g = h = r = 0; i < k; i++) if(MatS[i][6] == 0){ g += MatS[i][1]-MatS[i][0]; h += MatS[i][5]; } else r += MatS[i][1]-MatS[i][0]-MatS[i][5];
		for(i = j = 0; i < k; i++) if(VecV[i] > VecV[j]) j = i; 
		if(VecV[j] < 5.4135) break; 
		for(i = 0, v = 0.0; i < k; i++) v += VecW[i]; 
		printf("%d %d %e %e %d %d %e %e\n", k, j+1, VecV[j], v, h, g, 2.0*h/(g+Dc), 1.0*(h+r)/Dm); VecX[k-1] = 1.0*(h+r)/Dm; 
		MatT[k-1] = (int *) malloc(sizeof(int)*6); 
		MatT[k-1][0] = j; MatT[k-1][1] = k; MatT[k-1][2] = MatS[j][2]; 
		MatT[k-1][3] = MatS[j][3]; MatT[k-1][4] = MatS[j][1]-MatS[j][0]; MatT[k-1][5] = MatS[j][5]; 
		r = calSprit(j); 
		MatS[k] = (int *) malloc(sizeof(int)*7);
		MatS[k][1] = MatS[j][1]; MatS[j][1] = MatS[k][0] = r; MatS[j][2]++; MatS[k][2] = MatS[j][2];
		MatS[j][6] = MatS[j][4]; MatS[k][6] = 1-MatS[j][4]; 
		VecV[j] = calScore(j); VecV[k] = calScore(k);  
	}
	Dk = k; 
}
void	printValue1(char *fn1)
{
	static char *name[] = {"POS", "NEG"}; 
	FILE	*fp;
	int		i; 
//	printf("%s\n", fn1); 
	fp = fopen(fn1, "w");
	for(i = 0; i < Dk; i++) fprintf(fp, "%d %s(%d/%d)\n", MatS[i][6]+1, name[MatS[i][6]], MatS[i][5], MatS[i][1]-MatS[i][0]);
	fclose(fp); 
}
void	printValue2(char *fn1)
{
	FILE	*fp;
	int		i, k; 
	for(i = k = 0; i < Dk-1; i++) if(MatT[i][2] > k) k = MatT[i][2]; 
	fp = fopen(fn1, "w");
	fprintf(fp, "%d %d\n", Dk, Dk-1); 
	for(i = Dk-2; i >= 0; i--) fprintf(fp, "%d %d %d %d %s(%d/%d)\n", MatT[i][0]+1, MatT[i][1]+1, k-MatT[i][2], i+1, MatL[MatT[i][3]], MatT[i][5], MatT[i][4]);
	fclose(fp); 
}
int		main(int argc, char **argv)
{
	readValue1(argv[1]);
	readValue2(argv[2]);
	readValue3(argv[3]);
	Dh = atoi(argv[4]);
	Dk = atoi(argv[5]);
	initValue();
	calValue();
	printValue1(argv[6]);
	printValue2(argv[7]);
	return 0; 
}
