#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

char   **MatN;
int   N, V, C, K, *VecA, **MatA, *VecB, **MatB, *VecC, **MatQ;
double **MatX, **MatY, *VecW, *VecZ;

/*lbl.txt�̓��e���L�^����*/
void readValue(char *fn1)
{
	FILE   *fp;
	int    i, j, k;
	double v;
/*-----------------------------------------------------------------------------------*/	
//	�t�@�C���������ꍇ�ُ͈�I������
	if((fp = fopen(fn1, "r")) == NULL)
	{ 
		printf("Unknown File = %s\n", fn1); 
		exit(1);
	}
	fscanf(fp, "%d %d %d", &N, &V, &C); 
/*	
	�ȉ��̐��l����͂���
	������ : 7367 �P�ꐔ : 75024  �J�e�S���[�� : 1
	
	fscanf(������,������,��O����)
	
	������ : �ǂݎ��t�@�C��
	
	������ : �ǂ�Ȍ^�œǂݍ��ނ�
	
	��O���� : ��������������ϐ�
	
/*-----------------------------------------------------------------------------------*/	
	VecA = (int *)     malloc(sizeof(int)*N);
	MatA = (int **)    malloc(sizeof(int *)*N);
	MatX = (double **) malloc(sizeof(double *)*N);
/*	
	malloc�֐��Ń������𓮓I�Ɋ����Ă�
	
	�T�C�Y = int�^�̃T�C�Y * N / int�^�̃T�C�Y
	
	N �͕����̐��Ƃ���
	
/*-----------------------------------------------------------------------------------*/	
//	�P��ID,�o���񐔂̊��蓖��
	for(i = 0; i < N; i++)
	{
		fscanf(fp, "%d", &VecA[i]);
		MatA[i] = (int *)    malloc(sizeof(int)*VecA[i]);
		MatX[i] = (double *) malloc(sizeof(double)*VecA[i]);
		
		for(j = 0; j < VecA[i]; j++)
		{ 
//			:�̍�����int�^,:�̉E����double�^�œǂݍ���
			fscanf(fp, "%d:%lf", &k, &v); 
			MatA[i][j] = k-1;
			MatX[i][j] = v;
		}
	}
/*-----------------------------------------------------------------------------------*/	

/*	------------------------------------------------------
	i �͈ȉ��𖞂������R���ł���
	0 <= i <= ������
	
	VecA[i]�ɂ͈ȉ��̃f�[�^���i�[����
	VecA[i] : ��̕����̒P�ꐔ �E�E�E (A)
	
	------------------------------------------------------
	j �͈ȉ��𖞂������R���ł���
	0 <= j <= VecA�̗v�f��
	
	MatA[i][j], MatX[i][j]�͎��̂悤�ɒ�`����
	MatA[i][j] : ��̕����ɏo�������̒P��̒P��ID �E�E�E (B)
	MatX[i][j] : MatA[i][j]�̏o����
	
	(A),(B)���AVecA�̗v�f�� == j�̗v�f�� �ł���
	
	------------------------------------------------------
	�� : ������ == 7367, �ŏ��̕����ɂ���P�ꐔ == 177 �̏ꍇ
	
	MatA[i], MatX[i]�ɂ�����i�͈̔�       : 0 �` 7366
	MatA[i][j], MatX[i][j]�ɂ�����j�͈̔� : 0 �` 176
	
	------------------------------------------------------
	k,v �Ɏ��̒l���i�[����
	
	k = �P��ID
	v = �o����
	
	-------------------------------------------------------
	�� : �P��ID == 47618, �o���� == 6 �̏ꍇ
	
	k = 47618
	v = 6
	
	-------------------------------------------------------
	�]���āA��̔C�ӂ̒P���word�Ƃ���ƁAMatA,MatX�ɂ͎��̒l���i�[�����
	
	MatA[i][j] = word�̒P��ID
	MatX[i][j] = word�̏o���� �E�E�E (C)
	
	(C)���A MatX �� TF-IDF �� TF �ł���
	
	-------------------------------------------------------
*/
	fclose(fp);
	printf("%d %d %d \n", N, V, C);
}


/*uid.txt�̓��e��ǂݍ���*/
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
//	�z��̃T�C�Y�����蓖�Ă�(N�͕�����)
	VecC = (int *)   malloc(sizeof(int)*N);
	MatN = (char **) malloc(sizeof(char *)*N);
	
