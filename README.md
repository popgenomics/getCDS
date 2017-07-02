# getCDS  
Produces one fasta file for each coding loci (only exons) found in the corresponding gff file.  
Also produces a txt file containing one line per exons: locus_name, contig_name, first position of exon (1-based), last position of exon, strand (+ or -)  
  
## Compilation  
g++ getCDS.cpp -std=c++17 -O3 -o getCDS  
  
## Exemple:  
./getCDS contig_Hmel201012_ama.fasta Hmel2.gff  
**arg1:** fasta file  
**arg2:** gff file  
  
The fasta file is an alignement of one contig for different individuals.  
The gff file can be the gff produced for the whole genome.  
  
## Precaution  
The produced **fasta files** are written in 'std::ios::out mode'.  
The produced **txt files** are written in 'std::ios::app (for append) mode'.  
Running multiple times getCDS with the same fasta input file will overwrite the written fasta output files, but will append informations written in the correspond txt output files  
The txt file contains the contig, the position and the strand.  
If the strand is minus, the fasta file will need to be reverse_complemented.  

