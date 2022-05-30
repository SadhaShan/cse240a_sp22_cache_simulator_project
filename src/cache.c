//========================================================//
//  cache.c                                               //
//  Source file for the Cache Simulator                   //
//                                                        //
//  Implement the I-cache, D-Cache and L2-cache as        //
//  described in the README                               //
//========================================================//

#include "cache.h"
#include <math.h>
#include <stdio.h>

//
// TODO:Student Information
//
const char *studentName = "Sadhana Shanmuga Sundaram";
const char *studentID   = "A59011077";
const char *email       = "sshanmugasundaram@ucsd.edu";

//------------------------------------//
//        Cache Configuration         //
//------------------------------------//

uint32_t icacheSets;     // Number of sets in the I$
uint32_t icacheAssoc;    // Associativity of the I$
uint32_t icacheHitTime;  // Hit Time of the I$

uint32_t dcacheSets;     // Number of sets in the D$
uint32_t dcacheAssoc;    // Associativity of the D$
uint32_t dcacheHitTime;  // Hit Time of the D$

uint32_t l2cacheSets;    // Number of sets in the L2$
uint32_t l2cacheAssoc;   // Associativity of the L2$
uint32_t l2cacheHitTime; // Hit Time of the L2$
uint32_t inclusive;      // Indicates if the L2 is inclusive

uint32_t blocksize;      // Block/Line size
uint32_t memspeed;       // Latency of Main Memory

uint32_t icacheSets_bits;
uint32_t dcacheSets_bits;
uint32_t l2cacheSets_bits;

uint32_t icacheTag_bits;
uint32_t dcacheTag_bits;
uint32_t l2cacheTag_bits;

uint32_t blocksize_bits;

//------------------------------------//
//          Cache Statistics          //
//------------------------------------//

uint64_t icacheRefs;       // I$ references
uint64_t icacheMisses;     // I$ misses
uint64_t icachePenalties;  // I$ penalties

uint64_t dcacheRefs;       // D$ references
uint64_t dcacheMisses;     // D$ misses
uint64_t dcachePenalties;  // D$ penalties

uint64_t l2cacheRefs;      // L2$ references
uint64_t l2cacheMisses;    // L2$ misses
uint64_t l2cachePenalties; // L2$ penalties

//------------------------------------//
//        Cache Data Structures       //
//------------------------------------//
//Data storage
//uint8_t*** icache_data;
//uint8_t*** dcache_data;
//uint8_t*** l2cache_data;

//Tag and Valid bit storage
uint32_t** icache_tag;
uint32_t** dcache_tag;
uint32_t** l2cache_tag;

//LRU bits storage
uint32_t** icache_lru;
uint32_t** dcache_lru;
uint32_t** l2cache_lru;


//------------------------------------//
//          Cache Functions           //
//------------------------------------//

