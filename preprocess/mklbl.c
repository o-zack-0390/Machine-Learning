#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<time.h>
#include	<string.h>

char	*VecN, **MatM; 
int		Dm, Dn, *VecA, **MatA, *VecS, *VecT; 

/* wid.txt を呼び出し内容を記録する*/
void    readValue0(char *fn1)
{
	FILE		*fp;
	int		c, i, j, k;
	if((fp = fopen(fn1, "r")) == NULL) { printf("Unknown File = %s\n", fn1); exit(1);}
	Dn = 0;
	while((c = getc(fp)) != EOF) if(c == '\n') Dn +=1; 
	fclose(fp); 
	printf("%d\n", Dn); 
	VecN = (char *) malloc(sizeof(char)*4096);
	MatM = (char **) malloc(sizeof(char *)*Dn);
	fp = fopen(fn1, "r"); 
	for(i = 0; i < Dn; i++){
		while((c = getc(fp)) != EOF) if(c == ' ') break; 
		for(j = 0; j < 4096; j++) if((VecN[j] = getc(fp)) == ' ') break; 
		while((c = getc(fp)) != EOF) if(c == '\n') break; 
		MatM[i] = (char *) malloc(sizeof(char)*(j+1));
		for(k = 0; k < j; k++) MatM[i][k] = VecN[k]; 
		MatM[i][j] = '\0'; 
	}
	fclose(fp);
	printf("%d\n", Dn); 
}

int		mySearch(char *np, int b, int e)
{
	int		c, v;
//	if(b == e){ printf("unknown\n"); return(-1);}
	if(b == e) return(-1); 
	c = b + ((e-b)/2); 
	v = strcmp(np, MatM[c]);
	if(v == 0) return(c); 
	else if(v < 0) return(mySearch(np, b, c)); 
	else return(mySearch(np, c+1, e));
}

/* wakachi.txt を呼び出し内容を記録する */
void	readValue1(char *fn1)
{
	FILE	*fp;
	int		c, i, j, k;  
	if((fp = fopen(fn1, "r")) == NULL){ printf("Unknown File = %s\n", fn1); return;}
	Dm = 0; 
	while((c = getc(fp)) != EOF) if(c == '\n') Dm +=1; 
	fclose(fp); 
	printf("%d\n", Dm);
	VecA = (int *) malloc(sizeof(int)*Dm);
	MatA = (int **) malloc(sizeof(int *)*Dm);
	fp = fopen(fn1, "r"); 
	i = j = k = 0; 
	while((c = getc(fp)) != EOF){ 
		if(c == ' '){ 
			if(j > 0) {VecN[j] = '\0'; VecS[k++] = mySearch(VecN, 0, Dn); j = 0; } 
		}
		else if(c == '\n'){
			if(j > 0) { VecN[j] = '\0'; VecS[k++] = mySearch(VecN, 0, Dn); } 
			VecA[i] = k; 
			MatA[i] = (int *) malloc(sizeof(int)*k);
			for(j = 0; j < k; j++) MatA[i][j] = VecS[j]; 
			i++; j = k = 0; 
		}
		else VecN[j++] = c;
	}
	fclose(fp); 
	printf("%d\n", i);
}

/* 各単語に対応する単語番号を lbl.txt に登録 */
void	printValue(char *fn1)
{
	FILE	*fp;
	int		h, i, j, k;
	for(i = 0; i < Dn; i++) VecT[i] = 0; 
	fp = fopen(fn1, "w"); 
	fprintf(fp, "%d %d 1\n", Dm, Dn); 
	for(i = 0; i < Dm; i++){ 
		for(j = k = 0; j < VecA[i]; j++){ 
			h = MatA[i][j]; 
			if(VecT[h] != 0){ VecT[h] += 1; continue; }
			VecS[k++] = h; 
			VecT[h] = 1;
		}
		fprintf(fp, "%d ", k); 
		for(j = 0; j < k; j++){ 
			h = VecS[j]; 
			fprintf(fp, "%d:%d ", h+1, VecT[h]); 
			VecT[h] = 0;
		}
		fprintf(fp, "\n"); 
	}
	fclose(fp);
}

int	main(int argc, char **argv)
{
	int		i, j, k;
	readValue0(argv[1]); 
	VecS = (int *) malloc(sizeof(int)*Dn); 
	VecT = (int *) malloc(sizeof(int)*Dn); 
	readValue1(argv[2]);
	printValue(argv[3]); 
	return 0; 
}

/*
実行コマンド
gcc mklbl.c -lm -o mklbl
mklbl.exe data/wid.txt data/wakachi.txt data/lbl.txt
*/
