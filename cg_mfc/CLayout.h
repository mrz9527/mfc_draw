#pragma once
class CLayout
{
public:
	static CLayout& GetInstance() {
		static CLayout layout;
		return layout;
	}

	void Init(const CRect& rect) {
		screenLeftTop.x = -rect.Width() / 2;
		screenLeftTop.y = rect.Height() / 2;

		screenWidth = rect.Width();
		screenHeight = rect.Height();

		edgeLen = 150;

		id = 0;
	}

	bool GetCurrentPosition(int& row, int& col)
	{
		long d = 2 * margin + edgeLen;
		long colNum = screenWidth / d;
		long rowNum = screenHeight / d;


		// 3 ÐÐ 4ÁÐ
		row = id / colNum;
		col = id % colNum;
		return row < rowNum;
	}

	bool GetCurrentArea(CPoint& leftTop, CPoint& rightBottom) {
		int row, col;
		if (!GetCurrentPosition(row, col))
			return false;

		long d = 2 * margin + edgeLen;
		leftTop.x = screenLeftTop.x + d * col + margin;
		leftTop.y = screenLeftTop.y - d * row - margin;

		rightBottom.x = leftTop.x + edgeLen;
		rightBottom.y = leftTop.y - edgeLen;

		return true;
	}

	void IncreaseId() {
		++id;
	}
private:
	CPoint screenLeftTop{};
	long screenWidth = 0;
	long screenHeight = 0;
	long edgeLen = 0;
	int id = 0;
	const long margin = 20;
};