// Initialize the Cache Hierarchy
//
void
init_cache()
{
  // Initialize cache stats
  icacheRefs        = 0;
  icacheMisses      = 0;
  icachePenalties   = 0;
  dcacheRefs        = 0;
  dcacheMisses      = 0;
  dcachePenalties   = 0;
  l2cacheRefs       = 0;
  l2cacheMisses     = 0;
  l2cachePenalties  = 0;
  
  int i,j,k;

/*
  icache_data = (uint8_t***)malloc(icacheSets * sizeof(uint8_t**));
  for (i = 0; i < icacheSets; ++i)
		icache_data[i] = (uint8_t**)malloc(icacheAssoc * sizeof(uint8_t*));
  for (i = 0; i < icacheSets; ++i) 
		for (j = 0; j < icacheAssoc; ++j)
			icache_data[i][j] = (uint8_t*)malloc(blocksize * sizeof(uint8_t));

  dcache_data = (uint8_t***)malloc(dcacheSets * sizeof(uint8_t**));
  for (i = 0; i < dcacheSets; ++i)
		dcache_data[i] = (uint8_t**)malloc(dcacheAssoc * sizeof(uint8_t*));
  for (i = 0; i < dcacheSets; ++i) 
		for (j = 0; j < dcacheAssoc; ++j)
			dcache_data[i][j] = (uint8_t*)malloc(blocksize * sizeof(uint8_t));

  l2cache_data = (uint8_t***)malloc(l2cacheSets * sizeof(uint8_t**));
    for (i = 0; i < l2cacheSets; ++i)
  		l2cache_data[i] = (uint8_t**)malloc(l2cacheAssoc * sizeof(uint8_t*));
    for (i = 0; i < l2cacheSets; ++i) 
  		for (j = 0; j < l2cacheAssoc; ++j)
  			l2cache_data[i][j] = (uint8_t*)malloc(blocksize * sizeof(uint8_t));
*/

  icache_tag = (uint32_t**)malloc(icacheSets * sizeof(uint32_t*));
    for (i = 0; i < icacheSets; ++i)
  		icache_tag[i] = (uint32_t*)malloc(icacheAssoc * sizeof(uint32_t));

  dcache_tag = (uint32_t**)malloc(dcacheSets * sizeof(uint32_t*));
    for (i = 0; i < dcacheSets; ++i)
  		dcache_tag[i] = (uint32_t*)malloc(dcacheAssoc * sizeof(uint32_t));

  l2cache_tag = (uint32_t**)malloc(l2cacheSets * sizeof(uint32_t*));
    for (i = 0; i < l2cacheSets; ++i)
  		l2cache_tag[i] = (uint32_t*)malloc(l2cacheAssoc * sizeof(uint32_t));
  
  for (i=0; i<icacheSets; ++i)
    for (j=0; j<icacheAssoc; ++j)
      icache_tag[i][j] = 0;
  
  for (i=0; i<dcacheSets; ++i)
    for (j=0; j<dcacheAssoc; ++j)
      dcache_tag[i][j] = 0;

  for (i=0; i<l2cacheSets; ++i)
    for (j=0; j<l2cacheAssoc; ++j)
      l2cache_tag[i][j] = 0;

  icache_lru = (uint32_t**)malloc(icacheSets * sizeof(uint32_t*));
  for (i = 0; i < icacheSets; ++i)
  		icache_lru[i] = (uint32_t*)malloc(icacheAssoc * sizeof(uint32_t));

  dcache_lru = (uint32_t**)malloc(dcacheSets * sizeof(uint32_t*));
  for (i = 0; i < dcacheSets; ++i)
  		dcache_lru[i] = (uint32_t*)malloc(dcacheAssoc * sizeof(uint32_t));

  l2cache_lru = (uint32_t**)malloc(l2cacheSets * sizeof(uint32_t*));
  for (i = 0; i < l2cacheSets; ++i)
  		l2cache_lru[i] = (uint32_t*)malloc(l2cacheAssoc * sizeof(uint32_t));

  for(i=0; i<icacheSets; ++i)
    for (j=0; j<icacheAssoc; ++j)
      icache_lru[i][j] = j;

  for(i=0; i<dcacheSets; ++i)
    for (j=0; j<dcacheAssoc; ++j)
      dcache_lru[i][j] = j;

  for(i=0; i<l2cacheSets; ++i)
    for (j=0; j<l2cacheAssoc; ++j)
      l2cache_lru[i][j] = j;

  icacheSets_bits = (uint32_t) log2((double)icacheSets);
  dcacheSets_bits = (uint32_t) log2((double)dcacheSets);
  l2cacheSets_bits = (uint32_t) log2((double)l2cacheSets);

  blocksize_bits = (uint32_t) log2((double)blocksize);

  icacheTag_bits = 32 - icacheSets_bits - blocksize_bits;
  dcacheTag_bits = 32 - dcacheSets_bits - blocksize_bits;
  l2cacheTag_bits = 32 - l2cacheSets_bits - blocksize_bits;

}

// Perform a memory access through the icache interface for the address 'addr'
// Return the access time for the memory operation
//
uint32_t
icache_access(uint32_t addr)
{
  if(icacheSets > 0){
    icacheRefs++;
    uint32_t curr_block_byte  = addr % blocksize;
    uint32_t curr_index       = (addr >> blocksize_bits) % icacheSets;
    uint32_t curr_tag         = (addr >> (icacheSets_bits + blocksize_bits));
  
    int i,j,k;
    for(i=0; i< icacheAssoc; ++i){
      if((icache_tag[curr_index][i] % 2) == 1){ //Cache has valid data
        if((icache_tag[curr_index][i] >> 1) == curr_tag){ //Tags matches
          for(j=0; j<icacheAssoc; ++j){
            if(icache_lru[curr_index][j] > icache_lru[curr_index][i])
              icache_lru[curr_index][j] -= 1;
          }
          icache_lru[curr_index][i] = icacheAssoc - 1;
          return icacheHitTime;
        }
      }
    }
  
    if(i == icacheAssoc){ // Data notfound in icache
      icacheMisses++;
      uint32_t miss_penalty = l2cache_access(addr);
      for(i=0; i<icacheAssoc; ++i){
        if((icache_tag[curr_index][i] % 2) == 0){  //Cache is invalid
          icache_tag[curr_index][i] = (curr_tag << 1) | 1;
        }
      }
      if(i == icacheAssoc){   //LRU Replacement
        for(i=0; i<icacheAssoc; ++i){
          if(icache_lru[curr_index][i] == 0){  //Cache is invalid
            icache_tag[curr_index][i] = (curr_tag << 1) | 1;
            icache_lru[curr_index][i] = icacheAssoc-1;
          }
          else
            icache_lru[curr_index][i] -= 1;
        }
      }
      icachePenalties += (miss_penalty);
      return (miss_penalty+icacheHitTime);
    }
  }
  else
    return l2cache_access(addr);
}

