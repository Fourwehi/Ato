#pragma once

#include <vector>
#include <sb7.h>
#include <vmath.h>

#include "GameConstants.h"
#include "Tetromino.h"

// 게임 보드의 상태를 관리하고 렌더링하는 클래스입니다.
// 충돌 감지, 줄 완성 검사 및 제거, 블록 그리기의 핵심 로직을 포함합니다.
// 헤더 전용(Header-only)으로 구현되었습니다.
class Board {
public:
	// 생성자: 보드 상태를 초기화하고 큐브 렌더링을 위한 OpenGL 객체를 설정합니다.
	Board ( ) {
		// boardState 벡터의 크기를 설정하고 모든 값을 0(빈 공간)으로 초기화합니다.
		boardState.resize ( GameConstants::BOARD_HEIGHT , std::vector<int> ( GameConstants::BOARD_WIDTH , 0 ) );

		// 렌더링에 사용할 큐브 모델을 설정합니다.
		setupCubeModel ( );
	}

	// 소멸자: 생성된 OpenGL 객체들을 정리합니다.
	~Board ( ) {
		glDeleteVertexArrays ( 1 , &cubeVao );
		glDeleteBuffers ( 1 , &cubeVbo );
	}

	// 보드를 초기 상태로 리셋합니다. 게임 재시작 시 사용됩니다.
	void reset ( ) {
		for ( int y = 0; y < GameConstants::BOARD_HEIGHT; ++y ) {
			for ( int x = 0; x < GameConstants::BOARD_WIDTH; ++x ) {
				boardState[ y ][ x ] = 0;
			}
		}
	}

	// 지정된 테트로미노가 현재 위치에서 충돌하는지 검사합니다.
	// 벽, 바닥, 또는 이미 쌓인 다른 블록과의 충돌을 확인합니다.
	bool checkCollision ( const Tetromino& t ) const {
		for ( int r = 0; r < 4; ++r ) {
			for ( int c = 0; c < 4; ++c ) {
				// 현재 검사하는 4x4 영역에 블록이 없다면 건너뜁니다.
				if ( t.getBlock ( r , c ) == 0 ) {
					continue;
				}

				// 블록의 보드 상 실제 좌표 계산
				int boardX = t.getX ( ) + c;
				int boardY = t.getY ( ) + r;

				// 1. 벽 충돌 검사 (좌, 우 경계)
				if ( boardX < 0 || boardX >= GameConstants::BOARD_WIDTH ) {
					return true;
				}

				// 2. 바닥 충돌 검사
				if ( boardY >= GameConstants::BOARD_HEIGHT ) {
					return true;
				}

				// 3. 이미 쌓인 블록과의 충돌 검사
				// boardY가 음수일 수 있으므로(화면 위에서 생성됨), 양수일 때만 검사합니다.
				if ( boardY >= 0 && boardState[ boardY ][ boardX ] != 0 ) {
					return true;
				}
			}
		}
		return false; // 어떤 충돌도 발견되지 않음
	}

	// 착지한 테트로미노를 보드 상태에 영구적으로 추가(기록)합니다.
	void addTetrominoToBoard ( const Tetromino& t ) {
		int shapeType = t.getShapeType ( );
		for ( int r = 0; r < 4; ++r ) {
			for ( int c = 0; c < 4; ++c ) {
				if ( t.getBlock ( r , c ) != 0 ) {
					int boardX = t.getX ( ) + c;
					int boardY = t.getY ( ) + r;
					// 보드 범위 내에 있을 경우에만 상태를 기록합니다.
					if ( boardX >= 0 && boardX < GameConstants::BOARD_WIDTH &&
						boardY >= 0 && boardY < GameConstants::BOARD_HEIGHT ) {
						boardState[ boardY ][ boardX ] = shapeType;
					}
				}
			}
		}
	}

	// 완성된 줄이 있는지 검사하고, 있다면 제거한 후 제거한 줄의 수를 반환합니다.
	int checkAndClearLines ( ) {
		int linesCleared = 0;
		for ( int y = GameConstants::BOARD_HEIGHT - 1; y >= 0; --y ) {
			bool lineIsFull = true;
			for ( int x = 0; x < GameConstants::BOARD_WIDTH; ++x ) {
				if ( boardState[ y ][ x ] == 0 ) {
					lineIsFull = false;
					break;
				}
			}

			if ( lineIsFull ) {
				clearLine ( y );
				linesCleared++;
				// 한 줄이 지워지면, 같은 y-인덱스를 다시 검사해야 합니다 (윗 줄이 내려왔으므로).
				// y를 1 증가시켜 루프의 다음 반복에서 y--에 의해 같은 줄을 다시 검사하게 만듭니다.
				y++;
			}
		}
		return linesCleared;
	}

