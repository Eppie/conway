/*
 * =====================================================================================
 *
 *       Filename:  conwayFIRST.cpp
 *
 *    Description:  Conway's Game of Life
 *
 *        Version:  1.0
 *        Created:  07/14/2015 03:16:13 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Andrew Epstein
 *   Organization:  SevOne
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <bitset>
#include <unistd.h>

using namespace std;

vector<unsigned> xMasks(32);
vector<unsigned> yMasks(32);

void init() {
	xMasks[0] = 0x40000001;
	xMasks[1] = 0xA0000000;

	for( auto i = 2; i < 31; i++ ) {
		xMasks[i] = xMasks[i-1] >> 1;
	}

	xMasks[31] = 0x80000002;

	yMasks[0] = 0xC0000001;
	yMasks[1] = 0xE0000000;

	for( auto i = 2; i < 31; i++ ) {
		yMasks[i] = yMasks[i-1] >> 1;
	}

	yMasks[31] = 0x80000003;
}

void clearScreen() {
	cout << string( 100, '\n' );
}

void printBoard( vector<unsigned> board ) {
	for( int row : board ) {
		cout << bitset<32>(row) << endl;
	}
}

int countLiveNeighbors( vector<unsigned> board, int x, int y ) {
	int result = __builtin_popcount( board[y] & xMasks[x] );

	if( y == 0 ) {
		result += __builtin_popcount( board[31] & yMasks[x] );
	} else {
		result += __builtin_popcount( board[y-1] & yMasks[x] );
	}

	if( y == 31 ) {
		result += __builtin_popcount( board[0] & yMasks[x] );
	} else {
		result += __builtin_popcount( board[y+1] & yMasks[x] );
	}

	return result;
}

vector<unsigned> updateBoard( vector<unsigned> board ) {
	vector<unsigned> newBoard(32);
	int live;
	for( int x = 0; x < 32; x++ ) {
		for( int y = 0; y < 32; y++ ) {
			live = countLiveNeighbors( board, x, y );
			if( live == 3 ) {
				newBoard[y] |= 1 << ( 31 - x );
			}
			if( live == 2 && ( board[y] >> ( 31 - x ) ) & 1 ) {
				newBoard[y] |= 1 << ( 31 - x );
			}
		}
	}	
	return newBoard;
}

int main() {
	init();
	vector<unsigned> board(32);
	board[0] = 1 << 30;
	board[1] = 1 << 29;
	board[2] = 7 << 29;

	for( int i = 0; i < 1000; i++ ) {
		board = updateBoard( board );
	}
	printBoard( board );
	return 0;
}

