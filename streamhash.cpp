/* 
 * Copyright 2016 Emaad Ahmed Manzoor
 * License: Apache License, Version 2.0
 * http://www3.cs.stonybrook.edu/~emanzoor/streamspot/
 */

#include <bitset>
#include "hash.h"
#include "param.h"
#include "streamhash.h"
#include <tuple>
#include <unordered_map>
#include <vector>
#include <string>

namespace std {

double streamhash_similarity(const bitset<L>& sketch1, const bitset<L>& sketch2) {
  // XOR = 0 if bits are the same, 1 otherwise
  return static_cast<double>((~(sketch1 ^ sketch2)).count()) / L;
}

tuple<bitset<L>,vector<int>>
construct_streamhash_sketch(const unordered_map<string,uint32_t>& shingle_vector,
                            const vector<vector<uint64_t>>& H) {
  bitset<L> sketch;
  vector<int> projection(L, 0);

  for (auto& kv : shingle_vector) {
    auto& shingle = kv.first;
    auto& count = kv.second;
    for (uint32_t i = 0; i < L; i++) {
      projection[i] += count * hashmulti(shingle, H[i]);
    }
  }

  for (uint32_t i = 0; i < L; i++) {
    sketch[i] = projection[i] >= 0 ? 1 : 0;
  }

  return make_tuple(sketch, projection);
}

 vector<string> get_streamhash_index(bitset<L> sketch,uint32_t rows, uint32_t burkets) {
  vector<string> hash_index;
  bitset<L> mask = bitset<L>(string(rows, '1'));  // rows one's
  for (uint32_t i = 0; i < burkets; i++) {
    // get the i'th rows-bit band
    string band_string = (sketch >> (rows * i) & mask).to_string();
    band_string = band_string.substr(band_string.length() - rows, rows);
    hash_index.push_back(to_string(i) + string("_") + band_string);
    // bitset<rows> band(band_string);
  }
 
 return hash_index;
}

}
