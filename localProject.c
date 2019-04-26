#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>
#include "mpi.h"
#include "clcg4.h"




int m = -1;
int n = -1;
int k = -1;


int rank;
int size;
int rank_edge;
int unitedge;

/*input functions*/
void print2d(double *array, int r, int c);
void reshape(double* array, int np, int k,int *blocks,int row, int col, double *result, int type);
void SUMMA(double* a_loc, double* b_loc, double* c_loc);

int input(char* files,double* A, double* B){

	if(m == -1 || n == -1 || k == -1 || sizeof(files) == 0 || A == NULL || B == NULL){
		perror("No input is given");
		return -1;
	}


	/*Read in*/
	FILE* my_input_file = fopen( files, "r");
	if(my_input_file  == NULL){
		perror("FAIL TO READ INPUT FILE");
		return -1;
	}


	for(int i=0; i<(m*k); i++){if(!fscanf(my_input_file,"%lf",&A[i])){break;}}
	for(int i=0; i<(m*k); i++){if(!fscanf(my_input_file,"%lf",&B[i])){break;}}


	fclose(my_input_file);
	return 0;


}

int MakeRandom(double* array,int size,int key){
	
	if(array == NULL){return -1;}
	for(int i=0; i<size; i++){
		
		if(key == -1){array[i] = 1.00;}
		else{array[i] = (double)GenVal(key);}
	}
	return 0;
}


int main(int argc,char**argv){

	if(argc < 4){
		perror("ERROR: NOT ENOUGH INPUT");
		return -1;
	}

	InitDefault();
	/*
	 * argv[1] key if 42 then all matrix element is 1 for validation, else random generate
	 * argv[2] m
	 * argv[3] n
	 * argv[4] k
	 * A m*k
	 * B k*n
	 * C m*n
	 */     
	rank = -1;
	size = -1;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* calculate sizes for a 2d grid of processors */
	int blocks[2] = {0,0}; //required
	MPI_Dims_create(size, 2, blocks);
	if(blocks[1] == 0 || blocks[0] ==  0 || rank == -1 || size == -1){
		perror("ERROR: UNABLE TO SPLIT PROCESSOR\n");
		return -1;
	}

	rank_edge = sqrt(size);


	m = atoi(argv[2]);
	n = atoi(argv[3]);
	k = atoi(argv[4]);
	int keys = atoi(argv[1]);

	double *unit_A,*unit_B,*unit_C;
	//double *A = calloc(m*k, sizeof(double));
	//double *B = calloc(k*n,sizeof(double));
	double *C = calloc(m*n,sizeof(double));
	int blocksize_A = (m*k)/size;
	int blocksize_B = (k*n)/size;
	unit_A = calloc(blocksize_A,sizeof(double));
	unit_B = calloc(blocksize_B,sizeof(double));
	unit_C = calloc( m * n/ size, sizeof(double));

	
	if(keys == 42){keys = -1;}
	else{keys = rank;}
	MakeRandom(unit_A,blocksize_A,keys);
	MakeRandom(unit_B,blocksize_B,keys);

	MPI_Barrier(MPI_COMM_WORLD);

	SUMMA(unit_A, unit_B, unit_C);

	/* Debudding
	for(int i = 0; i < size; i++){
		if(rank == i){
			printf("Rank %d:\n", rank);
			print2d(unit_A, unitedge, unitedge);
			printf("\n");
			print2d(unit_B, unitedge, unitedge);
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}
	*/

	int blocksize_c = (m*n)/size;
	MPI_Gather(unit_C,blocksize_c,MPI_DOUBLE,C,blocksize_c,MPI_DOUBLE,0,MPI_COMM_WORLD);
	if(rank == 0){

		
		//reshape(C,size,n,blocks,m/blocks[0],n/blocks[1],ans,2);
		print2d(C,m,n);
	}

	MPI_Finalize();
	return 0;

}

void print2d(double *array, int r, int c){

	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){

			printf("%.0f ",array[i*c+j]);
		}
		printf("\n");
	}

}


