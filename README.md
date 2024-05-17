# Sparse Suffix Array based on Minimizers
Implementation of sparse suffix array based on sampling minimizers

Download and extract the a sample genome sequence from [link](https://ftp.ensembl.org/pub/release-111/fasta/homo_sapiens/dna/Homo_sapiens.GRCh38.dna.alt.fa.gz). A sample of queries is provided in the file `sampled_substrings_1M.txt`, but you can generate your own samples with arbitrary length and reference size using the code from `generate_queries.cpp`.

Implementation of the sparse suffix array is available in the file `suffix-array.cpp`, which you can run using the example script `_bash.sh`.

The arguments given to the code are as follow:

```g++ -std=c++17 suffix-array.cpp {k} {w} [optional*]{ord func} [optional*]{prefix lookup} [optional*]{ref_file} [optional*]{query_file}```.

1. k-mer size ($k$)
2. Window size ($w$)
3. Ordering function (default=1):
    - 0 : lexicographical
    - 1 : random
    - 2 : reverse lexicographical
4. Prefix lookup table for queries (default=0):
    - 0 : Don't use prefix lookup
    - 1 : Use prefix lookup
5. Path to reference string file
6. Path to query file

Some results for density and query time of our code for different values of $k$ and $w$ is provided in the directories `results/` and `plots/`.
