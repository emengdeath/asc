#include <iostream>
#include <cstdio>
#include <algorithm>
#include <mpi.h>
#include <malloc.h>
#define eps 1e-3
using namespace std;	
void Build_mpi_type(int&n, double&l, double&r, double*a, MPI_Datatype*mpi_struct){
	int *arr_of_blclength = (int*)malloc(3 * sizeof(int));
	MPI_Aint* arr_of_disp = (MPI_Aint*)malloc(3 * sizeof(MPI_Aint));
	MPI_Datatype* arr_of_type = (MPI_Datatype*)malloc(3 * sizeof(MPI_Datatype));
	
	arr_of_blclength[0] = arr_of_blclength[1] = 1;
	arr_of_blclength[2] = n;
	MPI_Aint l_addr, r_addr, a_addr;
	MPI_Get_address(&l, &l_addr), MPI_Get_address(&r, &r_addr), MPI_Get_address(a, &a_addr);
	arr_of_disp[0] = 0, arr_of_disp[1] = r_addr - l_addr, arr_of_disp[2] = a_addr - l_addr;
	arr_of_type[0] = arr_of_type[1] = arr_of_type[2] = MPI_DOUBLE;
	
	MPI_Type_create_struct(3, arr_of_blclength, arr_of_disp, arr_of_type,mpi_struct);
	MPI_Type_commit(mpi_struct);
	
	free(arr_of_blclength);
	free(arr_of_disp);
	free(arr_of_type);
}
double* Init_read(int my_rank, int comm_sz, int&n, double&l, double&r){
	double* a = NULL;
	MPI_Datatype mpi_struct;
	if ( my_rank == 0) {
		scanf("%d", &n);
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		a = (double*)malloc(n * sizeof(double));
		for (int i = 0; i < n; i++) 
			scanf("%lf", &a[i]);
		scanf("%lf %lf", &l, &r);
	} else {
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		a = (double*)malloc(n * sizeof(double));
	}
	Build_mpi_type(n, l, r, a, &mpi_struct);
	MPI_Bcast(&l, 1, mpi_struct, 0, MPI_COMM_WORLD);
	MPI_Type_free(&mpi_struct);
	return a;
}
double f(double x, double* a, int n){
	double s = 0, z = 1;
	for (int i = 0; i < n; i++)
		s += z * a[i], z *= x;
	return s;
}
double Get_area(double l, double r, double* a, int n){
	if ( r - l <= eps)
		return (f(l, a, n) + f(r, a, n)) / 2 * (r - l);
	return Get_area(l, (l + r) / 2, a, n) + Get_area((l + r) / 2, r, a, n);
}
int main(int argc, char* argv[]){
	MPI_Init(&argc, &argv);
	int my_rank, comm_sz, n;
	double l, r, len;
	double* a;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	a = (double*)Init_read(my_rank, comm_sz, n, l, r);
	len = (r - l) / comm_sz;
	l += len * my_rank;
	double area = Get_area(l, l + len, a, n);
	double all_area;
	if ( my_rank == 0){
		MPI_Reduce(&area, &all_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		printf("%.2f\n", all_area);			
	} else 
		MPI_Reduce(&area, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);	
	MPI_Finalize();
	return 0;
}
