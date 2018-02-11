/* vim:set noexpandtab tabstop=4 wrap */
#include "MRDspecs.hh"
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
//#include <iomanip>
//#include <cstdlib>  // for getenv for standalone call

//.x /annie/app/users/moflaher/wcsim/root_work/RegexTest.C+    << standalone call

//// FIXME: RESULTS RETURNED FROM STRIPMRDPOSITIONS ARE IN MM!
//// EVERYTHING ELSE HERE IS IN CM. THIS IS CONFUSING.

// declare the output vectors to put things in temporarily
static std::vector<int> temp_paddle_orientations(MRDSpecs::nummrdpmts);	// H is 0, V is 1
static std::vector<int> temp_paddle_layers(MRDSpecs::nummrdpmts);
static std::vector<double> temp_paddle_originx(MRDSpecs::nummrdpmts);
static std::vector<double> temp_paddle_originy(MRDSpecs::nummrdpmts);
static std::vector<double> temp_paddle_originz(MRDSpecs::nummrdpmts);
static std::vector<std::pair<double,double> > temp_paddle_extentsx(MRDSpecs::nummrdpmts);
static std::vector<std::pair<double,double> > temp_paddle_extentsy(MRDSpecs::nummrdpmts);
static std::vector<std::pair<double,double> > temp_paddle_extentsz(MRDSpecs::nummrdpmts);

