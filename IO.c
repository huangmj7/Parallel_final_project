#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>

int m = -1;
int n = -1;
int k = -1;

double *a = NULL;
double *b = NULL;
double *c = NULL;

/*input functions*/

int input(char* files,double* A, double* B){

        if(m == -1 || n == -1 || k == -1 || sizeof(files) == 0 || a == NULL || b == NULL){
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

int output(char* file, double* C){

	if(sizeof(file) == 0 || m == -1 || n == -1){
		perror("ERROR: No input is given for writing");
		return -1;
	}

	FILE *my_output_file = fopen(file,"w");
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){

			fprintf(my_output_file,"%lf ",C[i*n+j]);
		}
		fprintf(my_output_file,"\n");
	}
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
	m = atoi(argv[3]);
	n = atoi(argv[4]);
	k = atoi(argv[5]);
        
	a = calloc(m*k,sizeof(double));
	b = calloc(k*n,sizeof(double));
	c = calloc(m*n,sizeof(double));

	input(argv[1],a,b);
        
	for(int i=0; i<k; i++){
		for(int j=0; j<n; j++){
			printf("%le ",b[i*n+j]);
		}
		printf("\n");
	}

	for(int l=0; l<(m*n); l++){c[l] = l;}
	output(argv[2],c);
       
        free(a);
	free(b);
	free(c);	
	return 0;

}
