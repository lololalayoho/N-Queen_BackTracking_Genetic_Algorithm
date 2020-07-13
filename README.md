# N-Queen 문제 Backtracking과 Genetic Algorithm 으로 풀기

## Backtracking 1차원 배열로 풀기
* 1차원 vist 배열 사용 (visit 배열 사용으로 항상 행 과 열이 다르게 select)
* input_check 함수로 N-Queen문제가 성립되는지 판단 (대각선에 위치하고 있는지)

## Backtracking 2차원 배열로 풀기
* x좌표와 y좌표를 선택
* 선택할 때마다 N-Queen문제가 성립되는 판단 (대각선, 상,하,좌,우)

## Genetic Algorithm

### Encoding (binary Encoding)
* visit배열을 사용하여 random으로 뽑을 때 조차도 항상 다르게 뽑도록 함

### Selection (Ranking Selection)
* m_vecGenomes 배열에 이미 rank를 대입해줌
* 높은 rank를 갖고 있는 만큼 select_number에 rank를 넣어줌
* 랜덤으로 rank를 뽑고 해당 rank를 갖고 있는 index중에 random으로 index 선택

### CrossOver (PMX)
* 서로 다른 index를 고르고 두 index 사이에 있는 값들을 CrossOver
* 단 서로 index 사이에 있는 값들을 자신들의 유전 정보를 보고 swap

### Mutation (Exchange Mutation)
* 서로 다른 index를 뽑고 두 index의 값을 바꿔줌

### Scaling (Ranking Scaling)
* Sorting을 적합도 기준으로 오름차순으로 정리
* 적합도 마다 ranking으로 scaling 진행

|전체 시간 복잡도 비교|1차원 Backtracking|2차원 Backtracking|Genetic Algorithm|
|-------------------|-------------------|-------------------|-------------------|
|<img src = "/image/비교.jpg" width="350" height="200">|<img src = "/image/1.jpg" width="350" height="200">|<img src = "/image/2.jpg" width="350" height="200">|<img src = "/image/g.jpg" width="350" height="200">|
