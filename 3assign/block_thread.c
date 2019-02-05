////////////////////////////////////////////////////////////////////
//
//      Kevin Richmond
//      CMSC257 - Assignment 3
//	Last Update - 11/9/16
//
//      Description: Mutiply 2 matrices of n size with normal multiplication,
//              transposed multiplication, and block multiplication.  Accepts
//              user input for n, b, and elements of matrices A and B.  Will
//              print out multiplication of A and B to matrix C and show the 
//              time that the calculation took.
//

#include <sys/times.h>
#include <math.h> 	// For sqrt function
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

double start, stop, used, mf;
double ftime(void);
double *a, *b, *c;
sem_t **sem;

double ftime (void)
{
    struct tms t;
    times ( &t );
    return (t.tms_utime + t.tms_stime) / 100.0;
}

void block_multiply(double *a, double *b, double *c, int n, int block, sem_t **sem) {
	// Block mutliply Matrix A * Matrix B = Matrix C
	int iOut, jOut, kOut, i, j, k, pid;
	int fit = n, kidCount = 0;

	// First 3 loops of 6 loop, contains varOut
	for(iOut=0; iOut<fit; iOut+=block){
		for(jOut=0; jOut<fit; jOut+=block){
			for(kOut=0; kOut<fit; kOut+=block){ 

				// Insert FORK If/else if/else for last 3 loops
				kidCount++;
				pid = fork();
				if (pid == 0){
					for(i=iOut; i<iOut+block; i++){
						for(j=jOut; j<jOut+block; j++){
							for(k=kOut; k<kOut+block; k++){
//			printf("Child %d  Parent is %d\n", k, getpid());
			sem_wait(sem[i*n+j]);
			c[i*n+j] += a[i*n+k] * b[k*n+j];
			sem_post(sem[i*n+j]);
	//		printf("Child: %d\na[i*n+k] = %f\nb[k*n+j] = %f\nc[i*n+j] = %f\n\n", getpid(), a[i*n+k], b[k*n+j], c[i*n+j]);
			//exit(0);
							}
						}
					}
					exit(0);
				} else if (pid < 0) {
//					printf("Fork failed. Returned %d\n", pid);
					//kidCount--;
				} else {
//					printf("Parent: new child is %d\n", pid);
				}
			}
		}
	}
	if (kidCount > 0) {
//		printf("\nChild processes created: %d\n", kidCount);		
		for(i = 1; i <kidCount; i++){
			wait(NULL);
//			printf("Child processes remaining: %d\n", kidCount-i);
		}
	}
}

