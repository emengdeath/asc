#include <iostream>
#include <cstdio>
#include <algorithm>
#include <mpi.h>
using namespace std;
void getlr(int my_rank, int comm_sz, long long n, long long &l, long long &r) {
	l = (n / comm_sz) * my_rank + min((long long)my_rank, n % comm_sz) + 1;
	r = l + n / comm_sz - (my_rank + 1 > n % comm_sz);
}
void tree_send(int comm_sz, int my_rank, long long n, MPI_Comm comm){
	int step = 1;
	while (step <= my_rank)step += step;
	while (my_rank + step < comm_sz) {
		MPI_Send(&n, 1, MPI_LONG_LONG, my_rank + step, 0, comm);
		step += step;
	}
}
int main(){
	MPI_Init(NULL, NULL);
	int my_rank, comm_sz;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	long long sum = 0;
	long long l, r, n, result;
	if (my_rank == 0) {
		scanf("%lld", &n);
		tree_send(comm_sz, my_rank, n, MPI_COMM_WORLD);
		getlr(my_rank, comm_sz, n, l, r);
		while (l <= r) sum += l, l++;
		MPI_Reduce(&sum, &result, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
		printf("%lld\n",result);
	}else{
		MPI_Recv(&n, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		tree_send(comm_sz, my_rank, n, MPI_COMM_WORLD);
		getlr(my_rank, comm_sz, n, l, r);
		while (l <= r) sum += l, l++; 
		MPI_Reduce(&sum, NULL, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);	
	}
	MPI_Finalize();
	return 0;
}
