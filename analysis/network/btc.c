/* コーディングを研究室で指定されてる書き方に統一 */
#include	<stdio.h>
#include	<stdlib.h>

int		main(int argc, char **argv)
{
	char	name[512]; 
	int		h, n; 
	n = atoi(argv[1]); 
	for(h = 0; h < 9; h++){
		sprintf(name, "./edtc data/lbl.txt data/wid.txt data/uid.txt %d %d result/node%dn%02d.txt result/link%dn%02d.txt > result/log%dn%02d.txt", h+1, n, h+1, n, h+1, n, h+1, n); 
		system(name); 
		sprintf(name, "./ddgm result/link%dn%02d.txt result/node%dn%02d.txt html/res%dn%02d.html", h+1, n, h+1, n, h+1, n); 
		system(name); 
	}
	return 0; 
}
