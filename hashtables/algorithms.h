#pragma once
#include <iostream>
#include <optional>
#include <vector>

/*
    Returns the index of the target value
*/
namespace Algorithms {
    template<typename type>
    int binarySearch(std::vector<type>& vector, type target){
        int left { 0 };
        int right { static_cast<int>(vector.size()) - 1 };
        while(left <= right){
            int mid = left + (right - left) / 2;
            if(vector[mid] == target)
                return mid;
            if(vector[mid] < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
        // no value
        return -1;
    }
}
