#include <thread>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallel_acumulate(Iterator first, Iterator last, T init)
{
    int length = std::distance(first, last);
    if (!length)
        return init;
    unsigned long const cNum = std::thread::hardware_concurrency();
    unsigned long const nm = std::ceil((length / 25));
    auto num_threads = std::min(cNum, nm);
    auto block_size = length / num_threads;
    std::vector<T> results(num_threads, 0);
    std::vector<std::thread> threads(num_threads);
    Iterator block_start = first;
    for (unsigned long i = 0; i < num_threads - 1; ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    threads[num_threads - 1] = std::thread(accumulate_block<Iterator, T>(), block_start, last, std::ref(results[num_threads - 1]));

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(results.begin(), results.end(), init);
}

#include <chrono>

int main()
{
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; i++)
    {
        std::vector<int> nums(10000000, 1);
        auto rst = parallel_acumulate<std::vector<int>::iterator, int>(nums.begin(), nums.end(), 0);
    }
    auto cur1 = std::chrono::high_resolution_clock::now();
    std::cout << "My Acc: " << std::chrono::duration_cast<std::chrono::milliseconds>(cur1 - begin).count() << std::endl;
    for (int i = 0; i < 10; i++)
    {
        std::vector<int> nums(10000000, 1);
        auto rst = std::accumulate(nums.begin(), nums.end(), 0);
    }
    auto cur2 = std::chrono::high_resolution_clock::now();
    std::cout << "System Acc: " << std::chrono::duration_cast<std::chrono::milliseconds>(cur2 - cur1).count() << std::endl;
}