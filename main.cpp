//
//  main.cpp
//  debuging
//
//  Created by Kang Seung Il on 2017. 3. 18..
//  Copyright © 2017년 Kang Seung Il. All rights reserved.

//#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <time.h>

#include "CgaBob.h"
#include "defines.h"
using namespace std;

int main() {
	FILE * N_QueenGA;
	fopen_s(&N_QueenGA, "E:\\C++\\Project5\\N_Queen(GA).csv", "a");
	for (int i= 4; i <= 15; i++) {
		fflush(N_QueenGA);
		srand((unsigned int)time(NULL));
		CgaBob *temp = new CgaBob(CROSSOVER_RATE, MUTATION_RATE, i*i, i*i, GENE_LENGTH, i);
		bool op = false;
		time_t begin, end;
		begin = clock();

		while (1) {
			temp->Epoch(op);
			if (op == true)
				break;
		}

		end = clock();
		cout << "12151636 CHOI JIN WOO" << endl;
		cout <<"N: "<< i <<" time : " << (double)(end - begin)<<" ms " << endl;
		cout << endl;
		fflush(N_QueenGA);
		fprintf(N_QueenGA, "%d,%f\n", i, (double)(end - begin));
		fflush(N_QueenGA);
	}
    return 0;
}
