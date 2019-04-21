#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"


double *A = NULL;
double *B = NULL;
double *C = NULL;

/*Distribute rows, then columns, each with a single scatterv 
 * m*n = length of (matrix)
 * */

int MatrixScatte(double* matrix,int m, int n,int rank, int size, int *blocks,double *localdata);

void printarray(double **data, int n, int m) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++)
            printf("%lf ",data[i][j]);
        putchar('\n');
    }
}



int main(int argc, char** argv){

	 int rank, size;
    	 MPI_Init(&argc, &argv);
    	 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	 MPI_Comm_size(MPI_COMM_WORLD, &size);

	 /* calculate sizes for a 2d grid of processors */
         int blocks[2] = {0,0}; //required
         MPI_Dims_create(size, 2, blocks);
         if(blocks[1] == 0 || blocks[0] ==  0){
                 perror("ERROR: UNABLE TO SPLIT PROCESSOR\n");
                 return -1;
         }

	
	 double *unit_A = NULL;
	 if(rank == 0){
		 printf("row is %d col is %d\n",blocks[0],blocks[1]);
		 A = calloc(64,sizeof(double));
		 for(int i=0; i<64; i++){A[i] = i;}
	 }
	 MatrixScatte(A,8,8,rank,size,blocks,unit_A);
	 free(A);
	 free(unit_A);
	 MPI_Finalize();
	 return 0;

}


/*Distribute rows, then columns, each with a single scatterv 
 * m*n = length of (matrix)
 * Decide blocks based on MPI_DIms_create
 * # of row = row process, # of col processor
 * Will do a communicator split inside the program
 * */

int MatrixScatte(double* matrix,int m, int n,int rank, int size, int *blocks,double *localdata){

	/*check input*/
	if(blocks[1] < 0){return -1;}


	 /*Split communicator for Row and Col*/
	 int myrow = (int)(rank/blocks[1]);
	 int mycol = (rank%blocks[1]);
	 //MPI_Comm colComm, rowComm;
	 //MPI_Comm_split(MPI_COMM_WORLD,myrow, rank, &rowComm);
	 //MPI_Comm_split(MPI_COMM_WORLD,mycol, rank, &colComm);
	
	 int *sendcounts = calloc(size,sizeof(int)); //# of row processor
         int *senddispls = calloc(size,sizeof(int)); 

	 /*1.Scatter rows to rank 0,Pc,2*Pc..,(m-1)*pc*/
	 
	 int blocksize = (int)((m*n)/blocks[0]);
         senddispls[0] = 0;
         

         for (int row=0; row<size; row++){
		/* each processor gets blocksize of element... */
            	sendcounts[row] = 0;
		if(row%blocks[1] == 0){sendcounts[row]+=blocksize;}
		if (row > 0){
			
			senddispls[row] = senddispls[row-1] + sendcounts[row-1];
		}
			

	}

	if(rank == 0){
		
		printf("%d: ", rank);
                for(int i=0; i<size; i++){printf("%d ",sendcounts[i]);}
		printf("\n");
		for(int i=0; i<size; i++){printf("%d ",senddispls[i]);}
		printf("\n");
        }

	/* allocate my rowdata */
        double* rowdata = calloc(sendcounts[rank],sizeof(double));

        /* perform the scatter of rows */
        MPI_Scatterv(matrix, sendcounts, senddispls, MPI_DOUBLE,rowdata, sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//MPI_Scatter(matrix,16,MPI_DOUBLE,rowdata,16,MPI_DOUBLE,0,MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);
	printf("rank %d: ",rank);
	for(int i=0; i<sendcounts[rank]; i++){printf("%lf ",rowdata[i]);}
	printf("\n");
        
	/*2.scatter through row scatter around array*/
        	
	
	int count,blocklen,stride;
	count = (int)((m*n)/size);
	blocklen = 1;//(int)((m*n)/blocks[1]);
	stride = 1;

	MPI_Datatype vec, localvec;
    	MPI_Type_vector(count, blocklen, stride, MPI_DOUBLE, &vec);
    	MPI_Type_create_resized(vec, 0, sizeof(double), &vec);
    	MPI_Type_commit(&vec);

    	MPI_Type_vector(count,blocklen,stride, MPI_DOUBLE, &localvec);
    	MPI_Type_create_resized(localvec, 0, sizeof(double), &localvec);
    	MPI_Type_commit(&localvec);
	
	int *rowcounts,*rowdisples;
	localdata = calloc(100,sizeof(double));
	rowcounts = calloc(size,sizeof(int));
	rowdisples = calloc(size,sizeof(int));
	rowdisples[0] = 0;
	for (int row=0; row<size; row++){
		
		rowcounts[row] = 0;
		if((row >= rank && row < rank+blocks[1]) && rank%blocks[1] == 0){rowcounts[row] += 1;}
                if (row > 0){rowdisples[row] = rowdisples[row-1] + sendcounts[row-1];}
        }

	if(rank%blocks[1] == 0){

                printf("%d: ", rank);
                for(int i=0; i<size; i++){printf("%d ",rowcounts[i]);}
                printf("\n");
                for(int i=0; i<size; i++){printf("%d ",rowdisples[i]);}
                printf("\n");
       }

	MPI_Scatterv(rowdata,rowcounts,rowdisples,vec,localdata,sendcounts[rank], localvec, 0, MPI_COMM_WORLD);


        MPI_Barrier(MPI_COMM_WORLD);
        printf("rank %d: ",rank);
        for(int i=0; i<rowcounts[rank]; i++){printf("%lf ",localdata[i]);}
        printf("\n");

	
	
/*	
	*/
	MPI_Type_free(&localvec);
	MPI_Type_free(&vec);
	//
	free(sendcounts);
	free(senddispls);
        free(rowdata);
	free(rowcounts);
        free(rowdisples);


	//MPI_Comm_free(&rowComm);
	//MPI_Comm_free(&colComm);

}
