#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>
#include<mpi.h>
#include<pthread.h>

/*Define function*/

/*Macro for colmn location*/
#define A(i,j)(a[i+j*lda]) 
#define B(i,j)(b[i+j*ldb])
#define c(i,j)(c[i+j*ldc])
/*Minimum*/
#define min(x,y) ((x) < (y) ? (x):(y))

/*define variable*/
#define alpha 1
#define beta  0

/*---------------------------Global-variable-------------------------------------------*/

/*MPI*/
MPI_Comm comm_row;
MPI_Comm comm_col;

int myrank,rowsize,colsize,rowrank,colrank;

/*BLAS*/
int i_one = 1;
double d_zero = 0.0;
double d_one = 1.0;

/*Size*/
int m = -1;
int n = -1;
int k = -1;

/*Location*/

/*------------------------------------------------------------------------------------*/



/*functions*/

int RING_Bcast(double *buf, int count, MPI_Datatype type,int root,MPI_Comm comm){

	int myrank = -1;
	int mysize = -1;
	
	MPI_Status status;
	MPI_Comm_rank(comm,&myrank);
	MPI_Comm_size(comm,&mysize);

	if(myrank !=root){MPI_Recv(buf,count,type,(myrank-1+mysize)%mysize,MPI_ANY_TAG,comm);}
	if((myrank+1)%mysize != root){MPI_Send(buf,count,type,(myrank+1)%np,0,comm);}	
}
int output(char* filename, int* C);
int basic_multiple(int *A, int *B, int *C);
int paper_based_multiple(int *A, int *B, int *C);




int main(int argc, char* argv){

	if(argc < 5){
		perror("ERROR: NO ENOUGH INPUT");
		return -1;
	}

	char* filename = argv[1];
	
	m = atoi(argv[2]); //A's size m*k
	n = atoi(argv[3]); //B's size k*n
	k = atoi(argv[4]); 


	//MPI_rank()
}
