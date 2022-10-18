#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

enum programInfo : size_t { priority = 0ULL, callTime = 1ULL, cpuTime = 2ULL };

size_t
binarySearch(const size_t stIdx, 
             const size_t enIdx,
             const std::vector<int>& target,
             const std::vector<std::vector<int>>& currentQueue)
{
    size_t size = enIdx - stIdx;
    if (size == 0) 
    {
        if (currentQueue[stIdx][priority] > target[priority])
        {
            return stIdx;
        } else {
            return stIdx + 1;
        }
    }
    size_t pivot = (stIdx + enIdx)/2;
    if (currentQueue[pivot][priority] > target[priority])
    {
        return binarySearch(stIdx, pivot, target, currentQueue);
    } else {
        return binarySearch(pivot+1, enIdx, target, currentQueue);
    }
}

std::vector<long long> solution(std::vector<std::vector<int>> program) { // 우선순위, 호출시각, 소요시간
    std::vector<long long> answer(11, 0); // 종료 시각, 우선순위 별 대기시간

    // 우선순위 sort + 호출시각 sort
    std::sort(program.begin(), program.end(), [](const std::vector<int>& a, const std::vector<int>& b){
        return a[0] < b[0];
    });
    std::stable_sort(program.begin(), program.end(), [](const std::vector<int>& a, const std::vector<int>& b){
        return a[1] < b[1];
    });
    
    long long currentTime = 0;
    std::vector<std::vector<int>> currentQueue;
    size_t pushIdx = 0;

    while(true)
    {
        // skip empty time
        if (currentQueue.empty())
        {
            if (pushIdx >= program.size()) break; // pass : undone program exist
            long long currentTime = program[pushIdx][callTime];
        }

        // push
        while (true)
        {
            if (pushIdx >= program.size()) break; // pass : pushIdx valid
            if (currentTime < program[pushIdx][callTime]) break; // pass : pushable time

            size_t insertLocation = 0;
            if (!currentQueue.empty()) 
            {
                insertLocation = binarySearch(0, currentQueue.size()-1, program[pushIdx], currentQueue);
            }
            if (insertLocation == currentQueue.size())
            {
                currentQueue.push_back(program[pushIdx]);
            } else {
                currentQueue.insert(currentQueue.begin() + insertLocation, program[pushIdx]);
            }
            answer[program[pushIdx][priority]] += currentTime - program[pushIdx][callTime]; // waited partially
            ++pushIdx;
        }

        // pop, current time update, delaytime update
        auto doneProgram = currentQueue.front();
        currentQueue.erase(currentQueue.begin());
        currentTime += doneProgram[cpuTime];
        for (auto prog : currentQueue)
        {
            answer[prog[priority]] += doneProgram[cpuTime]; // waited fully
        }
    }
    answer[0] = currentTime;

    return answer;
}

int main()
{
    {
        std::vector<std::vector<int>> program = { {2, 0, 10}, {1, 5, 5}, {3, 5, 3}, {3, 12, 2} };
        auto answer = solution(program);
        for (auto ans : answer) std::cout << ans << ' ';
        std::cout << std::endl;
    }
    {
        std::vector<std::vector<int>> program = { {3, 6, 4}, {4, 2, 5}, {1, 0, 5}, {5, 0, 5} };
        auto answer = solution(program);
        for (auto ans : answer) std::cout << ans << ' ';
        std::cout << std::endl;
    }
    
    return 0;
}