/*-----------------------------------------------------------------------------------*/	
	for(i = 0; i < N; i++){
		fscanf(fp, "%d ", &VecC[i]);
/*		
		VecC�ɂ̓J�e�S���[�ԍ�1�`8���i�[�����
		�ȉ��ɃJ�e�S���[�ƃJ�e�S���[�ԍ��̑Ή�������
		
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
		i,j �͈ȉ��𖞂������R���ł���
		0 <= i <= 7367
		0 <= j <= 8192
		
		MatN �ɂ͈ȉ��̃f�[�^���i�[����
		MatN[i]    : �t�@�C����
		MatN[i][j] : �t�@�C�����̈ꕶ��
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
//	�z��̃T�C�Y�����蓖�Ă�
	VecB = (int *)     malloc(sizeof(int)*V);
	MatB = (int **)    malloc(sizeof(int *)*V);
	MatY = (double **) malloc(sizeof(double *)*V);
	
/*	
	V�͑S�����̑��P�ꐔ�ł���AreadValue���ŏ�������Ă���B
	�|�C���^�ŏ������Ă���̂łǂ̊֐�����ł��Q�Ɖ\�ł���B
*/
	
/*------------------------------------------------------------------------*/
	for(j = 0; j < V; j++) 
		VecB[j] = 0; 
	
	for(i = 0; i < N; i++){
		for(j = 0; j < VecA[i]; j++) 
			VecB[MatA[i][j]]++;
	}
	 
/*	�J��Ԃ�����
	
	    i < N ���� j < VecA[i] �̏ꍇ
	               ��
	i < ������  ���� j < ��̕��͂̒P�ꐔ �̏ꍇ
	               ��
	         VecB[�P��ID]��1���Z 
		   
	VecB : ��̔C�ӂ̒P�ꂪ���̕��͂ɏo������̂����L�^����
	
/*------------------------------------------------------------------------*/
// 	MatB,MatY�̗v�f�Ƀ����������蓖�Ă�
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
	
//	MatQ�Ƀ����������蓖�Ă�
	MatQ = (int **) malloc(sizeof(int *)*C);
	for(i = 0; i < C; i++){ 
		MatQ[i] = (int *) malloc(sizeof(int)*C); 
		for(j = 0; j < C; j++) MatQ[i][j] = 0; 
	}
	
//	VecW, VecZ�Ƀ����������蓖�Ă�
	VecW = (double *) malloc(sizeof(double)*V);
	VecZ = (double *) malloc(sizeof(double)*N);
/*-------------------------------------------------------------------------*/	
}


/*TF-IDF���v�Z����*/
void calTFIDF(char *fn1)
{
	int	   i, j;
	double v;
	FILE  *fp1;
	fp1 = fopen(fn1, "w"); 
	
/*------------------------------------------------------------------------*/
//	IDF �̌v�Z
	for(j = 0, v = log(1.0*N); j < V; j++) 
		VecW[j] = v;
	
	for(j = 0; j < V; j++) 
		VecW[j] -= log(1.0*VecB[j]);
/*	
	VecW[j]��log(1.0*N)����
	         ��
	j < v �̏ꍇ
	VecW[j]����log(1.0*VecB)�����Z
	
	log�̊��Z���A���Z���J��Ԃ����ƂŎ������Ă���
	IDF = log(1.0*N) / �P��word�̏o����
*/
/*------------------------------------------------------------------------*/
	int z, count = 0;
	
//	�S�����̃R�T�C���ގ��x���v�Z(�����̌v�Z��F�X�������Č����񍐂��� �� �������ނɓK�����v�Z���@�𕪐͂���)
	for(i = 0; i < N; i++)
	{
//		TF-IDF �̌v�Z
		for(j = 0; j < VecA[i]; j++) 
			MatX[i][j] *= VecW[MatA[i][j]];
/*		
		j < ��̕������̒P�ꐔ �̏ꍇ
		MatX[i][j] �� VecW[MatA[i][j]] ����Z
		              ��  
		    �P��̏o���� * VecW[�P��ID]
			          ��
			       TF * IDF    
		
		������AMatX �� TF-IDF �ƂȂ�
*/

//		TF-IDF ���L�^
		fprintf(fp1, "%s ", MatN[i]);
		for(j = 0; j < VecA[i]; j++) 
			fprintf(fp1, "%d:%e ", MatA[i][j]+1, MatX[i][j]); 
		fprintf(fp1, "\n");

//		�J��Ԃ�(j < ��̕������̒P�ꐔ)
		for(j = 0, v = 0.0; j < VecA[i]; j++) 
			v += MatX[i][j] * MatX[i][j];

/*		j < ��̕������̒P�ꐔ �܂ŌJ��Ԃ�
		
		      v �� TF-IDF * TF-IDF �����Z
		                 ��
		   v = v + MatX[i][j] * MatX[i][j]
*/
		
//		�J��Ԃ�(j < ��̕������̒P�ꐔ)
		for(j = 0, v = 1.0/sqrt(v); j < VecA[i]; j++) 
			MatX[i][j] *= v;

/*		v = 1.0/sqrt(TF-IDF*TF-IDF)
		j < ��̕������̒P�ꐔ �̏ꍇ
		
		         MatX[i][j](TF-IDF) �� v ����Z
		                  ��
		MatX[i][j] = TF-IDF * 1.0/sqrt(TF-IDF*TF-IDF)
		                ��                    ��
				    �C�ӂ̒P��              �S�Ă̒P��
*/
	}
/*------------------------------------------------------------------------*/
	fclose(fp1);
}


