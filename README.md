# getCDS  
Produces one fasta file for each coding loci (only exons) found in the corresponding gff file.  
Also produces a txt file containing one line per exons: locus_name, contig_name, first position of exon (1-based), last position of exon, strand (+ or -)  
  
## compilation  
g++ getCDS.cpp -std=c++17 -O3 -o getCDS  
  
## exemple:  
./getCDS contig_Hmel201012_ama.fasta Hmel2.gff  
**arg1:** fasta file  
**arg2:** gff file  
  
The fasta file is an alignement of one contig for different individuals.  
The gff file can be the gff produced for the whole genome.  
