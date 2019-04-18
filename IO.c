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
        
	char* x = calloc(m*k,sizeof(char));
	char* y = calloc(k*n,sizeof(char));
        fscanf( my_input_file, "%s %s", x, y );
	for(int i=0; i<(m*k); i++){A[i] =  (double)(x[i] - '0');}
	for(int j=0; j<(k*n); j++){b[j] =  (double)(y[j] - '0');}
	
        fclose(my_input_file);
	free(x);
	free(y);
        return 0;
        

}

int main(int argc,char**argv){

	if(argc < 4){
		perror("ERROR: NOT ENOUGH INPUT");
		return -1;
	}
/*
 * argv[1] input file
 * argv[2] m 
 * argv[3] n 
 * argv[4] k 
 * A m*k
 * B k*n
 * C m*n 
*/
	m = atoi(argv[2]);
	n = atoi(argv[3]);
	k = atoi(argv[4]);
        
	a = calloc(m*k,sizeof(double));
	b = calloc(k*n,sizeof(double));

	input(argv[1],a,b);

	for(int i=0; i<k; i++){
		for(int j=0; j<n; j++){
			printf("%le ",b[i*n+j]);
		}
		printf("\n");
	}

	return 0;

}
