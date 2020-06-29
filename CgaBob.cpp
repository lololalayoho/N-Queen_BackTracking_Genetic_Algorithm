#include "CgaBob.h"
#include <iostream>
#include <algorithm>
using namespace std;
//--------------------------RouletteWheelSelection-----------------
//
//	selects a member of the population by using roulette wheel 
//	selection as described in the text.
//------------------------------------------------------------------
struct position {
	int x;
	int y;
};
void check_queen(vector <int> queen, int &Queenfitness) { //N-Queen ���� Ȯ�� �� ���յ� ���
	bool oper = false;
	for (int i = 0; i < queen.size(); i++) {
		Queenfitness++; //i�� �����Կ� ���� ���յ� ����
		position v;
		v.x = queen[i]; v.y = i;
		for (int j = 0; j < queen.size(); j++) {
			if (i != j) {
				if (v.x == queen[j]) { //��,��,��,�� üũ
					oper = true;
					break;
				}
				if (abs(v.x - queen[j]) == abs(i - j)) { //�밢�� üũ
					oper = true;
					break;
				}
			}
		}
		if (oper == true)
			break;
	}
	if (oper == false)
		Queenfitness = 999999999; //���࿡ N-Queen ������ �����Ǹ� MAX�� ��ȯ
}
double test_queen(vector <int> queen) { //���յ� ��ȯ �Լ�
	int Queenfitness = 0;
	check_queen(queen, Queenfitness); 
	return Queenfitness;
}
bool cmp(SGenome v1, SGenome v2) { //���յ��� �������� ������������ ����
	return v1.dFitness > v2.dFitness;
}

SGenome& CgaBob::Selection() { // Ranking Selection ���
	vector<int> select_number; //ranking�� ���� vector
	select_number.clear();
	vector<int> *index_genom = new vector<int>[m_vecGenomes.size()]; //ranking�� index�� �� genome��, 2���� ����
	double index = 0; //Fitness�ٸ� ������ ����
	int cnt = 1;//Fitness�ٸ� ������ ����
	for (int i = 1; i <= m_vecGenomes.size(); i++) {
		if (index != m_vecGenomes[i - 1].dFitness) { //�ٸ� Fitness���� ������ �Ǹ�
			index = m_vecGenomes[i - 1].dFitness; //����
			for (int j = 1; j <= cnt; j++) //�̹� ������������ �����Ǿ��ֱ� ������ ���� ������ ���� �ֵ��� select_number�� �׸�ŭ ���� ��
				select_number.push_back(cnt);
			cnt = cnt + 1; //����
		}
		index_genom[cnt - 1].push_back(i - 1); //ranking�� �ش�Ǵ� genome�� push
	}
	int N1 = rand() % select_number.size(); //ranking�� ��
	int N2 = rand() % index_genom[select_number[N1]].size(); //�ش� ranking�� �����ִ� genom���� random���� ����
	return m_vecGenomes[N2];
}

//----------------------------Mutate---------------------------------
//	iterates through each genome flipping the bits acording to the
//	mutation rate
//--------------------------------------------------------------------
void CgaBob::Mutate(vector<int> &vecBits) {
	int N = sqrt(vecBits.size());
	int* visit = new int[N];
	for (int i = 0; i < N; i++)
		visit[i] = 0;
	int index1 = rand() % N; //random�ϰ� index �̱�
	visit[index1] = 1;
	int index2 = rand() % N;
	while (visit[index2] != 0)//�̹� ���� index�� �����ϰ� random�ϰ� index �̱�
		index2 = rand() % N;
	int start1 = index1 * N;
	int start2 = index2 * N;
	for (int i = 0; i < N; i++) //�ΰ��� index�� ���� �� ���� ��ü
		swap(vecBits[i + start1], vecBits[i + start2]);
}

