#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

char   **MatN;
int	     N, V, C, K, *VecA, **MatA, *VecB, **MatB, *VecC, **MatQ;
double **MatX, **MatY, *VecW, *VecZ;

/*lbl.txtの内容を記録する*/
void readValue(char *fn1)
{
	FILE   *fp;
	int    i, j, k;
	double v;
/*-----------------------------------------------------------------------------------*/	
//	ファイルが無い場合は異常終了する
	if((fp = fopen(fn1, "r")) == NULL)
	{ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	fscanf(fp, "%d %d %d", &N, &V, &C); 
/*	
	以下の数値を入力する
	文書数 : 7367 単語数 : 75024  カテゴリー数 : 1
	
	fscanf(第一引数,第二引数,第三引数)
	
	第一引数 : 読み取るファイル
	
	第二引数 : どんな型で読み込むか
	
	第三引数 : 第二引数を代入する変数
	
/*-----------------------------------------------------------------------------------*/	
	VecA = (int *)     malloc(sizeof(int)*N);
	MatA = (int **)    malloc(sizeof(int *)*N);
	MatX = (double **) malloc(sizeof(double *)*N);
/*	
	malloc関数でメモリを動的に割当てる
	
	サイズ = int型のサイズ * N / int型のサイズ
	
	N は文書の数とする
	
/*-----------------------------------------------------------------------------------*/	
//	単語ID,出現回数の割り当て
	for(i = 0; i < N; i++)
	{
		fscanf(fp, "%d", &VecA[i]);
		MatA[i] = (int *)    malloc(sizeof(int)*VecA[i]);
		MatX[i] = (double *) malloc(sizeof(double)*VecA[i]);
		
		for(j = 0; j < VecA[i]; j++)
		{ 
//			:の左側はint型,:の右側はdouble型で読み込む
			fscanf(fp, "%d:%lf", &k, &v); 
			MatA[i][j] = k-1;
			MatX[i][j] = v;
		}
	}
/*-----------------------------------------------------------------------------------*/	

/*	------------------------------------------------------
	i は以下を満たす自然数である
	0 <= i <= 文書数
	
	VecA[i]には以下のデータを格納する
	VecA[i] : 一つの文書の単語数 ・・・ (A)
	
	------------------------------------------------------
	j は以下を満たす自然数である
	0 <= j <= VecAの要素数
	
	MatA[i][j], MatX[i][j]は次のように定義する
	MatA[i][j] : 一つの文書に出現する一つの単語の単語ID ・・・ (B)
	MatX[i][j] : MatA[i][j]の出現回数
	
	(A),(B)より、VecAの要素数 == jの要素数 である
	
	------------------------------------------------------
	例 : 文書数 == 7367, 最初の文書にある単語数 == 177 の場合
	
	MatA[i], MatX[i]におけるiの範囲       : 0 ～ 7366
	MatA[i][j], MatX[i][j]におけるjの範囲 : 0 ～ 176
	
	------------------------------------------------------
	k,v に次の値を格納する
	
	k = 単語ID
	v = 出現回数
	
	-------------------------------------------------------
	例 : 単語ID == 47618, 出現回数 == 6 の場合
	
	k = 47618
	v = 6
	
	-------------------------------------------------------
	従って、一つの任意の単語をwordとすると、MatA,MatXには次の値が格納される
	
	MatA[i][j] = wordの単語ID
	MatX[i][j] = wordの出現回数 ・・・ (C)
	
	(C)より、 MatX は TF-IDF の TF である
	
	-------------------------------------------------------
*/
	fclose(fp);
	printf("%d %d %d \n", N, V, C);
}


/*uid.txtの内容を読み込む*/
void readName(char *fn1)
{
	FILE	*fp;
	int		i, j, k;
	double	v;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
/*-----------------------------------------------------------------------------------*/	
//	配列のサイズを割り当てる(Nは文書数)
	VecC = (int *)   malloc(sizeof(int)*N);
	MatN = (char **) malloc(sizeof(char *)*N);
	
/*-----------------------------------------------------------------------------------*/	
	for(i = 0; i < N; i++){
		fscanf(fp, "%d ", &VecC[i]);
/*		
		VecCにはカテゴリー番号1～8が格納される
		以下にカテゴリーとカテゴリー番号の対応を示す
		
		1 dokujo-tsushin
		2 it-life-hack
		3 kaden-channel
		4 livedoor-homme
		5 movie-enter
		6 peachy
		7 smax
		8 sports-watch
		9 topic-news
*/
		MatN[i] = (char *) malloc(sizeof(char)*8191);
		for(j = 0; j < 8192; j++) 
		{
			if((MatN[i][j] = getc(fp)) == '\n') 
				break; 
		}
		MatN[i][j] = '\0'; 
/*		
		i,j は以下を満たす自然数である
		0 <= i <= 7367
		0 <= j <= 8192
		
		MatN には以下のデータを格納する
		MatN[i]    : ファイル名
		MatN[i][j] : ファイル名の一文字
*/
	}
/*-----------------------------------------------------------------------------------*/	
	fclose(fp);
	printf("%d\n",N);
}


void initData()
{
	int	i, j;
/*------------------------------------------------------------------------*/	
//	配列のサイズを割り当てる
	VecB = (int *)     malloc(sizeof(int)*V);
	MatB = (int **)    malloc(sizeof(int *)*V);
	MatY = (double **) malloc(sizeof(double *)*V);
	
/*	
	Vは全文書の総単語数であり、readValue内で処理されている。
	ポインタで処理しているのでどの関数からでも参照可能である。
*/
	
/*------------------------------------------------------------------------*/
	for(j = 0; j < V; j++) 
		VecB[j] = 0; 
	
	for(i = 0; i < N; i++){
		for(j = 0; j < VecA[i]; j++) 
			VecB[MatA[i][j]]++;
	}
	 
/*	繰り返し条件
	
	    i < N かつ j < VecA[i] の場合
	               ↓
	i < 文書数  かつ j < 一つの文章の単語数 の場合
	               ↓
	         VecB[単語ID]に1加算 
		   
	VecB : 一つの任意の単語が何個の文章に出現するのかを記録する
	
/*------------------------------------------------------------------------*/
// 	MatB,MatYの要素にメモリを割り当てる
	for(j = 0; j < V; j++){ 
		MatB[j] = (int *)    malloc(sizeof(int)*VecB[j]); 
		MatY[j] = (double *) malloc(sizeof(double)*VecB[j]); 
	}
	
	for(i = C = 0; i < N; i++)
	{ 
		if(VecC[i] > C)
			C = VecC[i]; 
		
		VecC[i]--; 
	}
	
//	MatQにメモリを割り当てる
	MatQ = (int **) malloc(sizeof(int *)*C);
	for(i = 0; i < C; i++){ 
		MatQ[i] = (int *) malloc(sizeof(int)*C); 
		for(j = 0; j < C; j++) MatQ[i][j] = 0; 
	}
	
//	VecW, VecZにメモリを割り当てる
	VecW = (double *) malloc(sizeof(double)*V);
	VecZ = (double *) malloc(sizeof(double)*N);
/*-------------------------------------------------------------------------*/	
}


/*TF-IDFを計算する*/
void calTFIDF(char *fn1)
{
	int	   i, j;
	double v;
	FILE  *fp1;
	fp1 = fopen(fn1, "w"); 
	
/*------------------------------------------------------------------------*/
//	IDF の計算
	for(j = 0, v = log(1.0*N); j < V; j++) 
		VecW[j] = v;
	
	for(j = 0; j < V; j++) 
		VecW[j] -= log(1.0*VecB[j]);
/*	
	VecW[j]にlog(1.0*N)を代入
	         ↓
	j < v の場合
	VecW[j]からlog(1.0*VecB)を減算
	
	logの割算を、減算を繰り返すことで実現している
	IDF = log(1.0*N) / 単語wordの出現回数
*/
/*------------------------------------------------------------------------*/
	int z, count = 0;
	
//	全文書のコサイン類似度を計算(ここの計算を色々いじって研究報告する → 文書分類に適した計算方法を分析する)
	for(i = 0; i < N; i++)
	{
//		TF-IDF の計算
		for(j = 0; j < VecA[i]; j++) 
			MatX[i][j] *= VecW[MatA[i][j]];
/*		
		j < 一つの文書中の単語数 の場合
		MatX[i][j] に VecW[MatA[i][j]] を乗算
		              ↓  
		    単語の出現回数 * VecW[単語ID]
			          ↓
			       TF * IDF    
		
		これより、MatX は TF-IDF となる
*/

//		TF-IDF を記録
		fprintf(fp1, "%s ", MatN[i]);
		for(j = 0; j < VecA[i]; j++) 
			fprintf(fp1, "%d:%e ", MatA[i][j]+1, MatX[i][j]); 
		fprintf(fp1, "\n");

//		繰り返し(j < 一つの文書中の単語数)
		for(j = 0, v = 0.0; j < VecA[i]; j++) 
			v += MatX[i][j] * MatX[i][j];

/*		j < 一つの文書中の単語数 まで繰り返し
		
		      v に TF-IDF * TF-IDF を加算
		                 ↓
		   v = v + MatX[i][j] * MatX[i][j]
*/
		
//		繰り返し(j < 一つの文書中の単語数)
		for(j = 0, v = 1.0/sqrt(v); j < VecA[i]; j++) 
			MatX[i][j] *= v;

/*		v = 1.0/sqrt(TF-IDF*TF-IDF)
		j < 一つの文書中の単語数 の場合
		
		         MatX[i][j](TF-IDF) に v を乗算
		                  ↓
		MatX[i][j] = TF-IDF * 1.0/sqrt(TF-IDF*TF-IDF)
		                ↑                    ↑
				    任意の単語              全ての単語
*/
	}
/*------------------------------------------------------------------------*/
	fclose(fp1);
}


void calInverted()
{
	int	i, j, k;
	
//	繰り返し(j < 全文書の総単語数)
	for(j = 0; j < V; j++) 
		VecB[j] = 0; 
	
//	繰り返し(i < 文書数)
	for(i = 0; i < N; i++)
	{
//		繰り返し(j < 任意の文書の単語数)
		for(j = 0; j < VecA[i]; j++)
		{
			k = MatA[i][j]; 
			MatB[k][VecB[k]]   = i;
			MatY[k][VecB[k]++] = MatX[i][j];

/*			k = 単語ID
			MatB[単語ID][0] = i
			MatY[単語ID][1] = TF-IDF の改良値
*/
		}
	}
}


void calValue(char *fn1, char *fn2)
{
	FILE	*fp1, *fp2;
	int		i, j, k, h;
	double	v, w;
	fp1 = fopen(fn1, "w"); 
	fp2 = fopen(fn2, "w");
	
//	文書数まで繰り返し
	for(i = 0; i < N; i++)
	{
		w = 0;
		
//		文書数まで繰り返し
		for(j = 0; j < N; j++) 
			VecZ[j] = 0.0; 
		
//		繰り返し(j < 任意の文書の単語数)
		for(j = 0; j < VecA[i]; j++){
			h = MatA[i][j]; 
			v = MatX[i][j]; 
//			h = 単語ID; v = TF-IDF の改良値
			
//			繰り返し(k < VecB[単語ID])
			for(k = 0; k < VecB[h]; k++) 
				VecZ[MatB[h][k]] += v * MatY[h][k]; 
/*
			VecB : 一つの任意の単語が何個の文章に出現するのか登録されている
			MatB : i(0 <= i < 文書数)が登録されている
			VecZ : 一つの文書中の各単語の出現回数 * TF-IDF の改良値を単語数まで繰り返し加算する
			MatY : TF-IDF の改良値
			v    : TF-IDF の改良値
			
			VecZ[MatB[h][k]] += 出現回数 * TF-IDF の改良値
*/
		}
		
//		繰り返し(k < 2)
		for(k = 0; k < K; k++){
		
//			繰り返し(j < 文書数)
			for(j = h = 0, v = 0.0; j < N; j++)
			{
				if(VecZ[j] > v)
				{
					v = VecZ[j]; 
					h = j;
				}
			}
/*
			VecZ[j] > v の場合 (既存の最大値より値が大きい)
			
			v　の値を VecZ[j] にする (最大値を更新)
			h の値を j する        (分類先をjに変更)
*/
			
//			近似度が高い文書をnnsk02.txtに記録
			fprintf(fp1, "%d %e %s, ", h+1, v, MatN[h]); 
			
//			カテゴリー分類
			if(k > 0) 
				MatQ[VecC[i]][VecC[h]] += 1; 
			
			if((k > 0) && (VecC[i] == VecC[h])) 
				w += 1;
			
			VecZ[h] = 0.0; 
		}
		fprintf(fp1, "\n"); 
		
//		正解ラベルと同じカテゴリーに分類できなかったファイルをclassification.txtに記録
		if(w < 0.5) 
			fprintf(fp2, "%d %s\n", VecC[i]+1, MatN[i]); 
	}

//	コンフュージョンマトリクスを作成する
	printf("Confusion matrix\n"); 
	printf("\t"); 
	
	for(j = 0; j < C; j++) 
		printf("%d\t", j+1); 
	printf("\n"); 
	
	for(k = 0; k < C; k++)
	{ 
		printf("%d\t", k+1); 
		
		for(j = 0, v = 0.0; j < C; j++) 
			v += MatQ[k][j]; 
		
		for(j = 0; j < C; j++) 
			printf("%.2f\t", MatQ[k][j]/v); 
		
		printf("\n"); 
	}
	
//	正答率を計算する
	for(k = 0, v = 0.0; k < C; k++) 
		v += MatQ[k][k]; 
	
	printf("Accuracy = %.2f\n", v/(N*(K-1))); 
	fclose(fp1); 
}


void main(int argc, char **argv)
{
	readValue(argv[1]);			//引数はlbl.txt
	readName(argv[2]); 			//引数はuid.txt
	initData(); 
	calTFIDF(argv[3]);          //引数はtf-idf.txt
	calInverted(); 
	K = atoi(argv[4]);			//引数は2
	calValue(argv[5], argv[6]); //引数はnnsk02.txt, miss_uid.txt
}

/*
実行コマンド
gcc nnsk2.c -lm -o nnsk2
./nnsk2.exe data/lbl.txt data/uid.txt result/tf-idf.txt 2 result/nnsk02.txt data/miss_uid.txt

実行結果
./nnsk2.exe data/lbl.txt data/uid.txt result/tf-idf.txt 2 result/nnsk02.txt data/miss_uid.txt
7367 90859 1
7367

Confusion matrix
        1       2       3       4       5       6       7       8       9
1       0.73    0.02    0.02    0.04    0.04    0.10    0.02    0.01    0.03
2       0.02    0.86    0.02    0.01    0.00    0.01    0.06    0.00    0.01
3       0.02    0.05    0.80    0.01    0.00    0.03    0.04    0.00    0.04
4       0.03    0.03    0.02    0.80    0.03    0.05    0.03    0.01    0.01
5       0.03    0.01    0.00    0.01    0.91    0.02    0.01    0.00    0.01
6       0.12    0.01    0.02    0.05    0.04    0.70    0.02    0.03    0.02
7       0.01    0.04    0.01    0.01    0.00    0.01    0.92    0.00    0.00
8       0.02    0.01    0.01    0.02    0.01    0.02    0.00    0.83    0.08
9       0.06    0.03    0.08    0.02    0.04    0.03    0.01    0.08    0.66
Accuracy = 0.80