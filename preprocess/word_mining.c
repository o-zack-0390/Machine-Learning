#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include	<unistd.h>

char *str_list, **cmp_list1;
int  *index_list, *sort_part; 
int   new_line_count, space_count;


/*wakachi.txt を呼び出し内容を記録する*/
void read_word(char *f_name)
{
	FILE *fp;
	int	i, j, k, c;
	
	if((fp = fopen(f_name, "r")) == NULL)
	{
		printf("Unknown File = %s\n", f_name);
		exit(1);
	}
	
	new_line_count = 0;
	space_count    = 0;
	
	while((c = getc(fp)) != EOF)
	{
		if(c == ' ')  space_count    +=1;
		if(c == '\n') new_line_count +=1;
	}
	
	fclose(fp); 
	space_count += new_line_count; 
	printf("%d %d\n", new_line_count, space_count); 
	
	fp        = fopen(f_name, "r"); 
	cmp_list1 = (char **) malloc(sizeof(char *)*space_count);
	str_list  = (char *)  malloc(sizeof(char)*4096);
	i         = 0;
	j         = 0; 
	
	while((c = getc(fp)) != EOF)
	{ 
		if(c == ' ' || c == '\n')
		{ 
			if(j == 0)
				continue; 
			
			cmp_list1[i] = (char *) malloc(sizeof(char)*(j+1));
			
			for(k = 0; k < j; k++) {
				cmp_list1[i][k] = str_list[k]; 
				printf("%c ", cmp_list1[i][k]);
			}
			printf("\n");
			
			cmp_list1[i++][j] = '\0';
			j = 0;
		}
		
		else 
			str_list[j++] = c;
	}
	
	fclose(fp);
	printf("%d\n", i);
	space_count = i; 
}


/* 並べ替え */
void s_room_sort(int *ip, int n)
{
	int	i, j, k, h;
	
	if((h = n/2) == 0) return;
	s_room_sort(&ip[0], h);
	s_room_sort(&ip[h], n-h); 
	
	for(i = j = 0, k = h; i < n; i++)
	{
		if(j < h && (k == n || strcmp(cmp_list1[ip[j]], cmp_list1[ip[k]]) <= 0))
			sort_part[i] = ip[j++]; 
		
		else
			sort_part[i] = ip[k++]; 
	}
	
	for(i = 0; i < n; i++) 
		ip[i] = sort_part[i]; 
}


/* wid.txt を呼び出し内容を記録する*/
void print_id(char *f_name)
{
	FILE *fp;
	int   h, i, k, pos, next_pos;
	
	index_list  = (int *) malloc(sizeof(int)*space_count); 
	sort_part   = (int *) malloc(sizeof(int)*space_count); 
	
	for(i = 0; i < space_count; i++)
		index_list[i] = i;
	
	s_room_sort(&index_list[0], space_count); 
	fp = fopen(f_name, "w"); 
	
	for(h = i = 0; i < space_count; )
	{
		pos = index_list[i]; 
		
		for(next_pos = i+1, k = 1; next_pos < space_count; next_pos++)
		{
			if(strcmp(cmp_list1[pos], cmp_list1[index_list[next_pos]]) == 0)
				k++; 
			
			else
				break; 
		}
		
		fprintf(fp, "%d %s %d\n", h+1, cmp_list1[pos], k); 
		i = next_pos;
		h++; 
	}
	
	fclose(fp);
}


int	main(int argc, char **argv)
{
	read_word(argv[1]); //wakachi.txt
	print_id(argv[2]);  //wid.txt
	return 0; 
}
