#include <iostream>
#include <vector>
#include <time.h>
using namespace std;
struct position { // 좌표값
	int x;
	int y;
};
int *visit; //방문확인
int N;
int flag; //성립되는  좌표 
vector<position> v;//좌표값을 저장할 vector
int input_check(position pos) { //vector안에 해당 좌표가 들어가도 되는지 확인하는 함수
	for (int i = 0; i < v.size(); i++) { //visit배열로 상,하,좌,우로 겹치는 문제는 해결
		if (abs(v[i].x - pos.x) == abs(v[i].y - pos.y))//대각선으로 겹치는 경우는 x좌표와 y좌표를 각각 뺀값이 같은 경우 대각선에 위치.
			return 0;//대각선에 위치하면 해당 좌표는 vector에 들어가면 안되는다는 의미
	}
	return 1;//대각선에 위치하고 있지 않으면 해당 좌표는 vector에 들어가도 된다는 의미
}
void check(int cnt) {//cnt는 행을 의미함
	if (cnt == N) {//뽑힌 갯수가 N개면 N-Queen문제 성립.
		for (int i = 0; i < v.size(); i++)
			cout << v[i].y << " ";
		cout << endl;
		flag = true;//성립되는 자리 발견
	}
	else {
		for (int i = 0; i < N; i++) {//visit배열을 돌면서
			if (visit[i] == 0) {//visit을 안했으면, 즉 해당 열을 방문하지 않았으면
				if (flag == true)//성립되는 자리를 찾으면 종료
					break;
				visit[i] = 1; // 해당 열을 방문함.
				position pos;
				pos.x = cnt; pos.y = i; //행과 열을 좌표로 저장
				if (input_check(pos) == 1) {//좌표가 vector에 들어가도 되는지를 판단.
					v.push_back(pos);//vector에 들어가도 된다면 push
					check(cnt + 1);//다음행으로 넘어감
					v.pop_back();//사용한 좌표는 pop
				}
				visit[i] = 0;//방문한 열을 미방문으로 표시
			}
		}
	}
}
int check_time() {
	time_t start_time, end_time;
	double result;
	start_time = clock();
	check(0);//0번 행부터 시작
	end_time = clock();
	result = (double)(end_time - start_time);
	cout << "12151636_최진우" << endl;
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