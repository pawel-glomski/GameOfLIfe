#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <queue>


class GameOfCells : public sf::Drawable
{
	struct Range
	{
		int rangeStart = 0;
		int rangeEnd = 0;

		Range(int rangeStart, int rangeEnd) : rangeStart(rangeStart), rangeEnd(rangeEnd) {}
	};

	struct ChangedCell
	{
		int xIndex = 0;
		int* cell = nullptr;
		int nextGenValue = 0;

		ChangedCell(int xIndex, int* cell, int nextGenValue) : xIndex(xIndex), cell(cell), nextGenValue(nextGenValue) {}
	};

public:
	GameOfCells (const sf::Vector2u & boardSize);

	void fillBoard(size_t seed);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void simulate();

private:
	
	void addRange(int rowIndex, int cellXIndex);
	void fillCellsToCheckInARow(int rowIndex);


private:
	sf::Vector2u				boardSize;

	int *						currentBoard;
	std::vector<ChangedCell> *	changedCellsInARow;
	std::vector<Range> *		cellsToCheckInARow;
	sf::VertexArray				cellsToDraw;

	sf::Clock					clock;
};