int StripMrdPositions(){

//	temp_paddle_orientations;
//	temp_paddle_layers.;
//	temp_paddle_originx;
//	temp_paddle_originy;
//	temp_paddle_originz;
//	temp_paddle_extentsx;
//	temp_paddle_extentsy;
//	temp_paddle_extentsz;
	
	//char* pwd;
	//pwd = getenv ("PATH");
	//std::string rawfilename = std::string(pwd) + "/MRD_positions_raw";
	
	// input file reading infrastructure
	//std::ifstream input_file(fname);
	/* version that reads from file. Needs MRD_positions_raw to be in the same directory as executed.
	std::ifstream input_file(rawfilename);
		if ( input_file.fail() ) {
		std::cerr << "Failed to open file " << rawfilename << "!" << endl;
		exit(-1);
	}
	char linebuffer[256];
	std::string fileline;
	std::vector<std::string> filelines;
	
	// read the file and put the lines into a vector of std::strings
	while ( !input_file.eof() ) {
		input_file.getline(linebuffer,256);
		//istringstream streamconverter(linebuffer);
		//streamconverter >> fileline;	// pulls out each element by space delimitation
		fileline = std::string(linebuffer);
		filelines.push_back(fileline);
	}
	*/
	
	/* instead use string literal file to load into char[], then split this by newline tokens*/
	std::vector<std::string> filelines;
	const char* rawpositions = 
	#include "MRD_positions_raw.dat"
	;
	// use strtok to split char array into lines by searching for \n's. has problems?
//	char *nextline = strtok(rawpositions, "\n");
//	while(nextline) {                         // while token was found
//		std::string nextlinestring(nextline);
//		filelines.push_back(nextlinestring);
//		nextline = strtok(NULL, "\n");
//	}
	// maybe convert the whole char array into a string and use std::getline method
	std::string fileasstring(rawpositions); // ,length); // length optional, but needed if there may be zero's in your data
	std::istringstream fileasistream(fileasstring);
	std::string nextline;
	while(std::getline(fileasistream,nextline)){
		filelines.push_back(nextline);
	}
	
	// lines are of the format:
	// PMT 277 : Orientation V : Layer 9 : Origin (1216.5,-652.5,503.25) : Extent (1116.5→1316.5, -1388.5→83.5, 4450.7→4456.7)
	std::string theexpressionstring = "PMT ([0-9]+) : Orientation (.) : Layer ([0-9]+) : Origin \\(([0-9\\.\\+\\-]+),([0-9\\.\\+\\-]+),([0-9\\.\\+\\-]+)\\) : Extent \\(([0-9\\.\\+\\-]+)→([0-9\\.\\+\\-]+), ([0-9\\.\\+\\-]+)→([0-9\\.\\+\\-]+), ([0-9\\.\\+\\-]+)→([0-9\\.\\+\\-]+)\\)";
//	std::cout<<"pattern to match is "<<theexpressionstring<<endl;
	
	// use regex to extract the information
	std::match_results<std::string::const_iterator> submatches;
	std::regex theexpression (theexpressionstring);
	int numlayers=0;
	for(int linenum=0; linenum<filelines.size(); linenum++){
		std::string stringtomatch = filelines.at(linenum);
//		std::cout<<"next string is "<<stringtomatch<<endl;
		if(stringtomatch=="") break;
		std::regex_match (stringtomatch, submatches, theexpression);
		if(submatches.size()==0) { 
			std::cerr<<stringtomatch<<" was not matched"<<std::endl; return 0;
		} else {
//			std::cout << "whole match was "<<(std::string)submatches[0]<<endl;
//			for(int i=1; i<submatches.size(); i++){
//				std::string tval = (std::string)submatches[i];
//				std::cout <<"submatch " <<i<<" is "<<tval<<endl;
//			}
			int pmt_id=std::stoi(submatches[1]);
			//pmt_ids.push_back(pmt_id);	// 1:1 mapping, no need
			//std::cout<<"setting stats for pmt "<<pmt_id<<endl;
			std::string orientationstring = submatches[2];
//			std::cout<<"orientation of pmt "<<linenum<<" is "<<orientationstring.c_str()<<endl;
//			if(strcmp(orientationstring.c_str(),"H")==0) std::cout<<"it's horizontal"<<endl; else std::cout<<"it's vertical"<<endl;
			(strcmp(orientationstring.c_str(),"H")==0) ? temp_paddle_orientations.at(pmt_id)=0 : temp_paddle_orientations.at(pmt_id)=1;
			temp_paddle_layers.at(pmt_id)=(std::stoi(submatches[3]));
			temp_paddle_originx.at(pmt_id)=(std::stod(submatches[4]));
			temp_paddle_originy.at(pmt_id)=(std::stod(submatches[5]));
			temp_paddle_originz.at(pmt_id)=(std::stod(submatches[6]));
			temp_paddle_extentsx.at(pmt_id)=(std::pair<double, double>(std::stod(submatches[7]),std::stod(submatches[8])));
			temp_paddle_extentsy.at(pmt_id)=(std::pair<double, double>(std::stod(submatches[9]),std::stod(submatches[10])));
			temp_paddle_extentsz.at(pmt_id)=(std::pair<double, double>(std::stod(submatches[11]),std::stod(submatches[12])));
		}
	}
	// success should return for each match:
	//submatch 1 is 0
	//submatch 2 is H
	//submatch 3 is 0
	//submatch 4 is 737.5
	//submatch 5 is -1219.5
	//submatch 6 is -586.65
	//submatch 7 is 1.5
	//submatch 8 is 1473.5
	//submatch 9 is -1319.5
	//submatch 10 is -1119.5
	//submatch 11 is 2665.35
	//submatch 12 is 2671.35
	
//	std::ofstream mrdpositions;
//	mrdpositions.open("mrdpositions.txt", std::ios::out);
//	mrdpositions<<"pmtids is "<<pmt_ids<<endl;
//	mrdpositions<<"orientations is "<<orientations<<endl;
//	mrdpositions<<"layers is "<<layers<<endl;
//	mrdpositions<<"originxs is "<<originx<<endl;
//	mrdpositions<<"originys is "<<originy<<endl;
//	mrdpositions<<"originzs is "<<originz<<endl;
//	mrdpositions<<"extentsx is "<<extentsx<<endl;
//	mrdpositions<<"extentsy is "<<extentsy<<endl;
//	mrdpositions<<"extentsz is "<<extentsz<<endl;
//	mrdpositions.close();
	
//	for(int i=0; i<temp_paddle_orientations.size(); i++){
//		std::cout<<"orientations is "<<temp_paddle_orientations.at(i)<<endl;
//		std::cout<<"layers is "<<temp_paddle_layers.at(i)<<endl;
//		std::cout<<"originxs is "<<temp_paddle_originx.at(i)<<endl;
//		std::cout<<"originys is "<<temp_paddle_originy.at(i)<<endl;
//		std::cout<<"originzs is "<<temp_paddle_originz.at(i)<<endl;
//		std::cout<<"extentsx is "<<temp_paddle_extentsx.at(i).first<<", "<<temp_paddle_extentsx.at(i).second<<endl;
//		std::cout<<"extentsy is "<<temp_paddle_extentsy.at(i).first<<", "<<temp_paddle_extentsy.at(i).second<<endl;
//		std::cout<<"extentsz is "<<temp_paddle_extentsz.at(i).first<<", "<<temp_paddle_extentsz.at(i).second<<endl;
//	}
	
	return 1;
}

// call the function to set the temporary variables.
int nothing = StripMrdPositions();

// initialise the const namespace variables with the temporary ones.
const std::vector<int> &MRDSpecs::paddle_orientations = temp_paddle_orientations;
const std::vector<int> &MRDSpecs::paddle_layers = temp_paddle_layers;
const std::vector<double> &MRDSpecs::paddle_originx = temp_paddle_originx;
const std::vector<double> &MRDSpecs::paddle_originy = temp_paddle_originy;
const std::vector<double> &MRDSpecs::paddle_originz = temp_paddle_originz;
const std::vector<std::pair<double,double> > &MRDSpecs::paddle_extentsx = temp_paddle_extentsx;
const std::vector<std::pair<double,double> > &MRDSpecs::paddle_extentsy = temp_paddle_extentsy;
const std::vector<std::pair<double,double> > &MRDSpecs::paddle_extentsz = temp_paddle_extentsz;


