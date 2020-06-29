#ifndef CGABOB_H
#define CGABOB_H

/////////////////////////////////////////////////////////////////////////
//
//		File: CGABob.h
//
//		Author: Mat Buckland
//
//		Desc: definition of the SGenome class and the genetic algorithm
//			  class CGABob from chapter 3
//
/////////////////////////////////////////////////////////////////////////

#include <vector>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include "defines.h"
#include <iostream>

using namespace std;

//--------------------------------------------------------------
//	define the genome structure
//--------------------------------------------------------------
struct SGenome
{
	vector<int> vecBits;
	double		dFitness;
	SGenome():dFitness(0){}
    SGenome(const int num_bits):dFitness(0)
    {
        vecBits.clear();
        int N = sqrt(num_bits); //N의 수
		int *visit = new int[num_bits]; //방문한 node check
		for (int i = 0; i < num_bits; i++)
			visit[i] = 0;
		for (int i = 0; i < N; i++) {
			int index = rand() % N;
			while (visit[index]!=0) { //방문하지 않은 Node 선택
				index = rand()%N;
			}
			if(visit[index] == 0) {
				visit[index] = 1;
				int j = 0;
				while (j < index) { //binary화
					vecBits.push_back(0);
					j++;
				}
				vecBits.push_back(1);
				j++;
				while (j < N) {//binary화
					vecBits.push_back(0);
					j++;
				}
			}
		}
    }
};
//--------------------------------------------------------------
//	define the genetic algorithm class
//---------------------------------------------------------------
class CgaBob
{
private:

	//the population of genomes
	vector<SGenome>	m_vecGenomes;
	
	//size of population
	int             m_iPopSize;

	double          m_dCrossoverRate;
	
	double          m_dMutationRate;
	
	//how many bits per chromosome
	int             m_iChromoLength;

	//how many bits per gene
	int             m_iGeneLength;
	
	int             m_iFittestGenome;
	
	double          m_dBestFitnessScore;
	
	double          m_dTotalFitnessScore;
	
	int             m_iGeneration;
    
    int             m_iSqrtChromoLength;
    
    double          m_dAverageFitness;
    
    double          m_dSigma;

	//lets you know if the current run is in progress.
	bool			m_bBusy;
	

	
	void        Mutate(vector<int> &vecBits);
	
	void        Crossover(const vector<int>	&mum,
                        const vector<int> &dad,
                        vector<int>       &baby1,
                        vector<int>       &baby2);
	
	SGenome&		Selection();
    
    void RankingScale(vector<SGenome>	&m_vecGenomes);
	
	//updates the genomes fitness with the new fitness scores and calculates
  //the highest fitness and the fittest member of the population.
  void			  UpdateFitnessScores(bool &op);

	//decodes a vector of bits into a vector of directions (ints)
  vector<int>	Decode(const vector<int> &bits);

	//creates a start population of random bit strings
  void			  CreateStartPopulation();

public:
	
	CgaBob(double cross_rat,
         double mut_rat,
         int    pop_size,
         int    num_bits,
         int    gene_len,
         int    sqrt_num):m_dCrossoverRate(cross_rat),
                          m_dMutationRate(mut_rat),
                          m_iPopSize(pop_size),
                          m_iChromoLength(num_bits),
                          m_dTotalFitnessScore(0.0),
                          m_iGeneration(0),
                          m_iGeneLength(gene_len),
                          m_iSqrtChromoLength(sqrt_num),
                          m_bBusy(true),
                          m_dAverageFitness(0.0),
                          m_dSigma(0.0)
		
	{
		CreateStartPopulation();
	}
    void			Epoch(bool &op);
	
	//accessor methods
	int				Generation(){return m_iGeneration;}
	int				GetFittest(){return m_iFittestGenome;}
};

#endif
