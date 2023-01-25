#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include	<unistd.h>

char	*VecN, **MatN; 
int		Dm, Dn, *VecI, *VecJ; 


/* doc.txt */
void    readValue(char *fn1)
{
	FILE		*fp;
	int		i, j, k, c;
	if((fp = fopen(fn1, "r")) == NULL) { printf("Unknown File = %s\n", fn1); exit(1);}
	Dm = Dn = 0;
	while((c = getc(fp)) != EOF){ if(c == ' ') Dn +=1; if(c == '\n') Dm +=1; }
	fclose(fp); 
	Dn += Dm; printf("%d %d\n", Dm, Dn); 
	fp = fopen(fn1, "r"); 
	MatN = (char **) malloc(sizeof(char *)*Dn);
	VecN = (char *) malloc(sizeof(char)*4096);
	i = j = 0; 
	while((c = getc(fp)) != EOF){ 
		if(c == ' ' || c == '\n'){ 
			if(j == 0) continue; 
			MatN[i] = (char *) malloc(sizeof(char)*(j+1));
			for(k = 0; k < j; k++) MatN[i][k] = VecN[k]; 
			MatN[i++][j] = '\0'; j = 0; 
		}
		else VecN[j++] = c;
	}
	fclose(fp);
	printf("%d\n", i); Dn = i; 
}
void	mySort(int *ip, int n)
{
	int		i, j, k, h;
	if((h = n/2) == 0) return;
	mySort(&ip[0], h);
	mySort(&ip[h], n-h); 
	for(i = j = 0, k = h; i < n; i++){
		if(j < h && (k == n || strcmp(MatN[ip[j]], MatN[ip[k]]) <= 0)) VecJ[i] = ip[j++]; 
		else VecJ[i] = ip[k++]; 
	}
	for(i = 0; i < n; i++) ip[i] = VecJ[i]; 
}

/* wid.txt */
void	printValue(char *fn1)
{
	FILE	*fp;
	int		h, i, j, k, s;
	VecI = (int *) malloc(sizeof(int)*Dn); 
	VecJ = (int *) malloc(sizeof(int)*Dn); 
	for(i = 0; i < Dn; i++) VecI[i] = i; 
	mySort(&VecI[0], Dn); 
	fp = fopen(fn1, "w"); 
	for(h = i = 0; i < Dn; ){ 
		s = VecI[i]; 
		for(j = i+1, k = 1; j < Dn; j++) if(strcmp(MatN[s], MatN[VecI[j]]) == 0) k++; else break; 
		fprintf(fp, "%d %s %d\n", h+1, MatN[s], k); 
		i = j; h++; 
	}
	fclose(fp);
}
int		main(int argc, char **argv)
{
	readValue(argv[1]);  //doc.txt
	printValue(argv[2]); //wid.txt
	return 0; 
}

/*
gcc mkwid.c -lm -o mkwid
mkwid.exe data/wakachi.txt data/wid.txt
*/
