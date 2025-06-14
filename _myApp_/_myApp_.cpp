// sb7.h 헤더 파일을 포함합니다.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include <vector>
#include <string>  // for std::to_string
#include <iostream> // for std::cout

// stb_image.h와 Model.h는 요구사항에 따라 포함하지만,
// 이 특정 프로젝트에서는 직접 사용되지 않을 수 있습니다.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"

// 직접 구현한 헤더 전용 클래스들을 포함합니다.
#include "GameConstants.h"
#include "Score.h"
#include "Tetromino.h"
#include "Board.h"


// sb7::application 클래스를 상속받아 메인 애플리케이션 클래스를 정의합니다.
class tetris_app : public sb7::application
{
private:
	GLuint          shader_program; // 셰이더 프로그램 객체

	// 셰이더 유니폼 변수 위치 저장을 위한 변수들
	struct {
		GLint       P;   // Projection Matrix
		GLint       V;   // View Matrix
		GLint       M;   // Model Matrix
		GLint       mode; // Rendering Mode (0: color, 1: lighting)
	} uniforms;

	// 게임 로직 객체들
	Board* gameBoard;
	Tetromino* currentTetromino;
	Score* gameScore;

	// 게임 상태 변수들
	bool            gameOver;
	double          lastFallTime;   // 마지막으로 블록이 떨어진 시간
	double          fallSpeed;      // 현재 블록 하강 속도
	bool            fastDrop;       // 아래 방향키로 빠른 하강 중인지 여부

public:
	// 렌더링 함수를 오버라이드합니다. 매 프레임 호출됩니다.
	void render ( double currentTime ) override
	{
		// --- 1. 게임 로직 업데이트 ---
		if ( !gameOver )
		{
			double deltaTime = currentTime - lastFallTime;
			if ( deltaTime > fallSpeed )
			{
				lastFallTime = currentTime;

				// 테트로미노를 한 칸 아래로 이동 시도
				currentTetromino->move ( 0 , 1 );
				if ( gameBoard->checkCollision ( *currentTetromino ) )
				{
					// 충돌이 발생하면, 이전 위치로 되돌리고 보드에 고정합니다.
					currentTetromino->move ( 0 , -1 );
					gameBoard->addTetrominoToBoard ( *currentTetromino );

					// 완성된 줄이 있는지 확인하고 점수를 업데이트합니다.
					int clearedLines = gameBoard->checkAndClearLines ( );
					if ( clearedLines > 0 )
					{
						gameScore->addScore ( clearedLines );
						// 윈도우 제목에 점수를 업데이트합니다.
						std::string title = "Tetris 3D - Score: " + std::to_string ( gameScore->getScore ( ) );
						// set_title(title.c_str());  <- [수정됨] 잘못된 함수 호출
						glfwSetWindowTitle ( window , title.c_str ( ) ); // [수정됨] 올바른 함수 호출
					}

					// 새 테트로미노를 스폰합니다.
					currentTetromino->spawnNew ( );

					// 새 테트로미노가 스폰되자마자 충돌하면 게임 오버입니다.
					if ( gameBoard->checkCollision ( *currentTetromino ) )
					{
						gameOver = true;
						// set_title("GAME OVER - Press 'R' to Restart"); <- [수정됨] 잘못된 함수 호출
						glfwSetWindowTitle ( window , "GAME OVER - Press 'R' to Restart" ); // [수정됨] 올바른 함수 호출
					}
				}
			}
		}

		// --- 2. 렌더링 ---
		// 화면을 특정 색상으로 지웁니다.
		static const GLfloat black[ ] = { 0.1f, 0.1f, 0.1f, 1.0f };
		glClearBufferfv ( GL_COLOR , 0 , black );
		// 깊이 버퍼를 활성화하고 지웁니다.
		glEnable ( GL_DEPTH_TEST );
		glClearBufferfv ( GL_DEPTH , 0 , &vmath::vec1 ( 1.0f )[ 0 ] );

		// 셰이더 프로그램을 사용합니다.
		glUseProgram ( shader_program );

		// --- 행렬 설정 (vmath 사용) ---
		// Projection 행렬: 원근 투영을 설정합니다. (시야각 60도)
		vmath::mat4 proj_matrix = vmath::perspective ( 60.0f , ( float ) info.windowWidth / ( float ) info.windowHeight , 0.1f , 1000.0f );

		// View 행렬: 카메라의 위치와 방향을 설정합니다.
		// 보드를 약간 위에서 비스듬히 내려다보는 시점입니다.
		vmath::vec3 eye_pos = vmath::vec3 ( 0.0f , 5.0f , -15.0f );
		vmath::vec3 target_pos = vmath::vec3 ( 0.0f , -2.0f , 0.0f );
		vmath::vec3 up_dir = vmath::vec3 ( 0.0f , 1.0f , 0.0f );
		vmath::mat4 view_matrix = vmath::lookat ( eye_pos , target_pos , up_dir );

		// 계산된 Projection, View 행렬을 셰이더의 uniform 변수로 전달합니다.
		glUniformMatrix4fv ( uniforms.P , 1 , GL_FALSE , proj_matrix );
		glUniformMatrix4fv ( uniforms.V , 1 , GL_FALSE , view_matrix );

		// 렌더링 모드를 0 (단순 컬러 모드)으로 설정합니다.
		glUniform1i ( uniforms.mode , 0 );

		// --- 보드에 쌓인 블록들 그리기 ---
		const auto& boardState = gameBoard->getBoardState ( );
		glBindVertexArray ( gameBoard->getCubeVao ( ) );

		for ( int y = 0; y < GameConstants::BOARD_HEIGHT; ++y )
		{
			for ( int x = 0; x < GameConstants::BOARD_WIDTH; ++x )
			{
				if ( boardState[ y ][ x ] != 0 )
				{
					// 블록의 월드 좌표 계산
					float worldX = GameConstants::BOARD_START_X + x * GameConstants::CUBE_SIZE + 0.5f;
					float worldY = GameConstants::BOARD_START_Y + ( GameConstants::BOARD_HEIGHT - 1 - y ) * GameConstants::CUBE_SIZE + 0.5f;

					// Model 행렬 생성 (vmath::translate 사용)
					vmath::mat4 model_matrix = vmath::translate ( worldX , worldY , 0.0f );

					// Model 행렬과 색상을 셰이더에 전달하고 큐브를 그립니다.
					glUniformMatrix4fv ( uniforms.M , 1 , GL_FALSE , model_matrix );
					vmath::vec4 color = GameConstants::PIECE_COLORS[ boardState[ y ][ x ] ];
					glVertexAttrib4fv ( 1 , color ); // location=1에 색상 전달
					glDrawArrays ( GL_TRIANGLES , 0 , 36 );
				}
			}
		}

		// --- 현재 떨어지는 테트로미노 그리기 ---
		if ( !gameOver )
		{
			int shapeType = currentTetromino->getShapeType ( );
			vmath::vec4 color = GameConstants::PIECE_COLORS[ shapeType ];
			glVertexAttrib4fv ( 1 , color ); // location=1에 색상 전달

			for ( int r = 0; r < 4; ++r )
			{
				for ( int c = 0; c < 4; ++c )
				{
					if ( currentTetromino->getBlock ( r , c ) != 0 )
					{
						int gridX = currentTetromino->getX ( ) + c;
						int gridY = currentTetromino->getY ( ) + r;

						float worldX = GameConstants::BOARD_START_X + gridX * GameConstants::CUBE_SIZE + 0.5f;
						float worldY = GameConstants::BOARD_START_Y + ( GameConstants::BOARD_HEIGHT - 1 - gridY ) * GameConstants::CUBE_SIZE + 0.5f;

						vmath::mat4 model_matrix = vmath::translate ( worldX , worldY , 0.0f );
						glUniformMatrix4fv ( uniforms.M , 1 , GL_FALSE , model_matrix );
						glDrawArrays ( GL_TRIANGLES , 0 , 36 );
					}
				}
			}
		}

		glBindVertexArray ( 0 );
		glUseProgram ( 0 );
	}

