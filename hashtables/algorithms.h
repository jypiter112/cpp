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
    /*
        Time complexity: O(log n)

        Modify accuracy with step and tolerance.
        Tolerance is step - 1 decimal place.
        Quarenteed accuracy is step - 1 decimal place.

        Improvement ideas:
        Could use a static table of known common square root values?
    */
    double mysqrt(const double& num){
        if (num < 0) throw std::out_of_range("Complex numbers are not supported");
        double l = 0;
        const double step = 0.001;
        const double tolerance = 0.01;
        double r = num;
        // find the square root with binary search
        while (l <= r) {
            double mid = l + (r - l) / 2;
            if ((mid * mid) < (num + tolerance) && (mid*mid) > (num - tolerance)) return mid;
            if (mid*mid < num) {
                l = mid + step;
            }
            else {
                r = mid - step;
            }
        }
        // exception
        throw std::exception("Something went wrong");
        // unreachable
        return double(-1);
    }
}
