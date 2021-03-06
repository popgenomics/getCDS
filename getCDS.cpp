#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

// g++ getCDS.cpp -std=c++17 -O3 -o getCDS

void printFasta(const std::vector <std::string> sequences, const std::vector <std::string> names);
void getContigName(const std::string line, std::string & contigName, size_t & test);
void testContig(const std::string & line, const std::string targetContig, int & goodContig);
void initialize(std::vector <std::string> & indSequences_tmp, const std::vector <std::string> & indSequences);
void treatLine(const std::string & line, const std::vector <std::string> & indSequences, const std::vector <std::string> & indNames, int & testCDS, int & locus_ID, std::vector <std::string> & indSequences_tmp, const std::string contigName);
void checkCommandLine(const int argc);

int main(int argc, char* argv[]){

	checkCommandLine(argc);

	const std::string fastaFile(argv[1]);
	const std::string gffFile(argv[2]);

	int goodContig(0);
	size_t test(0);
	int seqID(-1);
	std::string contigName;

	std::vector <std::string> indSequences;
	std::vector <std::string> indNames;
	std::string line;
	std::ifstream infileFasta( fastaFile.c_str() );

	if( infileFasta ){
		while(std::getline( infileFasta, line )){
			if( line[0] == '>' ){
				if( test==0 ){
					getContigName(line, contigName, test);
				}

				++seqID;
				indNames.push_back(line);
				indSequences.push_back("");
			}else{
				indSequences[seqID].append(line);
			}
		}
		infileFasta.close();
	}else{
		std::cout << "ERROR: the file " << fastaFile << " was not found" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::ifstream infileGFF( gffFile.c_str() );
	
	if( infileGFF ){
		int testCDS(0);
		int locus_ID(0);
		// contains vector of CODING sequences subsample from indSequences
		std::vector <std::string> indSequences_tmp;

		// initialize indSequences_tmp: set the same length than indSequences
		initialize(indSequences_tmp, indSequences);

		while(std::getline( infileGFF, line )){
			if( line[0] == '#' ){
				continue;
			}else{
				testContig(line, contigName, goodContig); // test if the GFF line fits with the targeted locus 
				if( goodContig==1 ){
					//std::cout << line << std::endl;
					treatLine(line, indSequences, indNames, testCDS, locus_ID, indSequences_tmp, contigName);
				}else{
					continue;
				}
			}
		}

		infileGFF.close();
	}else{
		std::cout << "ERROR: the file " << gffFile << " was not found" << std::endl;
		exit(EXIT_FAILURE);	
	}


	//printFasta(indSequences, indNames);
	//std::cout << indSequences.size() << std::endl;
	//std::cout << indNames.size() << std::endl;

	return(0);
}

void printFasta(const std::vector <std::string> sequences, const std::vector <std::string> names){
	const size_t nSeq(sequences.size());
	size_t i(0);
	
	for(i=0; i<nSeq; ++i){
		std::cout << names[i] << std::endl << sequences[i] << std::endl;
	}
}

void getContigName(const std::string line, std::string & contigName, size_t & test){

	size_t i(0);
	std::istringstream iss(line);
	std::string word;
	
	while( std::getline( iss, word, '|') ){
//		std::cout << word << std::endl;
		if( i==0 ){
			contigName = word.substr(1);
		}
	++i;
	}

	test=1;
}

void testContig(const std::string & line, const std::string targetContig, int & goodContig){
	size_t i(0);

	std::istringstream iss(line);
	std::string word;

	while( getline( iss, word, '\t') ){
		++i;
		if( i== 1){
			if( word == targetContig ){
				goodContig=1;
			}else{
				goodContig=0;
			}
			return;
		}
	}
}

void initialize(std::vector <std::string> & indSequences_tmp, const std::vector <std::string> & indSequences){
	for(size_t i=0; i<indSequences.size(); ++i){
		indSequences_tmp.push_back("");
	}
}


void treatLine(const std::string & line, const std::vector <std::string> & indSequences, const std::vector <std::string> & indNames, int & testCDS, int & locus_ID, std::vector <std::string> & indSequences_tmp, const std::string contigName){
	size_t i(0);
	size_t j(0);
	size_t start(0);
	size_t end(0);
	char strand;

	std::string exon;

	std::istringstream iss(line);
	std::string word;

	std::string outputName_fasta(""); // name of the output file 
	std::string outputName_txt(""); // name of the output file 

	while( getline(iss, word, '\t') ){
		++i;
		if( i == 3 ){
			if( word == "CDS" ){
				if( testCDS == 0 ){
					testCDS=1;
					++locus_ID;
					for(j=0; j<indSequences.size(); ++j){
						indSequences_tmp[j] = "";
					}
				}
				std::ostringstream oss1;
				oss1 << contigName << "_" << locus_ID << ".fasta";	
				outputName_fasta = oss1.str();
				
				std::ostringstream oss2;
				oss2 << contigName << "_" << locus_ID << ".txt";	
				outputName_txt = oss2.str();

			}else{
				testCDS=0;
				return;
			}
		}
		if( i == 4 ){
			start = std::stoi(word);
		}

		if( i == 5 ){
			end = std::stoi(word);
		}
		
		if( i == 7 ){
			strand = word[0];

			std::ofstream fastaFile;
			fastaFile.open(outputName_fasta.c_str(), std::ios::out);

			std::ofstream txtFile;
			txtFile.open(outputName_txt.c_str(), std::ios::app);

			for(j=0; j<indSequences.size(); ++j){
				exon = indSequences[j].substr(start-1, end-start+1);
				if(strand == '+'){
					//indSequences_tmp[j].append(indSequences[j], start-1, end-start+1);
					indSequences_tmp[j].append(exon);
				}else{
					indSequences_tmp[j].insert(0, exon);
				}
				fastaFile << indNames[j] << std::endl;
				fastaFile << indSequences_tmp[j] << std::endl;

				if( j==0 ){
					txtFile << contigName << "_" << locus_ID << "\t" << contigName << "\t" << start << "\t" << end << "\t" << strand << std::endl;
				}
			}

			fastaFile.close();
			txtFile.close();
			return;
		}
	}
}

void checkCommandLine(const int argc){
	if( argc != 3 ){
		std::cout << std::endl << " getCDS produces coding sequences (concatenated CDS) from a gff and a fasta file." << std::endl;
		std::cout << " in the current version: the original fasta file is only for one contig, but gff can contains informations for all contigs" << std::endl;
		std::cout << " 2 arguments are needed:" << std::endl;
		std::cout << "\tname of the fasta file (string)." << std::endl;
		std::cout << "\tname of the gff file (string)." << std::endl;
		std::cout << "\t\t./getCDS contig_Hmel201012_ama.fasta Hmel2.gff" << std::endl << std::endl;
		std::cout << "\tcamille.roux.1983@gmail.com (22/06/2017)" << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
}

