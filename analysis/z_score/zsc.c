#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<unistd.h>

char  **MatN, **MatL;
double *VecV;
int		Dm, Dn, Dc, Dk, Dh, *VecA, **MatA, **MatX, *VecY, *VecB, *VecC;


void readValue(char *fn1/* lbl.txt */)
{
	FILE *fp;
	int	  i, j, k, h;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	fscanf(fp, "%d %d %d", &Dm, &Dn, &Dc); 
	VecA = (int *)  malloc(sizeof(int)*Dm);
	MatA = (int **) malloc(sizeof(int *)*Dm);
	MatX = (int **) malloc(sizeof(int *)*Dm);
	
	for(i = 0; i < Dm; i++)
	{
		fscanf(fp, "%d", &VecA[i]); 
		MatA[i] = (int *) malloc(sizeof(int)*VecA[i]);
		MatX[i] = (int *) malloc(sizeof(int)*VecA[i]);
		
		for(j = 0; j < VecA[i]; j++)
		{ 
			fscanf(fp, "%d:%d", &k, &h);
			MatA[i][j] = k-1;
			MatX[i][j] = h;
		}
		
		while((j = getc(fp)) != EOF)
		{
			if(j == '\n') 
				break; 
		}
	}
	
	fclose(fp);
	printf("%d %d %d\n", Dm, Dn, Dc);
}


void readValue2(char *fn1/* uid.txt */)
{
	FILE *fp;
	int	  i, j, k, c[3];
	
	if((fp = fopen(fn1, "r")) == NULL)
	{ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	VecY = (int *) malloc(sizeof(int)*Dm);
	for(i = 0; i < Dm; i++)
	{
		fscanf(fp, "%d", &VecY[i]);  
		while((j = getc(fp)) != EOF)
		{
			if(j == '\n') 
				break; 
		}
	}
	
	fclose(fp);
	printf("%d\n", Dm);
//	for(i = 0; i < Dm; i++) printf("%f\n", VecY[i]);
}


void readValue3(char *fn1/* wid.txt */)
{
	FILE *fp;
	int	  i, j;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	MatL  = (char **) malloc(sizeof(char *)*Dn);
	for(i = 0; i < Dn; i++)
	{
		while((j = getc(fp)) != EOF)
		{
			if(j == ' ' || j == '\t') 
				break; 
		}
		
		MatL[i] = (char *) malloc(sizeof(char)*1023);
		for(j = 0; j < 1024; j++)
		{
			if((MatL[i][j] = getc(fp)) == ' ') 
				break;
		}
		MatL[i][j] = '\0'; 
		
		while((j = getc(fp)) != EOF)
		{
			if(j == '\n') 
				break; 
		}
	}
	
	fclose(fp);
	printf("%d\n", Dn);
}


void calValue(char *fn1/* important_word.txt */)
{
	FILE  *fp;
	int	   i, j, k;
	double p, v, y; 
	
	fp   = fopen(fn1, "w"); 
	VecB = (int *)    malloc(sizeof(int)*Dn);
	VecC = (int *)    malloc(sizeof(int)*Dn);
	VecV = (double *) malloc(sizeof(double)*Dn);
	
	for(j = 0; j < Dn; j++)
	{ 
		VecB[j] = 0;
		VecC[j] = 0; 
		VecV[j] = 0.0; 
	}
	
	for(i = 0, y = 0.0; i < Dm; i++)
	{
		for(j = 0; j < VecA[i]; j++) 
			VecB[MatA[i][j]] += 1; 
		
//		対象のカテゴリーではない場合
		if(VecY[i] != Dk) 
			continue; 
		
		for(j = 0; j < VecA[i]; j++) 
			VecC[MatA[i][j]] += 1; 
		
		y += 1.0; 
	}
	printf("%d\n", (int)y);
	
//	カイ二乗検定で重要語を数値化
	for(j = 0, v = y*(1.0-(y/Dm)); j < Dn; j++)
	{
		if((p = 1.0*VecB[j]/Dm) > 0.0) 
			VecV[j] = (VecC[j] - y * p) / sqrt(v * p * (1.0 - p)); 
	}
	
//	重要語を上位から Dh 個ファイルに記録
	for(k = 0; k < Dh; k++)
	{
		for(j = 0, v = 0.0; j < Dn; j++)
		{
//			既存の最大値 < 最大値 なら最大値を更新
			if(v < VecV[j])
			{ 
				v = VecV[j]; 
				i = j; 
			}
		}
		
		VecV[i] = 0.0; 
		//fprintf(fp, "%d %s %e (%d/%d)\n", k+1, MatL[i], v, VecC[i], VecB[i]);
		fprintf(fp,"%s\n",MatL[i]);
	}
	
//	重要語を下位から Dh 個ファイルに記録
	for(k = 0; k < Dh; k++)
	{
		for(j = 0, v = 0.0; j < Dn; j++)
		{
//			既存の最小値 < 最小値 なら最小値を更新
			if(VecV[j] < v)
			{ 
				v = VecV[j]; 
				i = j; 
			}
		}
//		printf("%d %s %e (%d/%d)\n", k+1, MatL[i], v, VecC[i], VecB[i]); 
		VecV[i] = 0.0; 
	}
	
	fclose(fp);
}


int	main(int argc, char **argv)
{
	readValue(argv[1]);
	Dk = atoi(argv[3]);
	readValue2(argv[2]);
	readValue3(argv[4]);
	Dh = 100; 
	calValue(argv[5]);
	return 0; 
}

/*
chcp 65001
cd reserch\AI\AI_models\knn\miss_analysis
gcc zsc.c -lm -o zsc
./zsc.exe data/lbl.txt data/new_uid.txt 6 data/wid.txt data/important_word_6.txt

*/
