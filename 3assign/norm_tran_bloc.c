////////////////////////////////////////////////////////////////////
//
//	Kevin Richmond
//	CMSC257 - Assignment 3
//	Last update 11/9/16
//
//	Description: Mutiply 2 matrices of n size with normal multiplication,
//		transposed multiplication, and block multiplication.  Accepts
//		user input for n, b, and elements of matrices A and B.  Will
//		print out multiplication of A and B to matrix C and show the 
//		time that the calculation took.
//
#include <sys/times.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h> 	// For sqrt function

double start, stop, used, mf;

double ftime(void);
void multiply (double **a, double **b, double **c, int n);
void trans_multiply (double **a, double **b, double **c, int n);


double ftime (void)
{
    struct tms t;
    
    times ( &t );
 
    return (t.tms_utime + t.tms_stime) / 100.0;
}


void multiply (double **a, double **b, double **c, int n) {
	int i, j, k;

	for (i=0; i<n; i++) {		// Construct array C
     		for (j=0; j<n; j++)
         	c[i][j] = 0;
   	 }

   	for (i=0; i<n; i++) {		// Multiply A*B and put in C
		for (j=0; j<n; j++) {
         		for (k=0; k<n; k++){ 
        			c[i][j]= c[i][j] + a[i][k] * b[k][j];
			}
     		}
	}
}

void b_trans(double **b, int n){	// Transpose B to be read by trans_multiply
	int i, j, temp;

	for (i=0; i<n; i++) {
		for (j=i+1; j<n; j++){
			temp = b[i][j];
			b[i][j] = b[j][i];
			b[j][i] = temp;
		}
	}
}

void trans_multiply (double **a, double **b, double **c, int n) {
	int i, j, k;
	
	for (i=0; i<n; i++)   {		// Create an array for C
     		for (j=0; j<n; j++) {
         		c[i][j] = 0;
		}
    	}

	for (i=0; i<n; i++)    {	// Mutiply A * (b_trans) and put in C
		for (j=0; j<n; j++)       {
         		for (k=0; k<n; k++) {
           			c[i][j]= c[i][j] + a[i][k] * b[j][k]; // *b[k][j];
			}
		}
     	}
}


void block_multiply(double **a, double **b, double**c, int n, int block) {
	int iOut, jOut, kOut, i, j, k;
	int fit = n, count;


	for (i=0; i<n; i++)   {		// Create an array for C
     		for (j=0; j<n; j++) {
         		c[i][j] = 0;
		}
    	}
	
	for(iOut=0; iOut<fit; iOut+=block){
		for(jOut=0; jOut<fit; jOut+=block){
			for(kOut=0; kOut<fit; kOut+=block){ 
				count++;
				for(i=iOut; i<iOut+block; i++){
					for(j=jOut; j<jOut+block; j++){
						for(k=kOut; k<kOut+block; k++){
							c[i][j] += a[i][k] * b[k][j];
						}
					}
				}
			}
		}
	}		
}

void print_matrix(double **a, int size){
	int i, j;
	
	for (i=0; i<size; i++) {
		printf("\n");
		for (j=0; j<size; j++) {
		printf("[%.2f]\t", a[i][j]);
		}
      	}
	printf("\n");
}

	
int main (void) {
	int i, j, n, block;
	double **a, **b, **c, value;

	printf ( "Enter the value of n: ");
	scanf ( "%d", &n);

	printf ("Enter the block size (b): ");
	scanf ("%d", &block);

   	//Populate arrays....
     	a= (double**)malloc(n*sizeof(double));
     	b= (double**)malloc(n*sizeof(double));
     	c= (double**)malloc(n*sizeof(double));

 	for (i=0; i<n; i++)     {
       		a[i]= malloc(sizeof(double)*n);
      		b[i]= malloc(sizeof(double)*n);
      		c[i]= malloc(sizeof(double)*n);
     	 }

	printf("\nMatrix A\n");
	// Create matrix A
	for (i=0; i<n; i++)     {
		for (j=0; j<n; j++){
			printf("Enter element for row %d, column %d: ", i, j);
			scanf("%lf",&value); 
			a[i][j]=value;
		}
	 }
	
	printf("\nMatrix B\n");
	// Create matrix B 
     	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			printf("Enter element for row %d, column %d: ", i, j);
			scanf("%lf",&value); 
			b[i][j]=value;
		}
      	}

	// Create matrix C
	for (i=0; i<n; i++)   {		
     		for (j=0; j<n; j++) {
         		c[i][j] = 0;
		}
    	}


/*	printf("\nMatrix A\n");
	print_matrix(a, n);
	printf("\nMatrix B\n");
	print_matrix(b, n);
*/
	printf("\n**********Normal Multiplication**********");
     	start = ftime();
	multiply (a,b,c,n);
	stop = ftime();
        printf("\nMatrix C");
	print_matrix(c, n);
	used = stop - start;
      	mf = (n*n*n *2.0) / used / 1000000.0;
      	printf ( "\nElapsed time:   %10.2f \n", used);
 //     printf ( "DP MFLOPS:       %10.2f \n", mf);
	
	printf("\n**********Transpose Multiplication**********");
	b_trans(b,n);			// Transpose matrix b for trans_multiple
     	start = ftime();
	trans_multiply (a,b,c,n);	// Transpose reading of matrix b
	stop = ftime();
        printf("\nMatrix C");
	print_matrix(c, n);
      	used = stop - start;
      	mf = (n*n*n *2.0) / used / 1000000.0;
      	printf ( "\nElapsed time:   %10.2f \n", used);
//      printf ( "DP MFLOPS:       %10.2f \n", mf);
	b_trans(b,n);

	printf("\n**********Block Multiplication**********");
     	start = ftime();
      	block_multiply(a,b,c,n,block);	// Block multiplication
	stop = ftime();
     	printf("\nMatrix C");
	print_matrix(c, n);
      	used = stop - start;
      	mf = (n*n*n *2.0) / used / 1000000.0;
      	printf ( "\nElapsed time:   %10.2f \n", used);
 //     printf ( "DP MFLOPS:       %10.2f \n", mf);


      	return (0);
}

