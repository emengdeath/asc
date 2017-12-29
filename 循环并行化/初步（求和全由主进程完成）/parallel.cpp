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
	if (my_rank == 0) {
		long long N;
		scanf("%lld", &N);
		long long send_buf = N;
		long long n = N;	
		for (int i=1;i < comm_sz; i++)
			MPI_Send(&send_buf, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
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
		MPI_Recv(&n, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		getlr(my_rank, comm_sz, n, l, r);
		while (l <= r) sum += l, l++; 
		MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
