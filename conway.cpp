/*
 * =====================================================================================
 *
 *       Filename:  conway.cpp
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
#include <memory.h>

#ifdef CPLUSPLUS
#include <iostream>
#include <vector>
#include <bitset>
#include <unistd.h>
#include <random>
#endif

using namespace std;

unsigned xMasks[32];
unsigned yMasks[32];
unsigned newBoard[32];

inline void init() {
	xMasks[0] = 0x40000001;
	xMasks[1] = 0xA0000000;

	for( char i = 2; i < 31; i++ ) {
		xMasks[i] = xMasks[i-1] >> 1;
	}

	xMasks[31] = 0x80000002;

	yMasks[0] = 0xC0000001;
	yMasks[1] = 0xE0000000;

	for( char i = 2; i < 31; i++ ) {
		yMasks[i] = yMasks[i-1] >> 1;
	}

	yMasks[31] = 0x80000003;
}

#ifdef CPLUSPLUS
void clearScreen() {
	cout << string( 100, '\n' );
}

void printBoard( unsigned* board ) {
	for( char i = 0; i < 32; i++ ) {
		cout << bitset<32>(board[i]) << endl;
	}
}
#endif

char countLiveNeighbors( unsigned* board, int x, int y ) {
	char result = __builtin_popcount( board[y] & xMasks[x] );
	result += __builtin_popcount( board[((y-1%32)<0)?31:y-1] & yMasks[x] );
	result += __builtin_popcount( board[(y+1)%32] & yMasks[x] );
	return result;
}

inline void updateBoard( unsigned* board ) {
	char live;

	for( char y = 0; y < 32; y++ ) {
		newBoard[y] = 0;
		for( char x = 0; x < 32; x++ ) {
			live = countLiveNeighbors( board, x, y );
			if( live == 3 ) {
				newBoard[y] |= 1 << ( 31 - x );
			}
			if( live == 2 && ( board[y] >> ( 31 - x ) ) & 1 ) {
				newBoard[y] |= 1 << ( 31 - x );
			}
		}
	}

	memcpy( board, newBoard, 32*sizeof( unsigned ) );
}

#ifdef CPLUSPLUS
unsigned* makeRandomBoard() {
	unsigned *board = static_cast<unsigned*>( calloc( 32, sizeof( *board ) ) );
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 0xFFFF);

	for( char i = 0; i < 32; i++ ) {
		board[i] = dis(gen);
	}

	return board;
}
#endif

int main() {
	init();
	//unsigned *board = makeRandomBoard();
	unsigned *board = static_cast<unsigned*>( calloc( 32, sizeof( *board ) ) );
	board[0] = 1 << 30;
	board[1] = 1 << 29;
	board[2] = 7 << 29;

	for( int i = 0; i < 1000; i++ ) {
		//clearScreen();
		//printBoard( board );
		updateBoard( board );
		//usleep( 1000 * 25 );
	}

	//printBoard( board );
	free( board );
	return 0;
}

