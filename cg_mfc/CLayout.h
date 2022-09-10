#pragma once
class CLayout
{
public:
	static CLayout& GetInstance() {
		static CLayout layout;
		return layout;
	}

	void Init(const CRect& rect, int rCount, int cCount) {
		screenLeftTop.x = -rect.Width() / 2;
		screenLeftTop.y = rect.Height() / 2;

		rowCount = (rCount < 1) ? 1 : rCount;
		colCount = (cCount < 1) ? 1 : cCount;

		if (rowCount == 1) {
			edgeLenY = rect.Height();
		}
		else {
			edgeLenY = (rect.Height() - margin * (rowCount - 1)) / rowCount;
		}

		if (colCount == 1) {
			edgeLenX = rect.Width();
		}
		else {
			edgeLenX = (rect.Width() - margin * (colCount - 1)) / colCount;
		}

		id = 0;
	}

	bool GetCurrentPosition(int& row, int& col)
	{
		// 3 ÐÐ 4ÁÐ
		row = id / colCount;
		col = id % colCount;
		return row < rowCount;
	}

	bool GetCurrentArea(CPoint& leftTop, CPoint& rightBottom) {
		int row, col;
		if (!GetCurrentPosition(row, col))
			return false;

		long d = 2 * margin + edgeLenX;
		leftTop.x = screenLeftTop.x + (2 * margin + edgeLenX) * col + margin;
		leftTop.y = screenLeftTop.y - (2 * margin + edgeLenY) * row - margin;

		rightBottom.x = leftTop.x + edgeLenX;
		rightBottom.y = leftTop.y - edgeLenY;

		return true;
	}

	void IncreaseId() {
		++id;
	}
private:
	CPoint screenLeftTop{};/*
	long screenWidth = 0;
	long screenHeight = 0;*/
	long edgeLenX = 0;
	long edgeLenY = 0;
	int id = 0;
	const long margin = 20;

	int rowCount = 1;
	int colCount = 1;
};

