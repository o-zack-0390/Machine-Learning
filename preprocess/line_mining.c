#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<time.h>
#include	<string.h>

char *word, **word_list; 
int	  wakachi_count, word_count;
int  *roop_list, **index_list, *result_sort, *count_list; 

/* wid.txt を呼び出し内容を記録する*/
void read_word(char *f_name)
{
	FILE *fp;
	int	c, i, j, k;
	
	if((fp = fopen(f_name, "r")) == NULL){
		printf("Unknown File = %s\n", f_name);
		exit(1);
	}
	
	word_count = 0;
	while((c = getc(fp)) != EOF){
		if(c == '\n')
			word_count +=1; 
	}
	
	fclose(fp); 
	printf("%d\n", word_count); 
	word      = (char *) malloc(sizeof(char)*4096);
	word_list = (char **) malloc(sizeof(char *)*word_count);
	fp        = fopen(f_name, "r"); 
	
	for(i = 0; i < word_count; i++){
		while((c = getc(fp)) != EOF){
			if(c == ' ')
				break; 
		}
		
		for(j = 0; j < 4096; j++){
			if((word[j] = getc(fp)) == ' ')
				break;
		}
		
		while((c = getc(fp)) != EOF){
			if(c == '\n')
				break; 
		}
		
		word_list[i] = (char *) malloc(sizeof(char)*(j+1));
		for(k = 0; k < j; k++) word_list[i][k] = word[k]; 
		word_list[i][j] = '\0'; 
		
	}
	
	fclose(fp);
	printf("%d\n", word_count); 
}


int	s_room_sort(char *np, int b, int e)
{
	int	c, v;
//	if(b == e){ printf("unknown\n"); return(-1);}
	if(b == e) return(-1); 
	c = b + ((e-b)/2); 
	v = strcmp(np, word_list[c]);
	if(v == 0)     return(c); 
	else if(v < 0) return(s_room_sort(np, b, c)); 
	else           return(s_room_sort(np, c+1, e));
}


/* wakachi.txt を呼び出し内容を記録する */
void read_wakachi(char *f_name)
{
	FILE *fp;
	int	c, i, j, k;  
	
	if((fp = fopen(f_name, "r")) == NULL){
		printf("Unknown File = %s\n", f_name);
		return;
	}
	
	wakachi_count = 0; 
	while((c = getc(fp)) != EOF){
		if(c == '\n')
			wakachi_count +=1;
	}
	
	fclose(fp); 
	printf("%d\n", wakachi_count);
	roop_list  = (int *) malloc(sizeof(int)*wakachi_count);
	index_list = (int **) malloc(sizeof(int *)*wakachi_count);
	fp         = fopen(f_name, "r"); 
	i          = 0;
	j          = 0;
	k          = 0; 
	
	while((c = getc(fp)) != EOF){
		
		if( (c == ' ') && (j > 0) ){
			word[j]          = '\0';
			result_sort[k++] = s_room_sort(word, 0, word_count);
			j                = 0;
		}
		
		else if(c == '\n'){
			
			if(j > 0){
				word[j]          = '\0';
				result_sort[k++] = s_room_sort(word, 0, word_count);
			} 
			
			roop_list[i]  = k; 
			index_list[i] = (int *) malloc(sizeof(int)*k);
			for(j = 0; j < k; j++) index_list[i][j] = result_sort[j]; 
			i++;
			j = 0;
			k = 0; 
		}
		
		else word[j++] = c;
	}
	
	fclose(fp); 
	printf("%d\n", i);
}


/* 各単語に対応する単語番号を lbl.txt に登録 */
void write_mining(char *f_name)
{
	FILE *fp;
	int	word_id, i, j, k;
	
	for(i = 0; i < word_count; i++) count_list[i] = 0; 
	fp = fopen(f_name, "w"); 
	fprintf(fp, "%d %d 1\n", wakachi_count, word_count); 
	
	for(i = 0; i < wakachi_count; i++){
		
		for(j = k = 0; j < roop_list[i]; j++){ 
			word_id = index_list[i][j];
			
			if(count_list[word_id] != 0){
				count_list[word_id] += 1;
				continue;
			}
			
			result_sort[k++]    = word_id; 
			count_list[word_id] = 1;
		}
		fprintf(fp, "%d ", k); 
		
		for(j = 0; j < k; j++){ 
			word_id = result_sort[j]; 
			fprintf(fp, "%d:%d ", word_id+1, count_list[word_id]); 
			count_list[word_id] = 0;
		}
		
		fprintf(fp, "\n"); 
	}
	
	fclose(fp);
}


int	main(int argc, char **argv)
{
	int	i, j, k;
	read_word(argv[1]);//wid.txt
	result_sort = (int *) malloc(sizeof(int)*word_count); 
	count_list  = (int *) malloc(sizeof(int)*word_count); 
	read_wakachi(argv[2]);//doc.txt
	write_mining(argv[3]);//lbl.txt
	return 0; 
}
