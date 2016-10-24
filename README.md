# measurement for numa architecture

## number of node
2node

## lscpu
<pre><code>Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                36
On-line CPU(s) list:   0-35
Thread(s) per core:    1
Core(s) per socket:    18
Socket(s):             2
NUMA node(s):          2
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 79
Model name:            Intel(R) Xeon(R) CPU E5-2695 v4 @ 2.10GHz
Stepping:              1
CPU MHz:               2600.390
BogoMIPS:              4208.34
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              256K
L3 cache:              46080K
NUMA node0 CPU(s):     0-17
NUMA node1 CPU(s):     18-35</code></pre>

## numastat
<pre><code>                           node0           node1
numa_hit                67798958        57342158
numa_miss                      0               0
numa_foreign                   0               0
interleave_hit           5451975         5605664
local_node              67791294        51731637
other_node                  7664         5610521
</code></pre>

## numactl -H
<pre><code>available: 2 nodes (0-1)
node 0 cpus: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
node 0 size: 130958 MB
node 0 free: 121570 MB
node 1 cpus: 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
node 1 size: 131072 MB
node 1 free: 126148 MB
node distances:
node   0   1 
  0:  10  21 
  1:  21  10 
</code></pre>


## numactl --show
<pre><code>policy: default
preferred node: current
physcpubind: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 
cpubind: 0 1 
nodebind: 0 1 
membind: 0 1 
</code></pre>


## free -m
<pre><code>              total        used        free      shared  buff/cache   available
Mem:         257676        3618      204300          19       49757      253438
Swap:         20003           0       20003
</code></pre>


## Intel(R) Xeon(R) CPU E5-2695 v4
[Intel® Xeon® Processor E5-2695 v4](http://ark.intel.com/products/91316/Intel-Xeon-Processor-E5-2695-v4-45M-Cache-2_10-GHz)

<pre><code>of Cores	18
of Threads	36
Processor Base Frequency	2.10 GHz
Max Turbo Frequency	3.30 GHz
Cache	45 MB SmartCache
Bus Speed	9.6 GT/s QPI
Max Memory Size (dependent on memory type)	1.54 TB
Memory Types	DDR4 1600/1866/2133/2400
Max # of Memory Channels	4
Max Memory Bandwidth	76.8 GB/s
Physical Address Extensions	46-bit
ECC Memory Supported ‡ 	Yes
</code></pre>