//----------------------------Crossover--------------------------------
//	Takes 2 parent vectors, selects a midpoint and then swaps the ends
//	of each genome creating 2 new genomes which are stored in baby1 and
//	baby2.
//---------------------------------------------------------------------
void CgaBob::Crossover(const vector<int> &mum, //PMX Crossover
	const vector<int> &dad,
	vector<int>		  &baby1,
	vector<int>		  &baby2)
{
	int N = sqrt(mum.size());
	baby1 = mum;
	baby2 = dad;
	int *visit = new int[N];
	for (int i = 0; i < N; i++)
		visit[i] = 0;
	int indexmum = rand() % N; //random�ϰ� index �̱�
	visit[indexmum] = 1;
	int indexdad = rand() % N;
	while (visit[indexdad] == 1)//�̹� ���� index�� �����ϰ� random�ϰ� index �̱�
		indexdad = rand() % N;
	//index���� ���Ͽ� ���� �ͺ��� for�� ���� ū index�� �����Ҷ� ����
	if (indexdad > indexmum) { 
		for (int i = indexmum; i <= indexdad; i++) {
			int start1 = i * N; //index ����
			int flag2 = false; // ��ü�� �����ٸ� ����
			for (int k = 0; k < N; k++) {
				if (baby1[start1 + k] != baby2[start1 + k]) { //�̹� index�� �ִ� baby1�� baby2�� ���� �ٸ��ٸ� ��ü �ʿ�
					flag2 = true;
					break;
				}
			}
			if (flag2 == true) { //�ٸ��� ������ �� baby���� �ش� index�� ���� ����ü�� ������ ����ü�� �ڱ� �ڽſ��� ã�Ƽ� ��ü
				//child1 �ٲٱ�
				int cnt = 0;
				while (1) {
					int flag = false; // ��ü�� �Ǿ������� �Ǵ��ϴ� ���
					int start = cnt * N; //������
					for (int k = 0; k < N; k++) { //while�� ���鼭 ���� index�� ���� ����ü�� ���������� �Ǵ�
						if (baby1[start + k] != baby2[start1 + k]) {
							flag = true;
							break;
						}
					}
					if (flag == true) //���� index�� ���濰��ü�� �ٸ��� index ����
						cnt = cnt + 1;
					else { //���� ����ü�� ���� �Ǹ� �ڱ� �ڽ��� ���� index�� ������ ����ü�� ��ü
						for (int j = 0; j < N; j++)
							swap(baby1[start1 + j], baby1[start + j]); 
						break;
					}
				}
				//child 2�� �����ϰ� �ٲٱ�
				cnt = 0;
				while (1) {
					int flag = false;
					int start = cnt * N;
					for (int k = 0; k < N; k++) {
						if (baby1[start1 + k] != baby2[start + k]) {
							flag = true;
							break;
						}
					}
					if (flag == true)
						cnt = cnt + 1;
					else {
						for (int j = 0; j < N; j++)
							swap(baby2[start1 + j], baby2[start + j]);
						break;
					}

				}
			}
		}
	}
	else { 
		for (int i = indexdad; i <= indexmum; i++) {
			int start1 = i * N;
			int flag2 = false;
			for (int k = 0; k < N; k++) {
				if (baby1[start1 + k] != baby2[start1 + k]) {
					flag2 = true;
					break;
				}
			}
			if (flag2 == true) {
				//child1 �ٲٱ�
				int cnt = 0;
				while (1) {
					int flag = false;
					int start = cnt * N;
					for (int k = 0; k < N; k++) {
						if (baby1[start + k] != baby2[start1 + k]) {
							flag = true;
							break;
						}
					}
					if (flag == true)
						cnt = cnt + 1;
					else {
						for (int j = 0; j < N; j++)
							swap(baby1[start1 + j], baby1[start + j]);
						break;
					}
				}
				cnt = 0;
				//child 2 �ٲٱ�
				while (1) {
					int flag = false;
					int start = cnt * N;
					for (int k = 0; k < N; k++) {
						if (baby1[start1 + k] != baby2[start + k]) {
							flag = true;
							break;
						}
					}
					if (flag == true)
						cnt = cnt + 1;
					else {
						for (int j = 0; j < N; j++)
							swap(baby2[start1 + j], baby2[start + j]);
						break;
					}

				}
			}
		}
	}
}
void CgaBob::RankingScale(vector<SGenome>	&m_vecGenomes) { //ranking scaleing ���
	sort(m_vecGenomes.begin(), m_vecGenomes.end(), cmp); //genom�� vector�� sorting
	int cnt = 1;
	double index = 0;
	for (int i = 1; i <= m_vecGenomes.size(); i++) {
		if (index != m_vecGenomes[i - 1].dFitness) { 
			index = m_vecGenomes[i - 1].dFitness;
			cnt = cnt + 1;
		}
		m_vecGenomes[i - 1].dFitness = cnt - 1;//���� genom�鿡�� ranking���� �ο�
	}

}
//----------------------CreateStartPopulation---------------------------
//
//-----------------------------------------------------------------------
void CgaBob::CreateStartPopulation() {
	//clear existing population
	m_vecGenomes.clear();
	for (int i = 0; i < m_iPopSize; i++) {
		m_vecGenomes.push_back(SGenome(m_iChromoLength));
	}

	//reset all variables
	m_iGeneration = 0;
	m_iFittestGenome = 0;
	m_dBestFitnessScore = 0;
	m_dTotalFitnessScore = 0;
}
//--------------------------------Epoch---------------------------------
//
//	This is the workhorse of the GA. It first updates the fitness
//	scores of the population then creates a new population of
//	genomes using the Selection, Croosover and Mutation operators
//	we have discussed
//----------------------------------------------------------------------
void CgaBob::Epoch(bool &op) {

	UpdateFitnessScores(op);

	RankingScale(m_vecGenomes);

	if (m_bBusy == false) return;

	//Now to create a new population
	int NewBabies = 0;

	//create some storage for the baby genomes 
	vector<SGenome> vecBabyGenomes;
	while (NewBabies < m_iPopSize) {
		//select 2 parents
		SGenome mum = Selection();
		SGenome dad = Selection();

		//operator - crossover
		SGenome baby1, baby2;
		Crossover(mum.vecBits, dad.vecBits, baby1.vecBits, baby2.vecBits);

		//operator - mutate
		if (rand() % 10 == 1 || rand() % 10 == 5) {
			Mutate(baby1.vecBits);
			Mutate(baby2.vecBits);
		}
		
		//add to new population
		vecBabyGenomes.push_back(baby1);
		vecBabyGenomes.push_back(baby2);

		NewBabies += 2;
	}

	//copy babies back into starter population
	m_vecGenomes = vecBabyGenomes;

	//increment the generation counter
	m_iGeneration = m_iGeneration + 1;
}