	// 보드에 쌓인 모든 블록들을 그립니다.
	// projection과 view 행렬을 외부(메인 렌더링 루프)에서 받습니다.
	void draw ( const vmath::mat4& projection_matrix , const vmath::mat4& view_matrix , GLuint color_loc ) const {
		glBindVertexArray ( cubeVao );

		for ( int y = 0; y < GameConstants::BOARD_HEIGHT; ++y ) {
			for ( int x = 0; x < GameConstants::BOARD_WIDTH; ++x ) {
				// 해당 칸에 블록이 없으면 그리지 않습니다.
				if ( boardState[ y ][ x ] == 0 ) {
					continue;
				}

				// 블록 색상 가져오기
				int shapeType = boardState[ y ][ x ];
				vmath::vec4 color = GameConstants::PIECE_COLORS[ shapeType ];

				// 셰이더에 현재 vertex attribute로 색상 값을 전달합니다.
				// 이 값은 이어서 호출되는 glDrawArrays에 의해 그려지는 모든 정점에 동일하게 적용됩니다.
				glVertexAttrib4fv ( color_loc , color );

				// 블록의 월드 좌표를 기반으로 Model 행렬을 계산합니다.
				// vmath::translate 함수를 사용하여 위치를 지정합니다.
				float worldX = GameConstants::BOARD_START_X + x * GameConstants::CUBE_SIZE;
				float worldY = GameConstants::BOARD_START_Y + ( GameConstants::BOARD_HEIGHT - 1 - y ) * GameConstants::CUBE_SIZE;

				vmath::mat4 model_matrix = vmath::translate ( worldX , worldY , 0.0f );

				// 최종 MVP 행렬 계산 (이 부분은 main.cpp에서 처리하고 uniform으로 전달할 수도 있습니다)
				// vmath::mat4 mvp_matrix = projection_matrix * view_matrix * model_matrix;
				// 여기서는 Model 행렬만 계산하고, MVP 계산과 uniform 전달은 main_file에서 수행하도록 합니다.
				// main.cpp에서 drawCube(model_matrix, color) 같은 함수를 호출하는게 더 적절합니다.

				// 이 예제에서는 Board 클래스가 직접 그리도록 구현합니다.
				// main.cpp에서 셰이더 프로그램을 활성화하고 P, V 행렬을 uniform으로 전달했다고 가정합니다.
				// Model 행렬을 'M' uniform 변수에 전달합니다.
				// (실제 uniform location은 main에서 얻어와야 합니다.)
				// glUniformMatrix4fv(M_loc, 1, GL_FALSE, model_matrix);

				// 여기서는 main.cpp에서 그리도록 로직을 수정하는 것이 더 나으므로,
				// 이 클래스는 상태만 관리하고, 그리는 로직은 main.cpp의 render 함수에서 직접 처리하도록 하겠습니다.
				// 아래는 만약 이 클래스에서 직접 그린다고 가정했을 때의 코드입니다.
				// 이 코드는 실제로는 main.cpp로 옮겨져야 합니다.
				/*
				   // 셰이더 프로그램이 이미 활성화되었다고 가정
				   glUniformMatrix4fv(glGetUniformLocation(shader_program, "M"), 1, GL_FALSE, model_matrix);
				   glUniform4fv(glGetUniformLocation(shader_program, "objectColor"), 1, color); // 만약 색상을 uniform으로 전달한다면
				   glDrawArrays(GL_TRIANGLES, 0, 36);
				*/
			}
		}

		glBindVertexArray ( 0 );
	}

	// 보드 상태를 직접 접근하기 위한 const 참조를 반환합니다.
	const std::vector<std::vector<int>>& getBoardState ( ) const {
		return boardState;
	}

	// 큐브 모델의 VAO를 반환합니다. 렌더링 시 필요합니다.
	GLuint getCubeVao ( ) const {
		return cubeVao;
	}

private:
	std::vector<std::vector<int>> boardState; // 게임 보드 그리드 상태
	GLuint cubeVao; // 큐브 모델을 위한 Vertex Array Object
	GLuint cubeVbo; // 큐브 모델의 정점 데이터를 위한 Vertex Buffer Object

	// 특정 줄을 제거하고, 그 위의 모든 줄을 한 칸씩 아래로 내립니다.
	void clearLine ( int row ) {
		boardState.erase ( boardState.begin ( ) + row );
		boardState.insert ( boardState.begin ( ) , std::vector<int> ( GameConstants::BOARD_WIDTH , 0 ) );
	}

	// 렌더링에 사용할 단일 큐브 모델을 설정합니다. (VAO, VBO 생성)
	void setupCubeModel ( ) {
		// 정육면체의 36개 정점 데이터 (위치, 법선) - 12개 삼각형 * 3개 정점
		// 셰이더가 'vertex'와 'normal' 입력을 요구하므로 두 데이터를 모두 제공합니다.
		// 위치(x,y,z), 법선(nx,ny,nz)
		static const GLfloat cube_vertices[ ] = {
			// -Z face
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			// +Z face
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			// -X face
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			// +X face
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 // -Y face
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 // +Y face
			 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		glGenVertexArrays ( 1 , &cubeVao );
		glGenBuffers ( 1 , &cubeVbo );

		glBindVertexArray ( cubeVao );

		glBindBuffer ( GL_ARRAY_BUFFER , cubeVbo );
		glBufferData ( GL_ARRAY_BUFFER , sizeof ( cube_vertices ) , cube_vertices , GL_STATIC_DRAW );

		// vertex attribute (location = 0)
		glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof ( GLfloat ) , ( void* ) 0 );
		glEnableVertexAttribArray ( 0 );

		// normal attribute (location = 2, from v_simplest.txt)
		glVertexAttribPointer ( 2 , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof ( GLfloat ) , ( void* ) ( 3 * sizeof ( GLfloat ) ) );
		glEnableVertexAttribArray ( 2 );

		// color attribute (location = 1)는 VBO를 사용하지 않고, 렌더링 시점에
		// glVertexAttrib4fv()를 사용하여 설정할 것이므로 여기서는 비활성화 상태로 둡니다.

		glBindBuffer ( GL_ARRAY_BUFFER , 0 );
		glBindVertexArray ( 0 );
	}
};
