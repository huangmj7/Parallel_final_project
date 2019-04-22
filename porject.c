#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>
#include "mpi.h"

int m = -1;
int n = -1;
int k = -1;

/*input functions*/
void print2d(double *array, int r, int c);
void reshape(double* array, int np, int k,int *blocks,int row, int col, double *result);
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


int main(int argc,char**argv){

        if(argc < 5){
                perror("ERROR: NOT ENOUGH INPUT");
                return -1;
        }
/*
 * argv[1] input file
 * argv[2] output file
 * argv[3] m
 * argv[4] n
 * argv[5] k
 * A m*k
 * B k*n
 * C m*n
*/     
	int rank = -1;
	int size = -1;
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


        m = atoi(argv[3]);
        n = atoi(argv[4]);
        k = atoi(argv[5]);

	double *unit_A,*unit_B,*unit_C;
	double *A = calloc(m*k, sizeof(double));
        double *B = calloc(k*n,sizeof(double));
	int blocksize_A = (m*k)/size;
	int blocksize_B = (k*n)/size;
	unit_A = calloc(blocksize_A,sizeof(double));
	unit_B = calloc(blocksize_B,sizeof(double));

	if(rank == 0){
		
		double *a = calloc(m*k,sizeof(double));
		double *b = calloc(k*n,sizeof(double));
		input(argv[1],a,b);
		print2d(a,m,k);

		int ak = n;
                int ar = m/blocks[0];
                int ac = k/blocks[1];
                reshape(a,size,ak,blocks,ar,ac,A);
		int bk = k;
                int br = k/blocks[0];
                int bc = n/blocks[1];
                reshape(b,size,bk,blocks,br,bc,B);
		free(a);
        	free(b);
      
	}

	MPI_Scatter(A,blocksize_A,MPI_DOUBLE,unit_A,blocksize_A,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Scatter(B,blocksize_B,MPI_DOUBLE,unit_B,blocksize_B,MPI_DOUBLE,0,MPI_COMM_WORLD);       
      	MPI_Barrier(MPI_COMM_WORLD);

	if(rank == 1){
		printf("r: %d \n",rank);
		print2d(unit_A,(m/blocks[0]),(k/blocks[1]));
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

void reshape(double* array, int np, int k,int *blocks,int row, int col, double *result){

        int n=0;
        if(row == 0){return;}
        for(int p=0; p<np; p++){
                int r = (int) p/blocks[1];
                int c = p%blocks[1];
                for(int i=0; i<row; i++){
                        for(int j=0; j<col; j++){
                                //printf("%d %d %d\n",p,i,j);
                                result[n] = array[(i+r*row)*k+((c*col)+j)];
                                n++;
                        }
                }
        }
}

