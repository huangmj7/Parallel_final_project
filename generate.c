#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>

/*Create a diagonal matrix in 1d form and write to file*/
void diag(FILE* file,int m,int n, int num){

	for(int i=0; i<m; i++){
                for(int j=0; j<n; j++){

                        int C = 0;
                        if(i==j){
				
				C = 1;
				if(num == -1){C = i;} //output diagonal matrix with the ascending non-zero from 0 to n
			}
			fprintf(file,"%d ",C);
                }
        }
	fprintf(file,"\n");
}


int main(int argc, char** argv){

	/*
	 * argv[1] output file name
	 * argv[2] m 
	 * argv[3] n
	 * this program output a .txt file of two  diagonal matrics with 
	 * size m*n in 1d form 
	 * i.e: matrix [1, 0
	 * 		0, 1]
	 * will look like 1 0 0 1 */
	if(argc < 4){
                perror("ERROR: No input is given for writing");
                return -1;
        }

	char *file = argv[1];
	int m = atoi(argv[2]);
	int n = atoi(argv[3]);

        FILE *my_output_file = fopen(file,"w");
        diag(my_output_file,m,n,1); //[0,1,0,1]
	diag(my_output_file,m,n,-1);//[0,0,0,1]
	fclose(my_output_file);
        return 0;

}
