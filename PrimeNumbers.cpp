/*
 * PrimeNumbers.cpp
 * 
 * Copyright 2019 test <test@test-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <math.h>
#include <mpi.h>

int findPrime(int f){
	
	int cnt;
	int prime = 1;
	
	for( cnt = 2 ; cnt <= sqrt(f) ; cnt++){
		if ( f % cnt == 0 ){
			prime = 0;
			break;
		}
	}
	return prime;
}
		
		

int main(int argc, char **argv)
{
	int i;
	int p;
	int rank;
	int maxNum = 1000000;
	int numNums = maxNum + 1;
	int nums[maxNum + 1];
	int gnums[maxNum + 1];
	int count = 0;
	int gcount = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank = 0){
		for ( i = 0; i < numNums ; i++){
			nums[i] = 0;
			gnums[i] = 0;
		}
	nums[2] = 1;
	nums[3] = 1;
	printf("Arrays setup\n");
	fflush(stdout);
	}
	
	int start = rank + 4;
	MPI_Bcast(&numNums, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	for ( i = start ; i < numNums ; i+=p){
		nums[i] = findPrime(i);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Allgather( &nums, sizeof(*nums), MPI_INT, gnums, sizeof(*gnums), MPI_INT, MPI_COMM_WORLD);
	for ( i = start; i <= numNums ; i+=p){
		if ( gnums[i] == 1 && gnums[i - 2] == 1){
			count++;
			}
	}
	
	MPI_Reduce(&count, &gcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (rank == 0 ) {
		printf("Number of consecutive odd prime pairs between 0 and %d : %d \n", maxNum, gcount);
		fflush(stdout);
	}
	
	MPI_Finalize();
	return 0;
}
