#include <iostream>
#include <cstdio>
#include <algorithm>
#include <mpi.h>
using namespace std;
void getlr(int my_rank, int comm_sz, long long n, long long &l, long long &r) {
	l = (n / comm_sz) * my_rank + min((long long)my_rank, n % comm_sz) + 1;
	r = l + n / comm_sz - (my_rank + 1 > n % comm_sz);
}
int main(){
	MPI_Init(NULL, NULL);
	int my_rank, comm_sz;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	long long sum = 0;
	long long l, r;
	int step = 1;
	while (step <= my_rank)step += step;
	if (my_rank == 0) {
		long long N;
		scanf("%lld", &N);
		long long n = N;
		while (my_rank + step < comm_sz) {
			MPI_Send(&n, 1, MPI_LONG_LONG, my_rank + step, 0, MPI_COMM_WORLD);
			step += step;
		}
		long long v;
		getlr(my_rank,comm_sz,n,l,r);
		while (l <= r) sum += l, l++;
		for (int i = 1; i < comm_sz; i++) {
			MPI_Recv(&v, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum+=v;
		}
		printf("%lld\n",sum);
	}else{
		long long n;
		MPI_Recv(&n, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		while (my_rank + step < comm_sz) {
			MPI_Send(&n, 1, MPI_LONG_LONG, my_rank + step, 0, MPI_COMM_WORLD);
			step += step;
		}
		getlr(my_rank, comm_sz, n, l, r);
		while (l <= r) sum += l, l++; 
		MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
