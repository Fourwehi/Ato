#pragma once

#include <vmath.h>
#include <vector>

// GameConstants 네임스페이스를 사용하여 게임 전반에 사용되는 상수를 관리합니다.
// 이는 전역 스코프의 오염을 방지하고 코드의 가독성을 높입니다.
namespace GameConstants {

	// --- 보드 크기 및 위치 ---
	const int BOARD_WIDTH = 10;
	const int BOARD_HEIGHT = 20;
	const float BOARD_START_X = -5.0f; // 보드의 렌더링 시작 X 위치
	const float BOARD_START_Y = -10.0f; // 보드의 렌더링 시작 Y 위치
	const float CUBE_SIZE = 1.0f;     // 각 블록(큐브)의 크기

	// --- 게임 플레이 ---
	const int START_POS_X = 3; // 테트로미노 시작 X 위치 (그리드 인덱스)
	const int START_POS_Y = 0; // 테트로미노 시작 Y 위치 (그리드 인덱스)
	const float INITIAL_FALL_TIME = 0.8f; // 초기 블록 하강 속도 (초)
	const float FAST_FALL_TIME = 0.05f;   // 아래 방향키를 누르고 있을 때의 하강 속도

	// --- 테트로미노 색상 ---
	// 각 테트로미노 타입에 대한 색상을 vmath::vec4(R, G, B, A) 형태로 정의합니다.
	// 인덱스 0은 비어있는 상태를 의미하므로 사용하지 않습니다.
	const std::vector<vmath::vec4> PIECE_COLORS = {
		vmath::vec4 ( 0.2f, 0.2f, 0.2f, 1.0f ), // 0: 빈 공간 (배경색과 유사)
		vmath::vec4 ( 0.0f, 1.0f, 1.0f, 1.0f ), // 1: I (Cyan)
		vmath::vec4 ( 1.0f, 1.0f, 0.0f, 1.0f ), // 2: O (Yellow)
		vmath::vec4 ( 0.5f, 0.0f, 0.5f, 1.0f ), // 3: T (Purple)
		vmath::vec4 ( 0.0f, 0.0f, 1.0f, 1.0f ), // 4: J (Blue)
		vmath::vec4 ( 1.0f, 0.5f, 0.0f, 1.0f ), // 5: L (Orange)
		vmath::vec4 ( 0.0f, 1.0f, 0.0f, 1.0f ), // 6: S (Green)
		vmath::vec4 ( 1.0f, 0.0f, 0.0f, 1.0f )  // 7: Z (Red)
	};

	// --- 점수 ---
	const int SINGLE_LINE_SCORE = 100;
	const int DOUBLE_LINE_SCORE = 300;
	const int TRIPLE_LINE_SCORE = 500;
	const int TETRIS_SCORE = 800; // 4줄 동시 클리어 (테트리스)

} // namespace GameConstants
#pragma once
