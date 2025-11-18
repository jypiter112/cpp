# String pattern finding from a PE file
Idea
- Calculate number of matches of a vector of strings from a PE (or any) file
- Find flagged flagged patterns
Use case
- Static malware detection scan

Compiled with
- Platform: Windows 11 laptop
- Compiler: clang++
'''
clang version 18.1.8
Target: x86_64-pc-windows-msvc
Thread model: posix
'''
- lib=c++17

Tested with chrono::high_resolution_clock

## Variable defenitions
Time complexity
- n: length of text
- m: length of pattern
- k: vector elements

Vector random access
- Const: O(1)

Pre estimated time taken

## Hypothesis
Based on the time complexities of the look up algorithm. HashMaps should be way faster
- Though, HashMap creation is much slower than a "long string"

## TEST 1: Long string boyer lookup
Time complexity hypothesis
- Worst case: O(nm*k)
- Best case: O(K*(n/m))

### 1.1 Without long string creation
Actual run time:
800-1600 micro seconds

### 1.2 With long string creation
Actual run time:
11200-13600 micro seconds



## TEST 2: Unordered_set::find
Time complexity hypothesis
- O(k)

### 2.1 Without unordered_set creation
Actual run time:
750 micro seconds

### 2.2 With unrodered_set creation
Actual run time:
21000-51000 micro seconds

## Conclusions
Based on five test runs

Best case
HashMap 1.85 times slower

Worst case
HashMap 3.75 times slower

Boyer algorithm with long string
- Faster for more files

HashMap algorithm
- Faster for bigger and fewer files

## Sources
Book: C++17 in detail
- By B. Filipek 
- Published: 2019-03-01

https://en.cppreference.com/w/cpp/container/vector.htmls
- Vector random access time complexity reference

Other sources used in
- strings.cpp source code
