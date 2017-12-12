#include "GameOfCells.h"
#include <random>
#include <iostream>

GameOfCells::GameOfCells(const sf::Vector2u & boardSize) : boardSize(boardSize)
{
	cellsToCheckInARow = new std::vector<Range>[boardSize.y];
	changedCellsInARow = new std::vector<ChangedCell>[boardSize.y];

	currentBoard = new int[boardSize.x * boardSize.y];
	for (size_t i = 0; i < boardSize.x * boardSize.y; i++)
		currentBoard[i] = 0;

	cellsToDraw.setPrimitiveType(sf::PrimitiveType::Points);
}

void GameOfCells::fillBoard(size_t seed)
{
	std::mt19937 eng(seed);
	std::uniform_int_distribution<size_t> value(0, 1);
	std::uniform_int_distribution<size_t> index(1, boardSize.x * boardSize.y - 2);

	for (size_t i = boardSize.x + 1; i < (boardSize.y - 1) * boardSize.x; i++)
	{
		int x = i % boardSize.x;
		int y = i / boardSize.x;
		if (x == boardSize.x - 1 || x == 0 || value(eng))	// go further only if value == 0
			continue;

		changedCellsInARow[y].push_back(ChangedCell(x, &currentBoard[i], 1));
	}
}

void GameOfCells::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(cellsToDraw, states);
}

void GameOfCells::addRange(int rowIndex, int cellXIndex)
{
	if (cellsToCheckInARow[rowIndex].size())
		if ((cellXIndex + 1) - cellsToCheckInARow[rowIndex].back().rangeEnd <= 3)
		{
			cellsToCheckInARow[rowIndex].back().rangeEnd = cellXIndex + 1;
			return;
		}
	cellsToCheckInARow[rowIndex].push_back(Range(cellXIndex - 1, cellXIndex + 1));
}

void GameOfCells::fillCellsToCheckInARow(int rowIndex)
{
	int i = 0;
	int upperRowCheckIndex = 0;
	int lowerRowCheckIndex = 0;
	int lowestXindex;

	int rowModifier = 0;

	while (true)
	{
		if (i >= changedCellsInARow[rowIndex].size() || rowModifier)
			if (upperRowCheckIndex < changedCellsInARow[rowIndex - 1].size())
			{
				rowModifier = -1;
				i = upperRowCheckIndex;
			}
			else if (lowerRowCheckIndex < changedCellsInARow[rowIndex + 1].size())
			{
				rowModifier = 1;
				i = lowerRowCheckIndex;
			}
			else break;

			lowestXindex = changedCellsInARow[rowIndex + rowModifier][i].xIndex;

			if (rowModifier != -1 && upperRowCheckIndex < changedCellsInARow[rowIndex - 1].size())
				if (changedCellsInARow[rowIndex - 1][upperRowCheckIndex].xIndex < lowestXindex)
				{
					lowestXindex = changedCellsInARow[rowIndex - 1][upperRowCheckIndex].xIndex;

					if (rowModifier != 1 && lowerRowCheckIndex < changedCellsInARow[rowIndex + 1].size())
						if (changedCellsInARow[rowIndex + 1][lowerRowCheckIndex].xIndex < lowestXindex)
						{
							addRange(rowIndex, lowestXindex = changedCellsInARow[rowIndex + 1][lowerRowCheckIndex].xIndex);
							lowerRowCheckIndex++;
							continue;
						}

					addRange(rowIndex, lowestXindex);
					upperRowCheckIndex++;
					continue;
				}


			if (rowModifier != 1 && lowerRowCheckIndex < changedCellsInARow[rowIndex + 1].size())
				if (changedCellsInARow[rowIndex + 1][lowerRowCheckIndex].xIndex < lowestXindex)
				{
					addRange(rowIndex, changedCellsInARow[rowIndex + 1][lowerRowCheckIndex].xIndex);
					lowerRowCheckIndex++;
					continue;
				}

			addRange(rowIndex, lowestXindex);

			if (rowModifier)
				if (rowModifier == -1)
					upperRowCheckIndex++;
				else
					lowerRowCheckIndex++;
			else
				i++;
	}
}

void GameOfCells::simulate()
{
	if (boardSize.x == 0 || boardSize.y == 0)
		return;

	cellsToDraw.clear();

	for (size_t i = 1; i < boardSize.y - 1; i++)
		cellsToCheckInARow[i].clear();

	for (size_t i = 1; i < boardSize.y - 1; i++)
		for (size_t j = 0; j < changedCellsInARow[i].size(); j++)
			*changedCellsInARow[i][j].cell = changedCellsInARow[i][j].nextGenValue;

	for (size_t i = 1; i < boardSize.y - 1; i++)
		fillCellsToCheckInARow(i);

	for (size_t i = 1; i < boardSize.y - 1; i++)
		changedCellsInARow[i].clear();

	size_t closeCells = 0;

	for (int y = 1; y < boardSize.y - 1; y++)
	{
		for (int r = 0; r < cellsToCheckInARow[y].size(); r++)
			for (int x = cellsToCheckInARow[y][r].rangeStart; x <= cellsToCheckInARow[y][r].rangeEnd; x++)
			{
				if (x == 0 || x == boardSize.x - 1)
					continue;

				closeCells = 0;

				for (auto i = x - 1; i <= x + 1; i++)
					for (auto j = y - 1; j <= y + 1; j++)
						if (i == x && j == y)
							continue;
						else
							closeCells += currentBoard[j * boardSize.x + i];

				auto index = y * boardSize.x + x;

				if (!currentBoard[index])
				{
					if (closeCells == 3)
						changedCellsInARow[y].push_back(ChangedCell(x, &currentBoard[index], !currentBoard[index]));
				}
				else if (closeCells != 2 && closeCells != 3)
					changedCellsInARow[y].push_back(ChangedCell(x, &currentBoard[index], !currentBoard[index]));
			}
	}

	for (int i = boardSize.x + 1; i < (boardSize.y - 1) * boardSize.x; i++)
		if (currentBoard[i])
			cellsToDraw.append(sf::Vertex(sf::Vector2f(i % boardSize.x, i / boardSize.x)));

	std::cout << "cells: " << cellsToDraw.getVertexCount() << std::endl;
}