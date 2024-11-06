#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "printVec.h"

template <typename Comparable>
void quickSortUnstableRec(vector<Comparable> &vec, int startIndex, int endIndex,
                          Comparable &partition, int &i,
                          int &largerElementIndex, Comparable &temp,
                          unsigned long &reads, unsigned long &allocations) {

        allocations += sizeof(vector<Comparable> *); // vec
        allocations += sizeof(int *) * 2;            // i, largerElementIndex
        allocations += sizeof(Comparable *);         // temp
        allocations += sizeof(int) * 2;              // start index end index
        allocations += sizeof(unsigned long *);      // allocations reads

        // Recursive base case
        if (startIndex >= endIndex) {
                return;
        }

        // Choose a partition element
        reads++;
        partition = vec[startIndex];

        // Loop through vec from startIndex to endIndex
        // Keep track of where the > partition elements start
        largerElementIndex = startIndex + 1;
        for (i = startIndex + 1; i <= endIndex; ++i) {
                reads++;
                if (vec[i] <= partition) {
                        reads += 2;
                        // Swap the smaller/equal item to the left of the larger
                        // items
                        temp = vec[i];
                        vec[i] = vec[largerElementIndex];
                        vec[largerElementIndex] = temp;
                        // Update largerElementIndex
                        ++largerElementIndex;
                }
        }
        // Swap the partition element into place
        if (startIndex != largerElementIndex - 1) {
                reads += 2;
                temp = vec[startIndex];
                vec[startIndex] = vec[largerElementIndex - 1];
                vec[largerElementIndex - 1] = temp;
        }

        // Uncomment this line if you want to see each iteration
        // printVec(vec);

        // Recursive calls for two halves
        quickSortUnstableRec(vec, startIndex, largerElementIndex - 2, partition,
                             i, largerElementIndex, temp, reads, allocations);
        quickSortUnstableRec(vec, largerElementIndex, endIndex, partition, i,
                             largerElementIndex, temp, reads, allocations);
}

template <typename Comparable>
vector<Comparable> quickSortUnstable(vector<Comparable> vec,
                                     unsigned long &reads,
                                     unsigned long &allocations) {
        reads = allocations = 0;
        Comparable partition, temp;
        int i, largerElementIndex;

        allocations += sizeof(Comparable) * 2;          // partition temp
        allocations += sizeof(int) * 2;                 // i, largerElementIndex
        allocations += sizeof(unsigned long *) * 2;     // allocations, reads

        quickSortUnstableRec(vec, 0, vec.size() - 1, partition, i,
                             largerElementIndex, temp, reads, allocations);
        return vec;
}

template <typename Comparable>
void quickSortStableRec(vector<Comparable> &vec, unsigned long &reads,
                        unsigned long &allocations) {

        allocations += sizeof(unsigned long *) * 2;  // reads allocations
        allocations += sizeof(vector<Comparable> *); // vec

        // Recursive base case
        if (vec.size() <= 1) {
                return;
        }

        // Choose a partition element
        reads++;
        Comparable partition = vec[0];

        allocations += sizeof(vector<Comparable>) * 3;
        vector<Comparable> smaller, equal, larger;
        // Loop through vec and populate smaller, equal, larger
        int i;
        allocations += sizeof(int);
        for (i = 0; i < vec.size(); ++i) {

                // 2, 3, 3 because you have to read each time to evaluate the
                // conditions
                if (vec[i] < partition) {
                        reads += 2;
                        smaller.push_back(vec[i]);
                } else if (vec[i] > partition) {
                        reads += 3;
                        larger.push_back(vec[i]);
                } else {
                        reads += 3;
                        equal.push_back(vec[i]);
                }
        }
        allocations += sizeof(Comparable) *
                       (smaller.size() + larger.size() + equal.size());

        // Recursive calls
        quickSortStableRec(smaller, reads, allocations);
        quickSortStableRec(larger, reads, allocations);

        // Copy elements from smaller, equal, and larger back into vec
        for (i = 0; i < vec.size(); ++i) {
                if (i < smaller.size()) {
                        reads++;
                        vec[i] = smaller[i];
                } else if (i < smaller.size() + equal.size()) {
                        reads++;
                        vec[i] = equal[i - smaller.size()];
                } else {
                        reads++;
                        vec[i] = larger[i - smaller.size() - equal.size()];
                }
        }

        // Uncomment this line if you want to see each iteration
        // printVec(vec);
}

template <typename Comparable>
vector<Comparable> quickSortStable(vector<Comparable> vec, unsigned long &reads,
                                   unsigned long &allocations) {
        reads = allocations = 0;

        allocations += sizeof(unsigned long *) * 2;     // allocations, reads

        quickSortStableRec(vec, reads, allocations);
        return vec;
}

#endif