// Perform a memory access through the dcache interface for the address 'addr'
// Return the access time for the memory operation
//
uint32_t
dcache_access(uint32_t addr)
{
  if(dcacheSets > 0){
    dcacheRefs++;
    uint32_t curr_block_byte  = addr % blocksize;
    uint32_t curr_index       = (addr >> blocksize_bits) % dcacheSets;
    uint32_t curr_tag         = (addr >> (dcacheSets_bits + blocksize_bits));

    int i,j,k;
    for(i=0; i< dcacheAssoc; ++i){
      if((dcache_tag[curr_index][i] % 2) == 1){ //Cache has valid data
        if((dcache_tag[curr_index][i] >> 1) == curr_tag){ //Tags matches
          for(j=0; j<dcacheAssoc; ++j){
            if(dcache_lru[curr_index][j] > dcache_lru[curr_index][i])
              dcache_lru[curr_index][j] -= 1;
          }
          dcache_lru[curr_index][i] = dcacheAssoc - 1;
          return dcacheHitTime;
        }
      }
    }

    if(i == dcacheAssoc){ // Data notfound in dcache
      dcacheMisses++;
      uint32_t miss_penalty = l2cache_access(addr);
      for(i=0; i<dcacheAssoc; ++i){
        if((dcache_tag[curr_index][i] % 2) == 0){  //Cache is invalid
          dcache_tag[curr_index][i] = (curr_tag << 1) | 1;
        }
      }
      if(i == dcacheAssoc){   //LRU Replacement
        for(i=0; i<dcacheAssoc; ++i){
          if(dcache_lru[curr_index][i] == 0){  //Cache is invalid
            dcache_tag[curr_index][i] = (curr_tag << 1) | 1;
            dcache_lru[curr_index][i] = dcacheAssoc-1;
          }
          else
            dcache_lru[curr_index][i] -= 1;
        }
      }
      dcachePenalties += (miss_penalty);
      return (miss_penalty+dcacheHitTime);
    }
  }
  else
    return l2cache_access(addr);
}

// Perform a memory access to the l2cache for the address 'addr'
// Return the access time for the memory operation
//
uint32_t
l2cache_access(uint32_t addr)
{
  if(l2cacheSets > 0){
    l2cacheRefs++;
    uint32_t curr_block_byte  = addr % blocksize;
    uint32_t curr_index       = (addr >> blocksize_bits) % l2cacheSets;
    uint32_t curr_tag         = (addr >> (l2cacheSets_bits + blocksize_bits));

    int i=0,j=0,k=0;
    for(i=0; i< l2cacheAssoc; ++i){
      if((l2cache_tag[curr_index][i] % 2) == 1){ //Cache has valid data
        if((l2cache_tag[curr_index][i] >> 1) == curr_tag){ //Tags matches
          for(j=0; j<l2cacheAssoc; ++j){
            if(l2cache_lru[curr_index][j] > l2cache_lru[curr_index][i])
              l2cache_lru[curr_index][j] -= 1;
          }
          l2cache_lru[curr_index][i] = l2cacheAssoc - 1;
          return l2cacheHitTime;
        }
      }
    }

    if(i == l2cacheAssoc){ // Data notfound in l2cache
      l2cacheMisses++;
      uint32_t miss_penalty = memspeed;
      for(i=0; i<l2cacheAssoc; ++i){
        if((l2cache_tag[curr_index][i] % 2) == 0){  //Cache is invalid
          l2cache_tag[curr_index][i] = (curr_tag << 1) | 1;
        }
      }
      if(i == l2cacheAssoc){   //LRU Replacement
        for(i=0; i<l2cacheAssoc; ++i){
          if(l2cache_lru[curr_index][i] == 0){  //Cache is invalid
            l2cache_tag[curr_index][i] = (curr_tag << 1) | 1;
            l2cache_lru[curr_index][i] = l2cacheAssoc-1;
          }
          else
            l2cache_lru[curr_index][i] -= 1;
        }
      }
      l2cachePenalties += memspeed;
      return (miss_penalty+l2cacheHitTime);
    }
  }
  else
    return memspeed;
}
