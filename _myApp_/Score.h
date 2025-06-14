#pragma once

#include "GameConstants.h"

// 점수 관리를 위한 Score 클래스입니다.
// 헤더 파일에 선언과 구현을 모두 포함하는 Header-only 방식으로 작성되었습니다.
class Score {
private:
	int currentScore; // 현재 점수를 저장하는 변수

public:
	// 생성자: 점수를 0으로 초기화합니다.
	Score ( ) {
		currentScore = 0;
	}

	// 소멸자
	~Score ( ) {
	}

	// 점수를 추가하는 함수입니다.
	// 한 번에 지운 줄의 수(linesCleared)에 따라 차등 점수를 부여합니다.
	void addScore ( int linesCleared ) {
		switch ( linesCleared ) {
		case 1:
			currentScore += GameConstants::SINGLE_LINE_SCORE;
			break;
		case 2:
			currentScore += GameConstants::DOUBLE_LINE_SCORE;
			break;
		case 3:
			currentScore += GameConstants::TRIPLE_LINE_SCORE;
			break;
		case 4:
			currentScore += GameConstants::TETRIS_SCORE;
			break;
		default:
			break;
		}
	}

	// 현재 점수를 반환하는 함수입니다.
	int getScore ( ) const {
		return currentScore;
	}

	// 점수를 초기화하는 함수입니다. 게임 재시작 시 호출됩니다.
	void reset ( ) {
		currentScore = 0;
	}
};