void print_matrix(double *a, int size){
	int i, j;

	for (i=0; i<size; i++) {
		printf("\n");
		for (j=0; j<size; j++) {
			if (a[i*size+j] < 10) printf("[  %.2f] ", a[i*size+j]);
			else if (a[i*size+j] < 100) printf("[ %.2f] ", a[i*size+j]);
			else printf("[%.2f] ", a[i*size+j]);

		}
    	}
	printf("\n");

}

	
int main (void) {
	int i, j, n, block, shmfd;
	double value;	

	printf ( "Enter the value of n: ");
	scanf ( "%d", &n);
	printf("enter the block size (b): ");
	scanf ("%d", &block);

	start = ftime();
	// Create shared memory for matrix A
	shmfd = shm_open ( "/richmondkh_A", O_RDWR | O_CREAT, 0666 ); 
	if ( shmfd < 0 ) { 
        	fprintf(stderr,"Could not create richmondkh_A\n"); 
        	exit(1); 
	} 
	ftruncate ( shmfd, n*n*sizeof(double) ); 
    	a = (double *)mmap ( NULL, n*n*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 ); 
    	if ( a == NULL ) { 
        	fprintf(stderr,"Could not map richmondkh_A\n"); 
        	exit(1); 
    	} 
    	close ( shmfd ); 
    	shm_unlink ( "/richmondkh_A" ); 
	stop = ftime();
	used = stop - start;
//	printf("\nShared memory for A: %10.2f\n", used);	
	
	start = ftime();
	// Create shared memory for matrix B
	shmfd = shm_open ( "/richmondkh_B", O_RDWR | O_CREAT, 0666 ); 
	if ( shmfd < 0 ) { 
        	fprintf(stderr,"Could not create richmondkh_B\n"); 
        	exit(1); 
	} 
	ftruncate ( shmfd, n*n*sizeof(double) ); 
    	b = (double *)mmap ( NULL, n*n*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 ); 
    	if ( b == NULL ) { 
        	fprintf(stderr,"Could not map richmondkh_B\n"); 
        	exit(1); 
    	} 
    	close ( shmfd ); 
    	shm_unlink ( "/richmondkh_B" ); 
	stop = ftime();
	used = stop - start;
//	printf("Shared memory for B: %10.2f\n", used);	

	start = ftime();
	// Create shared memory for matrix C
	shmfd = shm_open ( "/richmondkh_C", O_RDWR | O_CREAT, 0666 ); 
	if ( shmfd < 0 ) { 
        	fprintf(stderr,"Could not create richmondkh_C\n"); 
        	exit(1); 
	} 
	ftruncate ( shmfd, n*n*sizeof(double) ); 
    	c = (double *)mmap ( NULL, n*n*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 ); 
    	if ( c == NULL ) { 
        	fprintf(stderr,"Could not map richmondkh_C\n"); 
        	exit(1); 
    	} 
    	close ( shmfd ); 
    	shm_unlink ( "/richmondkh_C" ); 
	stop = ftime();
	used = stop - start;
//	printf("Shared memory for C: %10.2f\n", used);	

	start = ftime();
	// Create shared memory for semaphore
	shmfd = shm_open ( "/richmondkh_sem", O_RDWR | O_CREAT, 0666 ); 
	if ( shmfd < 0 ) { 
        	fprintf(stderr,"Could not create richmondkh_sem\n"); 
        	exit(1); 
	} 
	ftruncate ( shmfd, n*n*sizeof(sem_t*) ); 
    	sem = (sem_t**)mmap ( NULL, n*n*sizeof(sem_t*), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 ); 
    	if ( sem == NULL ) { 
        	fprintf(stderr,"Could not map richmondkh_sem\n"); 
        	exit(1); 
    	} 
    	close ( shmfd ); 
    	shm_unlink ( "/richmondkh_sem" ); 
	stop = ftime();
	used = stop - start;
//	printf("Shared memory for semaphores: %10.2f\n", used);	
	
	start = ftime();
	// Create matrix A with numbering counting up from 1
	printf("\nMatrix A\n");
	for (i=0; i<n; i++){
		for (j=0; j<n ; j++){
			printf("Enter element for row %d, column %d: ", i, j);
			scanf("%lf", &value);
			a[i*n+j]=value;
		}
	}

	// Create matrix B with numbering counting down from n*n
	printf("\nMatrix B\n");
	for (i=0; i<n; i++){
		for (j=0; j<n ; j++){
			printf("Enter element for row %d, column %d: ", i, j);
			scanf("%lf", &value);
			b[i*n+j]=value;
		}
	}
	
	// Create matrix C with all 0s
	for (i=0; i<n; i++){
		for (j=0; j<n ; j++){
			c[i*n+j]=0.0;
		}
	}
	stop = ftime();
	used = stop - start;
//	printf("Fill matrix A, B and C: %10.2f\n", used);	


	// Create semaphore adddress space
	start = ftime();
	for (i=0; i<n; i++){
		for (j=0; j<n ; j++){
			char s1[100];
			sprintf(s1, "%d%d", i, j);
			sem[i*n+j] = sem_open (s1/*"richmondkh_sem"*/, O_CREAT, 0666, 1);
			if (sem == NULL) {
				fprintf(stderr,"Could not create Kevin's semaphore\n");
				exit(1);
			}
		sem_unlink (s1/*"richmondkh_sem"*/);
		}
	}
	
	stop = ftime();
	used = stop - start;
//	printf("Name sempahore address space: %10.2f\t(%2.0f mins)\n", used, used/60);	
/*	printf("\nMatrix A");
	print_matrix(a, n);
	printf("\nMatrix B");
	print_matrix(b, n);
*/	
     	start = ftime();
      	block_multiply(a,b,c,n,block,sem);	// Block multiplication
	stop = ftime();
	used = stop - start;
	printf("\nMatrix C\n");
	print_matrix(c, n);
	printf ("\nElapsed time:  %10.2f\n", used);


      	return (0);
}