//---------------------------UpdateFitnessScores------------------------
//	updates the genomes fitness with the new fitness scores and calculates
//	the highest fitness and the fittest member of the population.
//	Also sets m_pFittestGenome to point to the fittest. If a solution
//	has been found (fitness == 1 in this example) then the run is halted
//	by setting m_bBusy to false
//-----------------------------------------------------------------------
void CgaBob::UpdateFitnessScores(bool &op) {
	m_iFittestGenome = 0;
	m_dBestFitnessScore = 0;
	m_dTotalFitnessScore = 0;
	m_dAverageFitness = 0;

	//update the fitness scores and keep a check on fittest so far
	for (int i = 0; i < m_iPopSize; i++)
	{
		//decode each genomes chromosome into a vector of directions
		vector<int> queenSet = Decode(m_vecGenomes[i].vecBits);
		/*
		for (int k = 0; k < queenSet.size(); k++) {
			cout << queenSet[k] << " ";
		}
		cout << endl;
		*/
		//get it's fitness score
		m_vecGenomes[i].dFitness = test_queen(queenSet);
		if (m_vecGenomes[i].dFitness == 999999999) {
			m_bBusy = false;
			op = true;
			for (int j = 0; j < queenSet.size(); j++) {
				cout << queenSet[j] << " ";
			}
			cout << endl;
			cout << m_iGeneration << endl;
			break;
		}
	}//next genome
}

//---------------------------Decode-------------------------------------
//
//	decodes a vector of bits into a vector of directions (ints)
//
//-----------------------------------------------------------------------
vector<int> CgaBob::Decode(const vector<int> &bits) 
{
	vector<int>	queenSet;
	int N = sqrt(bits.size());

	for (int i = 0; i < bits.size(); i = i + N) {
		int flag = false;
		for (int j = 0; j < N; j++) {
			if (bits[i + j] == 1) {
				queenSet.push_back(j);
				flag = true;
			}
		}
	}
	return queenSet;
}
