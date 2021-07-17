# Cache_Simulator
Building a software which simulates cache(fully associative,set associative and direct mapped) based on random, LRU and pseudo LRU replacement policies.<br />
## Input Format<br />
Cache size in Bytes<br /> Block size in Bytes<br /> Associativity: 0 for fully associative,
1 for direct-mapped, 2/4/8/16/32(associativity) for set-associative<br /> Replacement Policy: 0 for
random, 1 for LRU, 2 for Pseudo-LRU<br /> Name of file containing memory traces (each entry
containing 8-digit Hex-decimal number) and the request type(read/write).<br />
## Output<br />
Cache Size<br /> Block Size<br /> Type of Cache (fully
associative/set-associative/direct-mapped)<br /> Replacement Policy<br /> Number of Cache
Accesses<br /> Number of Read Accesses<br /> Number of Write Accesses<br /> Number of Cache
Misses<br /> Number of Read Misses<br /> Number of Write Misses<br /> Number of Dirty
Blocks Evicted
