#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>

void diag(FILE* file,int m,int n, int num){

	for(int i=0; i<m; i++){
                for(int j=0; j<n; j++){

                        int C = 0;
                        if(i==j){
				
				C = 1;
				if(num == -1){C = i;}
			}
			fprintf(file,"%d ",C);
                }
        }
	fprintf(file,"\n");
}


int main(int argc, char** argv){

	if(argc < 4){
                perror("ERROR: No input is given for writing");
                return -1;
        }

	char *file = argv[1];
	int m = atoi(argv[2]);
	int n = atoi(argv[3]);

        FILE *my_output_file = fopen(file,"w");
        diag(my_output_file,m,n,1);
	diag(my_output_file,m,n,-1);
	fclose(my_output_file);
        return 0;

}
