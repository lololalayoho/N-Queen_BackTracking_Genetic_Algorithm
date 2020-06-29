#include <iostream>
#include <vector>
#include <time.h>
using namespace std;
struct position { // ��ǥ��
	int x;
	int y;
};
int *visit; //�湮Ȯ��
int N;
int flag; //�����Ǵ�  ��ǥ 
vector<position> v;//��ǥ���� ������ vector
int input_check(position pos) { //vector�ȿ� �ش� ��ǥ�� ���� �Ǵ��� Ȯ���ϴ� �Լ�
	for (int i = 0; i < v.size(); i++) { //visit�迭�� ��,��,��,��� ��ġ�� ������ �ذ�
		if (abs(v[i].x - pos.x) == abs(v[i].y - pos.y))//�밢������ ��ġ�� ���� x��ǥ�� y��ǥ�� ���� ������ ���� ��� �밢���� ��ġ.
			return 0;//�밢���� ��ġ�ϸ� �ش� ��ǥ�� vector�� ���� �ȵǴ´ٴ� �ǹ�
	}
	return 1;//�밢���� ��ġ�ϰ� ���� ������ �ش� ��ǥ�� vector�� ���� �ȴٴ� �ǹ�
}
void check(int cnt) {//cnt�� ���� �ǹ���
	if (cnt == N) {//���� ������ N���� N-Queen���� ����.
		for (int i = 0; i < v.size(); i++)
			cout << v[i].y << " ";
		cout << endl;
		flag = true;//�����Ǵ� �ڸ� �߰�
	}
	else {
		for (int i = 0; i < N; i++) {//visit�迭�� ���鼭
			if (visit[i] == 0) {//visit�� ��������, �� �ش� ���� �湮���� �ʾ�����
				if (flag == true)//�����Ǵ� �ڸ��� ã���� ����
					break;
				visit[i] = 1; // �ش� ���� �湮��.
				position pos;
				pos.x = cnt; pos.y = i; //��� ���� ��ǥ�� ����
				if (input_check(pos) == 1) {//��ǥ�� vector�� ���� �Ǵ����� �Ǵ�.
					v.push_back(pos);//vector�� ���� �ȴٸ� push
					check(cnt + 1);//���������� �Ѿ
					v.pop_back();//����� ��ǥ�� pop
				}
				visit[i] = 0;//�湮�� ���� �̹湮���� ǥ��
			}
		}
	}
}
int check_time() {
	time_t start_time, end_time;
	double result;
	start_time = clock();
	check(0);//0�� ����� ����
	end_time = clock();
	result = (double)(end_time - start_time);
	cout << "12151636_������" << endl;
	cout << "N: " << N << " time : " << result << " ms" << endl;
	cout << endl;
	return result;
}
int main() {
	FILE * N_Queen1;
	fopen_s(&N_Queen1, "E:\\C++\\Project5\\N_Queen1.csv", "a");
	v.clear();
	for (N = 4; N <= 15; N++) {
		visit = new int[N];
		flag = false;
		for (int i = 0; i < N; i++)
			visit[i] = 0;
		fprintf(N_Queen1, "%d,%d\n", N, check_time());
	}
}