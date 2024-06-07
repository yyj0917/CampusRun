#pragma once

#include <ctime>
#include "Item.h"

class GameInfo {
protected:
	int negScore;
	int posScore;
	int time;
public:
	GameInfo() {
		this->negScore = 0;
		this->posScore = 0;
		this->time = 0;
	}

	~GameInfo() {

	}

	int getNegScore() {
		return this->negScore;
	}

	void setNegScore() {
		this->negScore += 1;
	}

	int getPosScore() {
		return this->posScore;
	}

	void setPosScore() {
		this->posScore += 1;
	}

	void setScore(Item* item) {
		if (item->getItemType()) {
			setPosScore();
		}
		else {
			setNegScore();
		}
	}

	void setScore(SpinItem* item) {
		if (item->getItemType()) {
			setPosScore();
		}
		else {
			setNegScore();
		}
	}
};