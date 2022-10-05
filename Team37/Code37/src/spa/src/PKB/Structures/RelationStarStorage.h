#pragma once

#ifndef SPA_RELATION_STORAGE_STAR_H
#define SPA_RELATION_STORAGE_STAR_H

#include <unordered_set>
#include <unordered_map>
#include <map>
#include "RelationStorage.h"

using namespace std;

template<typename S, typename T>
class RelationStarStorage : public RelationStorage<S, T> {
public:
    // Store
    RelationStarStorage<S, T> buildStar() {
        RelationStarStorage<S, T> output = RelationStarStorage<S, T>();


        for (typename multimap<S, T>::iterator itr = this->forwardStore.begin(); itr != this->forwardStore.end();) {
            S key = itr->first;
            unordered_map<T, bool> memo;
            unordered_set<T> workingSet;
            vector<T> initalRelation = this->forwardRetrieve(key);

            for (T x: initalRelation) {
                workingSet.insert(x);
            }

            while (!workingSet.empty()) {
                T currVal = *workingSet.begin();
                workingSet.erase(workingSet.begin());
                output.store(key, currVal);

                // Add all that forwardRetreive
                vector<T> nextVals = this->forwardRetrieve(currVal);

                for (T x: nextVals) {
                    // Check if done before
                    if (memo[x]) {
                        continue;
                    }
                    workingSet.insert(x);
                    memo[x] = true;
                }
            }


            // Till the next key
            typename multimap<S, T>::iterator curr = itr;
            while (itr != this->forwardStore.end() && itr->first == curr->first)
                ++itr;
        }

        return output;
    }

};

#endif