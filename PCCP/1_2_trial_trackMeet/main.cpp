#include <string>
#include <vector>
#include <iostream>

class solutionClass
{
private:
    size_t studentCount;
    size_t gameCount;
    std::vector<int> DP;
    const std::vector<std::vector<int>>& ability;

public:
    solutionClass(const std::vector<std::vector<int>>& ability)
        : ability(ability)
    {
        studentCount = ability.size();
        gameCount = ability[0].size();
        DP.resize(1ULL << studentCount, 0);
    }

    int solve(size_t gameIdx, size_t studentFlags) // 남은 게임을 남은 학생으로 처리할 때의 최댓값
    {
        { // 완료 조건
            if (gameIdx == gameCount) return 0;
        }
        if (DP[studentFlags] != 0) return DP[studentFlags];
        int maxScore = 0;
        for (size_t studentIdx = 0; studentIdx < studentCount; ++studentIdx)
        {
            if ((studentFlags & (1ULL << studentIdx)) == 0)
            {
                size_t newFlags = studentFlags | (1ULL << studentIdx);
                int score = ability[studentIdx][gameIdx] + solve(gameIdx + 1, newFlags);
                maxScore = std::max(maxScore, score);
            }
        }
        DP[studentFlags] = maxScore;
        return maxScore;
    }
};

int solution(std::vector<std::vector<int>> ability) {
    int answer = 0;

    solutionClass sc(ability);
    answer = sc.solve(0, 0);

    return answer;
}

int main()
{
    std::vector<std::vector<int>> ability = { {40, 10, 10}, {20, 5, 0}, {30, 30, 30}, {70, 0, 70}, {100, 100, 100} };
    int ans = solution(ability);
    std::cout << ans << std::endl;
    return 0;
}

/*
    // 각종목 상위 gamecount 명을 순차 기록하기, 
    // 1등만 선택 후, 중복되는 부분에서

    // maximize 
    // P = 40s11 + 10s12 + 10s13
    //   + 20s21 +  5s22 +  0s23
    //   ...

    // s11 + s12 + s13 + S1 = 1
    // s21 + s22 + s23 + S2 = 1
    // s31 + s32 + s33 + S3 = 1
    // s41 + s42 + s43 + S4 = 1
    // s51 + s52 + s53 + S5 = 1

    // s11 + s21 + s31 + s41 + s51 = 1
    // s12 + s22 + s32 + s42 + s52 = 1
    // s13 + s23 + s33 + s43 + s53 = 1
    // s14 + s24 + s34 + s44 + s54 = 1
    // s15 + s25 + s35 + s45 + s55 = 1

    // - ability ** s + P = 0
    // s1  s2  s3  s4  s5  S1 S2 S3 S4 S5 P   rhs
    // 111 000 000 000 000 1  0  0  0  0  0 : 1
    // ...
    // -40,-10,-10 -20,-5,-0 -30-30-30 -70-0-70 -100-100-100 0  0  0  0  0  1 : 0

    // 0,30,30 0,15,20 0,0,0 0,70,0 0,0,0 40 20 30 70 100 1 : 260
    */