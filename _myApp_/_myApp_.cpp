#include <sb7.h>
#include <vmath.h>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Obstacle {
	float x , y , width , height;
	float speed;
};

class CrossRoadGame : public sb7::application
{
public:
	float characterX = 0.0f;
	float characterY = -0.8f;
	const float characterSize = 0.1f;

	std::vector<Obstacle> obstacles;
	int score = 0;
	bool gameOver = false;
	bool gameStarted = false;
	int level = 1;
	int roadsPassed = 0;
	const int roadsPerLevel = 3;
	float obstacleSpeed = 0.003f;

	double previousTime = 0.0;

	void init ( ) override
	{
		sb7::application::init ( );
		info.title = "Cross Road Game (sb7)";
		info.samples = 4;
	}

	void startup ( ) override
	{
		glClearColor ( 0.0 , 0.0 , 0.0 , 1.0 );
		glEnable ( GL_DEPTH_TEST );
		srand ( static_cast< unsigned >( time ( 0 ) ) );
		generateObstacles ( );
	}

	void render ( double currentTime ) override
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		if ( gameStarted && !gameOver )
			update ( currentTime );

		if ( !gameStarted )
		{
			// 텍스트 렌더링은 SB7에서 별도 구현 필요 (여기선 생략)
		}
		else if ( gameOver )
		{
			// 텍스트 렌더링 생략
		}
		else
		{
			drawRoad ( );
			drawCharacter ( );

			for ( const auto& obs : obstacles )
				drawObstacle ( obs );
		}
	}

	void update ( double currentTime )
	{
		double deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		for ( auto& obs : obstacles )
		{
			obs.x += obs.speed * deltaTime * 60.0; // 보정된 속도
			if ( obs.x > 1.0f ) obs.x = -1.0f;
		}

		checkCollision ( );

		if ( characterY > 0.8f )
		{
			characterY = -0.8f;
			roadsPassed++;
			score += 10;

			if ( roadsPassed >= roadsPerLevel )
			{
				level++;
				roadsPassed = 0;
				obstacleSpeed += 0.002f;
				generateObstacles ( );
			}
		}
	}

	void onKey ( int key , int action ) override
	{
		if ( action != GLFW_PRESS )
			return;

		if ( !gameStarted || gameOver )
		{
			if ( key == GLFW_KEY_ENTER )
			{
				restart ( );
			}
			return;
		}

		switch ( key )
		{
		case GLFW_KEY_W: characterY += 0.1f; break;
		case GLFW_KEY_S: characterY -= 0.1f; break;
		case GLFW_KEY_A: characterX -= 0.1f; break;
		case GLFW_KEY_D: characterX += 0.1f; break;
		}
	}

	void restart ( )
	{
		gameStarted = true;
		gameOver = false;
		characterX = 0.0f;
		characterY = -0.8f;
		score = 0;
		level = 1;
		roadsPassed = 0;
		obstacleSpeed = 0.003f;
		generateObstacles ( );
	}

	void drawCharacter ( )
	{
		drawRectangle ( characterX , characterY , characterSize , characterSize , 0.0f , 1.0f , 0.0f );
	}

	void drawObstacle ( const Obstacle& obs )
	{
		drawRectangle ( obs.x , obs.y , obs.width , obs.height , 1.0f , 0.0f , 0.0f );
	}

	void generateObstacles ( )
	{
		obstacles.clear ( );
		float roadY[ ] = { -0.5f, 0.0f, 0.5f };
		for ( int j = 0; j < 3; ++j )
		{
			float y = roadY[ j ];
			for ( int i = 0; i < 5; ++i )
			{
				float width = 0.2f + ( rand ( ) / ( float ) RAND_MAX ) * 0.1f;
				float height = 0.1f;
				float x = -1.0f + ( rand ( ) / ( float ) RAND_MAX ) * 2.0f;
				float speed = 0.002f + ( rand ( ) / ( float ) RAND_MAX ) * 0.004f;
				obstacles.push_back ( { x, y, width, height, speed } );
			}
		}
	}

	void checkCollision ( )
	{
		for ( const auto& obs : obstacles )
		{
			if ( characterX < obs.x + obs.width &&
				characterX + characterSize > obs.x &&
				characterY < obs.y + obs.height &&
				characterY + characterSize > obs.y )
			{
				gameOver = true;
				return;
			}
		}
	}

	void drawRectangle ( float x , float y , float width , float height , float r , float g , float b )
	{
		glColor3f ( r , g , b );
		glBegin ( GL_QUADS );
		glVertex2f ( x , y );
		glVertex2f ( x + width , y );
		glVertex2f ( x + width , y + height );
		glVertex2f ( x , y + height );
		glEnd ( );
	}

	void drawRoad ( )
	{
		glColor3ub ( 95 , 96 , 91 );
		glBegin ( GL_QUADS );
		glVertex2f ( -1.0f , -0.5f );
		glVertex2f ( 1.0f , -0.5f );
		glVertex2f ( 1.0f , -0.2f );
		glVertex2f ( -1.0f , -0.2f );
		glEnd ( );

		// 동일하게 나머지 road, divider, footpath 추가 가능
		// 이 부분은 원본 drawRoad()에서 그대로 이식하면 됨
	}
};

DECLARE_MAIN ( CrossRoadGame )