	// 애플리케이션 시작 시 한 번 호출됩니다.
	void startup ( ) override
	{
		// 셰이더를 로드하고 컴파일합니다.
		shader_program = sb7::shader::load ( "tetris.vs.glsl" , "tetris.fs.glsl" );

		// 셰이더의 uniform 변수들의 위치를 얻어옵니다.
		uniforms.M = glGetUniformLocation ( shader_program , "M" );
		uniforms.V = glGetUniformLocation ( shader_program , "V" );
		uniforms.P = glGetUniformLocation ( shader_program , "P" );
		uniforms.mode = glGetUniformLocation ( shader_program , "mode" );

		// 게임 객체들을 생성합니다.
		gameBoard = new Board ( );
		currentTetromino = new Tetromino ( );
		gameScore = new Score ( );

		// 게임을 초기화합니다.
		restartGame ( );

		// 난수 시드를 초기화합니다.
		srand ( ( unsigned int ) time ( NULL ) );
	}

	// 애플리케이션의 기본 정보를 설정합니다.
	void configure ( sb7::application::app_config& config ) override
	{
		config.title = "Tetris 3D";
	}

	// 애플리케이션 종료 시 한 번 호출됩니다.
	void shutdown ( ) override
	{
		glDeleteProgram ( shader_program );
		delete gameBoard;
		delete currentTetromino;
		delete gameScore;
	}

