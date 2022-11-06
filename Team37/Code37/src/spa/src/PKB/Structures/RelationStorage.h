#pragma once

#ifndef SPA_RELATION_STORAGE_H
#define SPA_RELATION_STORAGE_H

#include <map>
#include <vector>

using namespace std;

template <typename S, typename T>
class RelationStorage {
protected:
    multimap<S, T> forwardStore = {};
    multimap<T, S> reverseStore = {};

public:
    // Store
    void store(S index1, T index2)
    {
        forwardStore.insert(pair<S, T>(index1, index2));
        reverseStore.insert(pair<T, S>(index2, index1));
    }

    // Retrieve 1
    bool retrieve(S index1, T index2)
    {
        auto itr1 = forwardStore.lower_bound(index1);
        auto itr2 = forwardStore.upper_bound(index1);
        while (itr1 != itr2) {
            if (itr1->first == index1) {
                if (itr1->second == index2) {
                    return true;
                }
            }
            itr1++;
        }
        return false;
    }

    // Forward retrieve
    vector<T> forwardRetrieve(S index1)
    {
        vector<T> output;

        auto itr1 = forwardStore.lower_bound(index1);
        auto itr2 = forwardStore.upper_bound(index1);
        while (itr1 != itr2) {
            if (itr1->first == index1) {
                output.push_back(itr1->second);
            }
            itr1++;
        }
        return output;
    }

    // Reverse
    vector<S> reverseRetrieve(T index2)
    {
        vector<S> output;
        auto itr1 = reverseStore.lower_bound(index2);
        auto itr2 = reverseStore.upper_bound(index2);
        while (itr1 != itr2) {
            if (itr1->first == index2) {
                output.push_back(itr1->second);
            }
            itr1++;
        }
        return output;
    }
};

#endif