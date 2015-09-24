/*
 * =====================================================================================
 *
 *       Filename:  evogcc.cpp
 *
 *    Description:  Evolutionary Optimization of GCC flags
 *
 *        Version:  1.0
 *        Created:  07/18/2015 02:11:14 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Andrew Epstein
 *   Organization:  SevOne
 *
 * =====================================================================================
 */

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <time.h>
#include <map>

using namespace std;

string exec( string cmd ) {
	FILE* pipe = popen( cmd.c_str(), "r" );
	int bufSize = 128;

	if( !pipe ) {
		return "ERROR";
	}

	char buffer[bufSize];
	string result = "";

	while( !feof( pipe ) ) {
		if( fgets( buffer, bufSize, pipe ) != NULL ) {
			result += buffer;
		}
	}

	pclose( pipe );
	return result;
}

vector<string> readFile( string fileName ) {
	ifstream is( fileName.c_str() );
	istream_iterator<string> start( is ), end;
	vector<string> results( start, end );
	cout << "Read " << results.size() << " flags" << endl;
	return results;
}

class Organism {
	vector<string> genes;
	int geneSize;
	vector<int> genesOn;
	public:
	Organism( vector<string> g );
	void print();
	Organism operator+( const Organism &other );
	void setGenesOn( vector<int> gOn ) { genesOn = gOn; }
	string getFlags();
};

Organism::Organism( vector<string> g ) {
	genes = g;
	geneSize = genes.size();

	for( int i = 0; i<geneSize; i++ ) {
		int val = rand() % 2;
		genesOn.push_back( val );
	}
}

void Organism::print() {
	cout << "Organism: \n";

	for( int val : genesOn  ) {
		cout << val;
	}

	cout << endl;
}

Organism Organism::operator+( const Organism &other ) {
	vector<int> newGenesOn;
	srand( time( NULL ) );
	for( int i = 0; i < geneSize; i++ ) {
		if( genesOn[i] == other.genesOn[i] ) {
			newGenesOn.push_back( genesOn[i] );
		} else {
			newGenesOn.push_back( rand() % 2 );
		}
	}
	Organism result( genes );
	result.setGenesOn( newGenesOn );
	return result;
}

string Organism::getFlags() {
	string flags = "";

	for( int i = 0; i < geneSize; i++ ) {
		if( genesOn[i] == 1 ) {
			flags += " " + genes[i];
		}
	}

	return flags;
}

int evaluate( Organism o ) {
	string flags = o.getFlags();
	exec( "gcc -w conway.cpp -o conway -std=c++11 -march=native" + flags );
	exec( "./conway" );
	exec( "gcc -w conway.cpp -o conway -std=c++11 -march=native" + flags ); 
	return stoi ( exec( "valgrind --log-fd=1 --tool=callgrind ./conway | awk '/Collected/ {print $4}'" ) );
}

int main() {
	srand( time( NULL ) );
	vector<string> flags = readFile( "flags" );
	int numOrgs = 10;
	map<string,int> evaluations;

	for( int i = 0; i < numOrgs; i++ ) {
		Organism o( flags );
		int fitness = evaluate( o );
		string genes = o.getFlags();
		evaluations[genes] = fitness;
		//cout << genes << endl;
		cout << fitness << endl;
	}

	exec( "rm -f gmon.out callgrind.out* *.gcda" );
	return 0;
}
