#pragma once

#ifndef SPA_RELATION_STORAGE_STAR_H
#define SPA_RELATION_STORAGE_STAR_H

#include <unordered_set>
#include <unordered_map>
#include <map>
#include "RelationStorage.h"
using namespace std;

template <typename S, typename T> class RelationStarStorage : public RelationStorage<S,T> {
public:
    // Store
	RelationStarStorage<S,T> buildStar() {
		RelationStarStorage<S, T> output = RelationStarStorage<S, T>();


		for (multimap<S, T>::iterator itr = forwardStore.begin(); itr != forwardStore.end(); ) {
			S key = itr->first;
			unordered_map<T, bool> memo;
			unordered_set<T> workingSet;

			vector<T> initalRelation = forwardRetrieve(key);

			for (T x : initalRelation) {
				workingSet.insert(x);
			}

			while (!workingSet.empty()) {
				T currVal = *workingSet.begin();
				workingSet.erase(workingSet.begin());
				output.store(key, currVal);

				// Add all that forwardRetreive
				vector<T> nextVals = forwardRetrieve(currVal);

				for (T x : nextVals) {
					// Check if done before
					if (memo[x]) {
						continue;
					}
					workingSet.insert(x);
					memo[x] = true;
				}
			}


			// Till the next key
			multimap<S, T>::iterator curr = itr;
			while (itr != forwardStore.end() && itr->first == curr->first)
				++itr;
		}

		return output;
    }

};

#endif