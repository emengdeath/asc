#include <iostream>
#include <cstdio>
#include <algorithm>
#include <mpi.h>
#include <malloc.h>
using namespace std;
void Init_read(int my_rank, int comm_sz, int &n, int &local_n, MPI_Comm comm){
	if ( my_rank == 0) {
		scanf("%d", &n);
		local_n = n / comm_sz + (n % comm_sz > 0);
		MPI_Bcast(&local_n, 1, MPI_INT, 0, comm);
	} else 
		MPI_Bcast(&local_n, 1, MPI_INT, 0, comm);
}
double* Read_vector(int &local_n, int  my_rank, int comm_sz, int &n, MPI_Comm comm) {
	double *a = NULL;
	double *local_a = NULL;
	if ( my_rank == 0) {
		a = (double*)malloc(local_n * comm_sz * sizeof(double));
		local_a = (double*)malloc(local_n * sizeof(double));
		for (int i = 0; i < n; i++) 
			scanf("%lf", &a[i]);
		for (int i = comm_sz * local_n - 1; i >= n; i--) a[i] = 0;
		MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
		free(a);
	} else {
		local_a = (double*)malloc(local_n * sizeof(double));
		MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
	}
	return local_a;
}
void Print_vector(double local_a[], int local_n, int my_rank, int comm_sz, int n, MPI_Comm comm){
	double *a = NULL;
	if ( my_rank == 0) {
		a = (double*)malloc(local_n * comm_sz * sizeof(double));
		MPI_Gather(local_a, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, comm);
		for (int i = 0; i < n; i++)
			printf("%.2f ", a[i]);
		printf("\n");
		free(a);
	} else 
		MPI_Gather(local_a, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, comm);
}
int main(){
	MPI_Init(NULL, NULL);
	int my_rank, comm_sz, local_n, n;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	Init_read(my_rank, comm_sz, n, local_n, MPI_COMM_WORLD);
	double *local_a = (double*) Read_vector(local_n, my_rank, comm_sz, n, MPI_COMM_WORLD);
	double *local_b = (double*) Read_vector(local_n, my_rank, comm_sz, n, MPI_COMM_WORLD);
	for (int i = 0; i < local_n; i++) local_a[i] += local_b[i];
	Print_vector(local_a, local_n, my_rank, comm_sz, n, MPI_COMM_WORLD);
	free(local_a);
	free(local_b);
	MPI_Finalize();
	return 0;
}
