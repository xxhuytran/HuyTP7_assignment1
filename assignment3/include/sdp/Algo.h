#pragma once
#include <cstddef>
#include <vector>

namespace sdp {
namespace algo {

namespace detail {
template <typename T, typename Less>
void mergeRange(std::vector<T>& v, std::vector<T>& tmp, std::size_t lo, std::size_t mid,
                std::size_t hi, Less less) {
    std::size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (less(v[j], v[i])) {
            tmp[k++] = v[j++];
        } else {
            tmp[k++] = v[i++];  // bang nhau thi lay ben trai -> on dinh
        }
    }
    while (i < mid) tmp[k++] = v[i++];
    while (j < hi) tmp[k++] = v[j++];
    for (std::size_t x = lo; x < hi; ++x) v[x] = tmp[x];
}

template <typename T, typename Less>
void sortRange(std::vector<T>& v, std::vector<T>& tmp, std::size_t lo, std::size_t hi, Less less) {
    if (hi - lo < 2) return;
    std::size_t mid = lo + (hi - lo) / 2;
    sortRange(v, tmp, lo, mid, less);
    sortRange(v, tmp, mid, hi, less);
    mergeRange(v, tmp, lo, mid, hi, less);
}
}  // namespace detail

// Merge sort, O(n log n). less(a, b) = true neu a dung truoc b
template <typename T, typename Less>
void mergeSort(std::vector<T>& v, Less less) {
    std::vector<T> tmp(v.size());
    detail::sortRange(v, tmp, 0, v.size(), less);
}

// Tim nhi phan tren vector da sort tang dan theo key. Tra index hoac -1
// keyOf(item) lay key cua phan tu
template <typename T, typename Key, typename KeyOf>
int binarySearch(const std::vector<T>& v, const Key& key, KeyOf keyOf) {
    int lo = 0;
    int hi = static_cast<int>(v.size()) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (keyOf(v[mid]) < key) {
            lo = mid + 1;
        } else if (key < keyOf(v[mid])) {
            hi = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}

}  // namespace algo
}  // namespace sdp
