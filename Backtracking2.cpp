#include <iostream>
#include <vector>
#include <time.h>
using namespace std;
int map[16][16];//visit 2차원 배열
int N;
int total_flag;//N-Queen 문제가 성립되느지 확인하는 깃발
vector<pair<int, int>> position;//x좌표와 y좌표를 저장하는 vector
void check(int start1, int start2) { //행과 열의 시작점.
	bool flag = true; //해당좌표가 vector안에 들어가도 되는지를 판단하는 깃발
	for (int i = 0; i < position.size(); i++) {
		for (int j = 0; j < position.size(); j++) {
			if (flag == false) {
				return;
				break;
			}
			if (i != j) {//index가 다르다면
				if ((position[i].first == position[j].first) || (position[i].second == position[j].second)) { //상,하,좌,우에 위치하고 있다면
					flag = false;//깃발 교체
					break;
				}
				if (abs(position[i].first - position[j].first) == abs(position[i].second - position[j].second)) { //대각선에 위치하고 있다면
					flag = false;//깃발 교체
					break;
				}
			}
		}
	}
	if (flag == true && position.size() == N) {//해당 좌표가 vector에 들어가도 되고 vector에 들어있는 queen의 숫자가 N개라면 깃발 교체
		for (int i = 0; i < position.size(); i++)
			cout << position[i].second << " ";
		cout << endl;
		total_flag = true;
	}
	for (int i = start1; i < N; i++) {
		if (total_flag == true)
			break;
		for (int j = start2; j < N; j++) {
			if (total_flag == true)
				break;
			if (map[i][j] == 0) {//visit을 하지 않았다면
				map[i][j] = 1;//visit
				position.push_back({ i,j });//vector에 삽입
				check(i + 1, 0);//다음 행으로 진행
				position.pop_back();//과거 좌표 반환
				map[i][j] = 0;//visit해제
			}
		}
	}
}
int check_time() {
	time_t start_time, end_time;
	double result;
	start_time = clock();
	check(0, 0);
	end_time = clock();
	result = (double)(end_time - start_time);
	cout << "12151636_최진우" << endl;
	cout << "N: " << N << " time : " << result << " ms" << endl;;
	cout << endl;
	return result;
}
int main() {
	FILE * N_Queen2;
	fopen_s(&N_Queen2, "E:\\C++\\Project5\\N_Queen2.csv", "a");
	for (N = 4; N <= 15; N++) {
		position.clear();
		total_flag = false;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				map[i][j] = 0;
		fprintf(N_Queen2, "%d,%d\n", N, check_time());
	}
}