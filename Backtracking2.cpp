#include <iostream>
#include <vector>
#include <time.h>
using namespace std;
int map[16][16];//visit 2���� �迭
int N;
int total_flag;//N-Queen ������ �����Ǵ��� Ȯ���ϴ� ���
vector<pair<int, int>> position;//x��ǥ�� y��ǥ�� �����ϴ� vector
void check(int start1, int start2) { //��� ���� ������.
	bool flag = true; //�ش���ǥ�� vector�ȿ� ���� �Ǵ����� �Ǵ��ϴ� ���
	for (int i = 0; i < position.size(); i++) {
		for (int j = 0; j < position.size(); j++) {
			if (flag == false) {
				return;
				break;
			}
			if (i != j) {//index�� �ٸ��ٸ�
				if ((position[i].first == position[j].first) || (position[i].second == position[j].second)) { //��,��,��,�쿡 ��ġ�ϰ� �ִٸ�
					flag = false;//��� ��ü
					break;
				}
				if (abs(position[i].first - position[j].first) == abs(position[i].second - position[j].second)) { //�밢���� ��ġ�ϰ� �ִٸ�
					flag = false;//��� ��ü
					break;
				}
			}
		}
	}
	if (flag == true && position.size() == N) {//�ش� ��ǥ�� vector�� ���� �ǰ� vector�� ����ִ� queen�� ���ڰ� N����� ��� ��ü
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
			if (map[i][j] == 0) {//visit�� ���� �ʾҴٸ�
				map[i][j] = 1;//visit
				position.push_back({ i,j });//vector�� ����
				check(i + 1, 0);//���� ������ ����
				position.pop_back();//���� ��ǥ ��ȯ
				map[i][j] = 0;//visit����
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
	cout << "12151636_������" << endl;
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