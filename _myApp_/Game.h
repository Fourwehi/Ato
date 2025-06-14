#pragma once

#include "Tetromino.h"
#include "Board.h"
#include "Score.h"

#include <vector>
#include <map>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>

const int GRID_WIDTH = 5;
const int GRID_DEPTH = 5;
const int GRID_HEIGHT = 12;
const int BLOCK_SIZE = 2;

const std::map<TetrominoType , int> tetrominoPoints = {
	{ TetrominoType::HERO, 5 }, { TetrominoType::L1, 3 }, { TetrominoType::L2, 3 },
	{ TetrominoType::Z1, 4 }, { TetrominoType::Z2, 4 }, { TetrominoType::TEEWEE, 6 },
	{ TetrominoType::SMASHBOY, 10 }
};

class Game {
public:
	Game ( GLuint shader_program , vmath::mat4* V , vmath::mat4* P , std::map<TetrominoType , vmath::vec4>* colors ) {
		this->shader_program = shader_program;
		this->V = V;
		this->P = P;
		this->tetrominoColors = colors;
		this->grid.resize ( GRID_HEIGHT , std::vector<std::vector<bool>> ( GRID_WIDTH , std::vector<bool> ( GRID_DEPTH , false ) ) );
		this->texGrid.resize ( GRID_HEIGHT , std::vector<std::vector<TetrominoType>> ( GRID_WIDTH , std::vector<TetrominoType> ( GRID_DEPTH , TetrominoType::None ) ) );
		this->currentBlockCoords.resize ( 4 , vmath::vec3 ( 0.0f ) );
		this->board = new Board ( shader_program , V , P , GRID_WIDTH , GRID_HEIGHT );
		this->currentPiece = new Tetromino ( shader_program , V , P );
		this->score = new Score ( shader_program , V , P );
		this->moving = false;
		this->justSpawned = false;
		this->dropInterval = 1.0f;
		this->currentDropInterval = 1.0f;
		refillBag ( );
		this->nextType = getRandomTetrominoType ( );
	}
	~Game ( ) {
		if ( board ) delete board;
		if ( currentPiece ) delete currentPiece;
		if ( score ) delete score;
	}
	void init ( ) {}
	void update ( bool timeReady ) {
		if ( timeReady ) {
			if ( !moving ) {
				this->currentType = nextType;
				nextType = getRandomTetrominoType ( );
				moving = true;
				justSpawned = true;
				setStartingCoords ( );
				if ( !canPlace ( ) ) {
					printf ( "Game Over! Cannot place the tetromino.\n" );
					exit ( 0 );
				}
			}
			if ( justSpawned ) {
				justSpawned = false;
			}
			else if ( canMove ( ) ) {
				decrementCoordY ( );
			}
			else {
				addTetrominoToGrid ( );
				clearFullLayers ( );
				this->moving = false;
				this->currentBlockCoords.assign ( 4 , vmath::vec3 ( 0.0f ) );
			}
		}
	}
	void render ( vmath::mat4 M_in , float angle_x , float nextTetrominoAngle ) {
		this->M = M_in;
		spawnBoard ( M , angle_x );
		spawnStaticBlocks ( M );
		spawnCurrentTetromino ( M );
		spawnNextTetromino ( nextTetrominoAngle );
		this->score->displayScore ( );
	}
	void moveLeft ( float angle_x ) { moveByOffset ( 1 , 0 , angle_x ); }
	void moveRight ( float angle_x ) { moveByOffset ( -1 , 0 , angle_x ); }
	void moveForward ( float angle_x ) { moveByOffset ( 0 , 1 , angle_x ); }
	void moveBackward ( float angle_x ) { moveByOffset ( 0 , -1 , angle_x ); }
	void tryRotateY ( ) {
		std::vector<vmath::vec3> rotatedCoords = rotateBlockCoordsY ( currentBlockCoords );
		if ( canRotate ( rotatedCoords ) ) { currentBlockCoords = rotatedCoords; }
	}
	void tryRotateX ( ) {
		std::vector<vmath::vec3> rotatedCoords = rotateBlockCoordsX ( currentBlockCoords );
		if ( canRotate ( rotatedCoords ) ) { currentBlockCoords = rotatedCoords; }
	}
	void setFastDrop ( bool fast ) { currentDropInterval = fast ? std::min ( 0.05f , dropInterval ) : dropInterval; }
	float getCurrentDropInterval ( ) const { return currentDropInterval; }
private:
	GLuint shader_program;
	std::map<TetrominoType , vmath::vec4>* tetrominoColors;
	vmath::mat4* V; vmath::mat4* P; vmath::mat4 M;
	Board* board; Tetromino* currentPiece; Score* score;
	std::vector<std::vector<std::vector<bool>>> grid;
	std::vector<std::vector<std::vector<TetrominoType>>> texGrid;
	std::vector<vmath::vec3> currentBlockCoords;
	TetrominoType currentType , nextType;
	bool moving , justSpawned;
	float dropInterval , currentDropInterval;
	std::vector<TetrominoType> bag; int currentIndex;
	bool isWithinBounds ( int x , int y , int z ) { return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT && z >= 0 && z < GRID_DEPTH; }
	vmath::vec3 getWorldCoords ( vmath::vec3 coords ) {
		coords[ 0 ] = ( coords[ 0 ] - GRID_WIDTH / 2.0f ) * BLOCK_SIZE + 1.0f;
		coords[ 1 ] = coords[ 1 ] * BLOCK_SIZE + 1.0f;
		coords[ 2 ] = ( coords[ 2 ] - GRID_DEPTH / 2.0f ) * BLOCK_SIZE + 1.0f;
		return coords;
	}
	void spawnCurrentTetromino ( vmath::mat4 M_in ) {
		if ( !moving ) return;
		vmath::vec4 color = ( *tetrominoColors )[ currentType ];
		for ( int i = 0; i < 4; i++ ) {
			vmath::vec3 blockPosition = getWorldCoords ( currentBlockCoords[ i ] );
			this->currentPiece->drawCube ( M_in , color , blockPosition );
		}
	}
	void spawnBoard ( vmath::mat4 M_in , float angle_x ) { board->drawBoard ( M_in , angle_x ); }
	void spawnStaticBlocks ( vmath::mat4 M_in ) {
		for ( int i = 0; i < GRID_HEIGHT; i++ ) {
			for ( int j = 0; j < GRID_WIDTH; j++ ) {
				for ( int k = 0; k < GRID_DEPTH; k++ ) {
					if ( this->grid[ i ][ j ][ k ] ) {
						vmath::vec3 block_pos = getWorldCoords ( vmath::vec3 ( ( float ) j , ( float ) i , ( float ) k ) );
						this->currentPiece->drawCube ( M_in , ( *tetrominoColors )[ texGrid[ i ][ j ][ k ] ] , block_pos );
					}
				}
			}
		}
	}
	void spawnNextTetromino ( float nextTetrominoAngle ) {
		vmath::mat4 nextM = vmath::translate ( -15.0f , 18.0f , 0.0f ) * vmath::rotate ( nextTetrominoAngle , 0.0f , 1.0f , 0.0f ) * vmath::scale ( 0.8f );
		const auto& shape = tetrominoShapes.at ( this->nextType );
		vmath::vec4 color = ( *tetrominoColors )[ nextType ];
		for ( const auto& blockPos : shape ) {
			this->currentPiece->drawCube ( nextM , color , blockPos );
		}
	}
	bool canPlace ( ) {
		for ( const auto& coord : currentBlockCoords ) {
			int x = static_cast< int >( round ( coord[ 0 ] ) ); int y = static_cast< int >( round ( coord[ 1 ] ) ); int z = static_cast< int >( round ( coord[ 2 ] ) );
			if ( !isWithinBounds ( x , y , z ) || grid[ y ][ x ][ z ] ) return false;
		}
		return true;
	}
	void setStartingCoords ( ) {
		const auto& shape = tetrominoShapes.at ( this->currentType );
		for ( int i = 0; i < 4; i++ ) {
			currentBlockCoords[ i ][ 0 ] = ( GRID_WIDTH / 2.0f ) + ( shape[ i ][ 0 ] / 2.0f );
			currentBlockCoords[ i ][ 1 ] = ( GRID_HEIGHT - 1.0f );
			currentBlockCoords[ i ][ 2 ] = ( GRID_DEPTH / 2.0f ) + ( shape[ i ][ 2 ] / 2.0f );
		}
	}
	bool canMove ( ) {
		for ( const auto& coord : currentBlockCoords ) {
			int x = static_cast< int > ( round ( coord[ 0 ] ) ); int y = static_cast< int > ( round ( coord[ 1 ] ) ); int z = static_cast< int > ( round ( coord[ 2 ] ) );
			if ( y <= 0 || ( isWithinBounds ( x , y - 1 , z ) && grid[ y - 1 ][ x ][ z ] ) ) return false;
		}
		return true;
	}
	void decrementCoordY ( ) { for ( int i = 0; i < 4; i++ ) { this->currentBlockCoords[ i ][ 1 ] -= 1.0f; } }
	void addTetrominoToGrid ( ) {
		for ( const auto& coord : currentBlockCoords ) {
			int x = static_cast< int > ( round ( coord[ 0 ] ) ); int y = static_cast< int > ( round ( coord[ 1 ] ) ); int z = static_cast< int > ( round ( coord[ 2 ] ) );
			if ( isWithinBounds ( x , y , z ) ) {
				this->grid[ y ][ x ][ z ] = true;
				this->texGrid[ y ][ x ][ z ] = this->currentType;
			}
		}
	}
	bool canMoveByOffset ( int dx , int dz ) {
		for ( const auto& coord : currentBlockCoords ) {
			int y = static_cast< int >( round ( coord[ 1 ] ) ); int newX = static_cast< int >( round ( coord[ 0 ] ) ) + dx; int newZ = static_cast< int >( round ( coord[ 2 ] ) ) + dz;
			if ( !isWithinBounds ( newX , y , newZ ) || grid[ y ][ newX ][ newZ ] ) return false;
		}
		return true;
	}
	void moveByOffset ( int dx , int dz , float angle_x ) {
		float cosA = cos ( -angle_x ); float sinA = sin ( -angle_x );
		float rotatedDx = dx * cosA - dz * sinA;
		float rotatedDz = dx * sinA + dz * cosA;
		int finalDx = 0 , finalDz = 0;
		if ( std::abs ( rotatedDx ) > std::abs ( rotatedDz ) ) { finalDx = ( rotatedDx > 0 ) ? 1 : -1; }
		else { finalDz = ( rotatedDz > 0 ) ? 1 : -1; }
		if ( canMoveByOffset ( finalDx , finalDz ) ) {
			for ( int i = 0; i < 4; i++ ) {
				currentBlockCoords[ i ][ 0 ] += finalDx;
				currentBlockCoords[ i ][ 2 ] += finalDz;
			}
		}
	}
	void clearFullLayers ( ) {
		for ( int y = 0; y < GRID_HEIGHT; y++ ) {
			bool fullLayer = true;
			for ( int x = 0; x < GRID_WIDTH && fullLayer; x++ ) { for ( int z = 0; z < GRID_DEPTH && fullLayer; z++ ) { if ( !grid[ y ][ x ][ z ] ) fullLayer = false; } }
			if ( fullLayer ) {
				for ( int x = 0; x < GRID_WIDTH; x++ ) { for ( int z = 0; z < GRID_DEPTH; z++ ) { score->addScore ( tetrominoPoints.at ( texGrid[ y ][ x ][ z ] ) ); } }
				dropInterval *= 0.95f;
				for ( int yy = y; yy < GRID_HEIGHT - 1; yy++ ) { grid[ yy ] = grid[ yy + 1 ]; texGrid[ yy ] = texGrid[ yy + 1 ]; }
				grid[ GRID_HEIGHT - 1 ].assign ( GRID_WIDTH , std::vector<bool> ( GRID_DEPTH , false ) );
				texGrid[ GRID_HEIGHT - 1 ].assign ( GRID_WIDTH , std::vector<TetrominoType> ( GRID_DEPTH , TetrominoType::None ) );
				y--;
			}
		}
	}
	bool canRotate ( const std::vector<vmath::vec3>& rotatedCoords ) {
		for ( const auto& point : rotatedCoords ) {
			int x = static_cast< int > ( round ( point[ 0 ] ) ); int y = static_cast< int > ( round ( point[ 1 ] ) ); int z = static_cast< int > ( round ( point[ 2 ] ) );
			if ( !isWithinBounds ( x , y , z ) || grid[ y ][ x ][ z ] ) return false;
		}
		return true;
	}
	std::vector<vmath::vec3> rotateBlockCoordsY ( const std::vector<vmath::vec3>& coords ) {
		std::vector<vmath::vec3> rotatedCoords; vmath::vec3 center = coords[ 2 ];
		for ( const auto& point : coords ) {
			vmath::vec3 local = point - center;
			rotatedCoords.push_back ( vmath::vec3 ( local[ 2 ] , local[ 1 ] , -local[ 0 ] ) + center );
		}
		return rotatedCoords;
	}
	std::vector<vmath::vec3> rotateBlockCoordsX ( const std::vector<vmath::vec3>& coords ) {
		std::vector<vmath::vec3> rotatedCoords; vmath::vec3 center = coords[ 2 ];
		for ( const auto& point : coords ) {
			vmath::vec3 local = point - center;
			rotatedCoords.push_back ( vmath::vec3 ( local[ 0 ] , -local[ 2 ] , local[ 1 ] ) + center );
		}
		return rotatedCoords;
	}
	void refillBag ( ) {
		bag = { TetrominoType::HERO, TetrominoType::L1, TetrominoType::L2, TetrominoType::Z1, TetrominoType::Z2, TetrominoType::TEEWEE, TetrominoType::SMASHBOY };
		std::random_device rd; std::mt19937 g ( rd ( ) );
		std::shuffle ( bag.begin ( ) , bag.end ( ) , g );
		currentIndex = 0;
	}
	TetrominoType getRandomTetrominoType ( ) {
		if ( currentIndex >= bag.size ( ) ) { refillBag ( ); }
		return bag[ currentIndex++ ];
	}
};