	// 게임 재시작 함수
	void restartGame ( )
	{
		gameOver = false;
		gameBoard->reset ( );
		currentTetromino->spawnNew ( );
		gameScore->reset ( );
		lastFallTime = 0.0;
		fallSpeed = GameConstants::INITIAL_FALL_TIME;
		fastDrop = false;
		// set_title("Tetris 3D - Score: 0"); <- [수정됨] 잘못된 함수 호출
		glfwSetWindowTitle ( window , "Tetris 3D - Score: 0" ); // [수정됨] 올바른 함수 호출
	}

	// 키보드 입력 처리 함수를 오버라이드합니다.
	void onKey ( int key , int action ) override
	{
		if ( action == GLFW_PRESS || action == GLFW_REPEAT )
		{
			if ( gameOver )
			{
				if ( key == GLFW_KEY_R )
				{
					restartGame ( );
				}
				return;
			}

			// 임시 테트로미노를 만들어 이동/회전 가능 여부를 먼저 확인합니다.
			Tetromino testTetromino = *currentTetromino;

			switch ( key )
			{
			case GLFW_KEY_LEFT:
				testTetromino.move ( -1 , 0 );
				if ( !gameBoard->checkCollision ( testTetromino ) )
					*currentTetromino = testTetromino;
				break;
			case GLFW_KEY_RIGHT:
				testTetromino.move ( 1 , 0 );
				if ( !gameBoard->checkCollision ( testTetromino ) )
					*currentTetromino = testTetromino;
				break;
			case GLFW_KEY_UP:
				testTetromino.rotate ( 1 ); // 시계 방향 회전
				if ( !gameBoard->checkCollision ( testTetromino ) )
					*currentTetromino = testTetromino;
				break;
			case GLFW_KEY_DOWN:
				if ( !fastDrop ) {
					fallSpeed = GameConstants::FAST_FALL_TIME;
					fastDrop = true;
				}
				break;
			case GLFW_KEY_R:
				restartGame ( );
				break;
			}
		}
		else if ( action == GLFW_RELEASE )
		{
			if ( key == GLFW_KEY_DOWN )
			{
				fallSpeed = GameConstants::INITIAL_FALL_TIME;
				fastDrop = false;
			}
		}
	}
};

// 애플리케이션 인스턴스를 선언하고 실행합니다.
DECLARE_MAIN ( tetris_app )