void calInverted()
{
	int	i, j, k;
	
//	�J��Ԃ�(j < �S�����̑��P�ꐔ)
	for(j = 0; j < V; j++) 
		VecB[j] = 0; 
	
//	�J��Ԃ�(i < ������)
	for(i = 0; i < N; i++)
	{
//		�J��Ԃ�(j < �C�ӂ̕����̒P�ꐔ)
		for(j = 0; j < VecA[i]; j++)
		{
			k = MatA[i][j]; 
			MatB[k][VecB[k]]   = i;
			MatY[k][VecB[k]++] = MatX[i][j];

/*			k = �P��ID
			MatB[�P��ID][0] = i
			MatY[�P��ID][1] = TF-IDF �̉��ǒl
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
	
//	�������܂ŌJ��Ԃ�
	for(i = 0; i < N; i++)
	{
		w = 0;
		
//		�������܂ŌJ��Ԃ�
		for(j = 0; j < N; j++) 
			VecZ[j] = 0.0; 
		
//		�J��Ԃ�(j < �C�ӂ̕����̒P�ꐔ)
		for(j = 0; j < VecA[i]; j++){
			h = MatA[i][j]; 
			v = MatX[i][j]; 
//			h = �P��ID; v = TF-IDF �̉��ǒl
			
//			�J��Ԃ�(k < VecB[�P��ID])
			for(k = 0; k < VecB[h]; k++) 
				VecZ[MatB[h][k]] += v * MatY[h][k]; 
/*
			VecB : ��̔C�ӂ̒P�ꂪ���̕��͂ɏo������̂��o�^����Ă���
			MatB : i(0 <= i < ������)���o�^����Ă���
			VecZ : ��̕������̊e�P��̏o���� * TF-IDF �̉��ǒl��P�ꐔ�܂ŌJ��Ԃ����Z����
			MatY : TF-IDF �̉��ǒl
			v    : TF-IDF �̉��ǒl
			
			VecZ[MatB[h][k]] += �o���� * TF-IDF �̉��ǒl
*/
		}
		
//		�J��Ԃ�(k < 2)
		for(k = 0; k < K; k++){
		
//			�J��Ԃ�(j < ������)
			for(j = h = 0, v = 0.0; j < N; j++)
			{
				if(VecZ[j] > v)
				{
					v = VecZ[j]; 
					h = j;
				}
			}
/*
			VecZ[j] > v �̏ꍇ (�����̍ő�l���l���傫��)
			
			v�@�̒l�� VecZ[j] �ɂ��� (�ő�l���X�V)
			h �̒l�� j ����        (���ސ��j�ɕύX)
*/
			
//			�ߎ��x������������nnsk02.txt�ɋL�^
			fprintf(fp1, "%d %e %s, ", h+1, v, MatN[h]); 
			
//			�J�e�S���[����
			if(k > 0) 
				MatQ[VecC[i]][VecC[h]] += 1; 
			
			if((k > 0) && (VecC[i] == VecC[h])) 
				w += 1;
			
			VecZ[h] = 0.0; 
		}
		fprintf(fp1, "\n"); 
		
//		�������x���Ɠ����J�e�S���[�ɕ��ނł��Ȃ������t�@�C����classification.txt�ɋL�^
		if(w < 0.5) 
			fprintf(fp2, "%d %s\n", VecC[i]+1, MatN[i]); 
	}

//	�R���t���[�W�����}�g���N�X���쐬����
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
	
//	���������v�Z����
	for(k = 0, v = 0.0; k < C; k++) 
		v += MatQ[k][k]; 
	
	printf("Accuracy = %.2f\n", v/(N*(K-1))); 
	fclose(fp1); 
}


void main(int argc, char **argv)
{
	readValue(argv[1]);			//������lbl.txt
	readName(argv[2]); 			//������uid.txt
	initData(); 
	calTFIDF(argv[3]);          //������tf-idf.txt
	calInverted(); 
	K = atoi(argv[4]);			//������2
	calValue(argv[5], argv[6]); //������nnsk02.txt, miss_uid.txt
}

/*
���s�R�}���h
gcc nnsk2.c -lm -o nnsk2
./nnsk2.exe data/lbl.txt data/uid.txt result/tf-idf.txt 2 result/nnsk02.txt data/miss_uid.txt

���s����
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
*/