void reshape(double* array, int np, int k,int *blocks,int row, int col, double *result, int type){

        int n=0;
        if(row == 0){return;}
        for(int p=0; p<np; p++){
                int r = (int) p/blocks[1];
                int c = p%blocks[1];
                for(int i=0; i<row; i++){
                        for(int j=0; j<col; j++){
                                //printf("%d %d %d\n",p,i,j);
                                if(type == 1){result[n] = array[(i+r*row)*k+((c*col)+j)];}
                                else if(type == 2){result[(i+r*row)*k+((c*col)+j)] = array[n];}
                                n++;
                        }
                }
        }
}





void SUMMA(double* a_loc, double* b_loc, double* c_loc){

	MPI_Status status;
	MPI_Request request;

	double* work_row = calloc(k, sizeof(double));
	double* work_col = calloc(k, sizeof(double));

	int rank_loc[2];
	int loc[2];

	rank_loc[0] = rank / rank_edge;			//Rows
	rank_loc[1] = rank % rank_edge;			//Cols

	unitedge = m / rank_edge;

	/*
	if(rank == 0){
		printf("%d %d\n", rank_loc[0], rank_loc[1]);
		printf("rankedge: %d\nunitedge: %d\n", rank_edge, unitedge);
	}
	*/

		
	MPI_Barrier(MPI_COMM_WORLD);
	
	for(int kk = 0; kk < k; kk++){

		//if(rank == 0) printf("KK %d\n", kk);

		//printf("R%d: %d %d\n", rank, rank_loc[0], rank_loc[1]);

		if(kk / unitedge != rank_loc[0]){
			loc[0] = kk / unitedge;
			loc[1] = rank_loc[1];

			int soc_rank = (loc[0]*rank_edge + loc[1]);

			//printf("R%d: %d %d\n",rank, loc[0], loc[1]);
				
			//printf("R%d: soc row rank %d\n",rank, soc_rank);
			
			MPI_Irecv(work_row, unitedge, MPI_DOUBLE, soc_rank, kk, MPI_COMM_WORLD, &request);
		}

		MPI_Barrier(MPI_COMM_WORLD);
		
		if(kk / unitedge != rank_loc[0]) {MPI_Wait(&request, &status);}

		//Get partial row for computation
		if(kk / unitedge == rank_loc[0]){
		

			int loc_row = kk - rank_loc[0] * unitedge; 

			for(int i = 0; i < unitedge; i++){
				work_row[i] = a_loc[loc_row * unitedge + i];
			}
			
			for(int i = 1; (rank_loc[0] + i) % rank_edge != rank_loc[0]; i++){
				
				loc[0] = (rank_loc[0] + i) % rank_edge;
				loc[1] = rank_loc[1];

				int dst_rank = (loc[0]*rank_edge + loc[1]);

				//printf("R%d: dst row rank %d\n", rank, dst_rank);

				MPI_Isend(work_row, unitedge, MPI_DOUBLE, dst_rank, kk, MPI_COMM_WORLD, &request);
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);

		if(kk / unitedge != rank_loc[1]){
			loc[0] = rank_loc[0];
			loc[1] = kk / unitedge;

			int soc_rank = (loc[0]*rank_edge + loc[1]);
			
			//printf("R%d: soc col rank %d\n",rank, soc_rank);

			MPI_Irecv(work_col, unitedge, MPI_DOUBLE, soc_rank, kk, MPI_COMM_WORLD, &request);

		}
		
		MPI_Barrier(MPI_COMM_WORLD);

		if(kk / unitedge != rank_loc[1]) {MPI_Wait(&request, &status);}
		
		//Get partial col for computation
		if(kk / unitedge == rank_loc[1]){
			int loc_col = kk - rank_loc[1] * unitedge;
			
			for(int i = 0; i < unitedge; i++){
				work_col[i] = b_loc[i * unitedge + loc_col];
			}

			for(int i = 1; (rank_loc[1] + i) % rank_edge != rank_loc[1]; i++){

				loc[0] = rank_loc[0];
				loc[1] = (rank_loc[1] + i) % rank_edge;
				
				int dst_rank = (loc[0]*rank_edge + loc[1]);
				
				//printf("R%d: dst col rank %d\n", rank, dst_rank);
				
				MPI_Isend(work_col, unitedge, MPI_DOUBLE, dst_rank, kk, MPI_COMM_WORLD, &request);
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);

		//Multiply
		for(int i = 0; i < unitedge; i++){
			for(int j = 0; j < unitedge; j++){
				c_loc[i*unitedge + j] += work_col[i] * work_row[j];
			}
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
	}

}
