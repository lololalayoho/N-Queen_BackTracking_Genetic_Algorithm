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
void check_queen(vector <int> queen, int &Queenfitness) { //N-Queen 성립 확인 및 적합도 계산
	bool oper = false;
	for (int i = 0; i < queen.size(); i++) {
		Queenfitness++; //i가 증가함에 따라 적합도 증가
		position v;
		v.x = queen[i]; v.y = i;
		for (int j = 0; j < queen.size(); j++) {
			if (i != j) {
				if (v.x == queen[j]) { //상,하,좌,우 체크
					oper = true;
					break;
				}
				if (abs(v.x - queen[j]) == abs(i - j)) { //대각선 체크
					oper = true;
					break;
				}
			}
		}
		if (oper == true)
			break;
	}
	if (oper == false)
		Queenfitness = 999999999; //만약에 N-Queen 문제가 성립되면 MAX값 반환
}
double test_queen(vector <int> queen) { //적합도 반환 함수
	int Queenfitness = 0;
	check_queen(queen, Queenfitness); 
	return Queenfitness;
}
bool cmp(SGenome v1, SGenome v2) { //적합도를 기준으로 오름차순으로 정리
	return v1.dFitness > v2.dFitness;
}

SGenome& CgaBob::Selection() { // Ranking Selection 사용
	vector<int> select_number; //ranking을 넣을 vector
	select_number.clear();
	vector<int> *index_genom = new vector<int>[m_vecGenomes.size()]; //ranking을 index로 들어갈 genome들, 2차원 벡터
	double index = 0; //Fitness다를 때마다 갱신
	int cnt = 1;//Fitness다를 때마다 갱신
	for (int i = 1; i <= m_vecGenomes.size(); i++) {
		if (index != m_vecGenomes[i - 1].dFitness) { //다른 Fitness값이 나오게 되면
			index = m_vecGenomes[i - 1].dFitness; //갱신
			for (int j = 1; j <= cnt; j++) //이미 오름차순으로 정리되어있기 때문에 낮은 비율을 갖는 애들은 select_number에 그만큼 적게 들어감
				select_number.push_back(cnt);
			cnt = cnt + 1; //갱신
		}
		index_genom[cnt - 1].push_back(i - 1); //ranking에 해당되는 genome들 push
	}
	int N1 = rand() % select_number.size(); //ranking을 고름
	int N2 = rand() % index_genom[select_number[N1]].size(); //해당 ranking을 갖고있는 genom들중 random으로 고르기
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
	int index1 = rand() % N; //random하게 index 뽑기
	visit[index1] = 1;
	int index2 = rand() % N;
	while (visit[index2] != 0)//이미 뽑은 index를 제외하고 random하게 index 뽑기
		index2 = rand() % N;
	int start1 = index1 * N;
	int start2 = index2 * N;
	for (int i = 0; i < N; i++) //두개의 index를 갖는 두 수를 교체
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
	int indexmum = rand() % N; //random하게 index 뽑기
	visit[indexmum] = 1;
	int indexdad = rand() % N;
	while (visit[indexdad] == 1)//이미 뽑은 index를 제외하고 random하게 index 뽑기
		indexdad = rand() % N;
	//index들을 비교하여 작은 것부터 for문 시작 큰 index가 도달할때 까지
	if (indexdad > indexmum) { 
		for (int i = indexmum; i <= indexdad; i++) {
			int start1 = i * N; //index 선택
			int flag2 = false; // 교체가 끝났다면 종료
			for (int k = 0; k < N; k++) {
				if (baby1[start1 + k] != baby2[start1 + k]) { //이미 index에 있는 baby1과 baby2의 값이 다르다면 교체 필요
					flag2 = true;
					break;
				}
			}
			if (flag2 == true) { //다르기 때문에 각 baby들은 해당 index의 상대방 염색체와 동일한 염색체를 자기 자신에서 찾아서 교체
				//child1 바꾸기
				int cnt = 0;
				while (1) {
					int flag = false; // 교체가 되었느지를 판단하는 깃발
					int start = cnt * N; //시작점
					for (int k = 0; k < N; k++) { //while이 돌면서 뽑은 index가 상대방 염색체와 같은것인지 판단
						if (baby1[start + k] != baby2[start1 + k]) {
							flag = true;
							break;
						}
					}
					if (flag == true) //뽑은 index가 상대방염색체와 다르면 index 증가
						cnt = cnt + 1;
					else { //상대방 염색체와 같게 되면 자기 자신의 현재 index과 동일한 염색체를 교체
						for (int j = 0; j < N; j++)
							swap(baby1[start1 + j], baby1[start + j]); 
						break;
					}
				}
				//child 2도 동일하게 바꾸기
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
				//child1 바꾸기
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
				//child 2 바꾸기
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
void CgaBob::RankingScale(vector<SGenome>	&m_vecGenomes) { //ranking scaleing 사용
	sort(m_vecGenomes.begin(), m_vecGenomes.end(), cmp); //genom의 vector를 sorting
	int cnt = 1;
	double index = 0;
	for (int i = 1; i <= m_vecGenomes.size(); i++) {
		if (index != m_vecGenomes[i - 1].dFitness) { 
			index = m_vecGenomes[i - 1].dFitness;
			cnt = cnt + 1;
		}
		m_vecGenomes[i - 1].dFitness = cnt - 1;//각각 genom들에게 ranking값을 부여
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
