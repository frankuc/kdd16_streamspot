/* 
 * Copyright 2016 Emaad Ahmed Manzoor
 * License: Apache License, Version 2.0
 * http://www3.cs.stonybrook.edu/~emanzoor/streamspot/
 */

#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <deque>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "hash.h"
#include "io.h"
#include "param.h"
#include "streamhash.h"

using namespace std;

void allocate_random_bits(vector<vector<uint64_t>>&, mt19937_64&, uint32_t);

int main(int argc, char *argv[]) {
  vector<vector<uint64_t>> H(L);                 // Universal family H, contains
                                                 // L hash functions, each
                                                 // represented by chunk_length+2
                                                 // 64-bit random integers

  mt19937_64 prng(SEED);                         // Mersenne Twister 64-bit PRNG

  uint32_t num_graphs=4;
  uint32_t chunk_length=256;

  vector<bitset<L>> streamhash_sketches(num_graphs);
  vector<vector<int>> streamhash_projections(num_graphs, vector<int>(L, 0));
 
  // set up universal hash family for StreamHash
  allocate_random_bits(H, prng, chunk_length);

  // construct StreamHash sketches for bootstrap graphs offline
  cout << "Constructing StreamHash sketches for training graphs:" << endl;
  unordered_map<string,uint32_t> map1;
  unordered_map<string,uint32_t> map2;
  unordered_map<string,uint32_t> map3;
  unordered_map<string,uint32_t> map4;
  map1["abc"] = 1;
  map1["def"] = 1;
  map1["def1"] = 1;
  
  map1["def2"] = 1;
  map1["def3"] = 1;
  map1["def4"] = 1;
 
  map2["yyy"] = 1;
  map2["kkk"] = 1;
  map2["jjj"] = 1;
  
  map3["abc"] = 1;
  map3["bcd"] = 1;
  map3["bck"] = 1;
  map3["bckt"] = 1;
 
  map4["abc"] = 1;
  map4["bcd"] = 1;
  map4["bcky"] = 1;
  
  //map2["abc"] = 1;
  //map2["def"] = 1;
  
  //map2["abc"] = 1;
  //map2["def"] = 1;
  //map2["def1"] = 1;
  //map2["def2"] = 1;
  // map1.insert( { {"abc":1}, {"def":1} } );
  // map2.insert( { {"abc":1}, {"def":1} } );
  
  tie(streamhash_sketches[0], streamhash_projections[0]) = construct_streamhash_sketch(map3, H);
  tie(streamhash_sketches[1], streamhash_projections[1]) = construct_streamhash_sketch(map4, H);
  
  double sim=0.0;
  sim = streamhash_similarity(streamhash_sketches[0], streamhash_sketches[1]);
  for(int i=0; i<L; i++) {
   cout<<streamhash_sketches[0][i];
  }
  cout<<endl;
  cout<< sim << endl;
  vector<string> out = get_streamhash_index(streamhash_sketches[0], 50, 40);
  for(uint32_t i=0; i<out.size(); i++)
    cout << out[i] << "|";
  cout<<endl;

  return 0;
}

void allocate_random_bits(vector<vector<uint64_t>>& H, mt19937_64& prng,
                          uint32_t chunk_length) {
  // allocate random bits for hashing
  for (uint32_t i = 0; i < L; i++) {
    // hash function h_i \in H
    H[i] = vector<uint64_t>(chunk_length + 2);
    for (uint32_t j = 0; j < chunk_length + 2; j++) {
      // random number m_j of h_i
      H[i][j] = prng();
    }
  }
#ifdef DEBUG
    cout << "64-bit random numbers:\n";
    for (int i = 0; i < L; i++) {
      for (int j = 0; j < chunk_length + 2; j++) {
        cout << H[i][j] << " ";
      }
      cout << endl;
    }
#endif
}

