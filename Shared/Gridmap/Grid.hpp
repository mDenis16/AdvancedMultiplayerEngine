#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <iostream>
#include <algorithm>
template<class T>
class GridCell {
public:
	GridCell() {};

	GridCell(int rowIndex, int collumnIndex, int index) : _rowIndex(rowIndex), _collumIndex(collumnIndex), _index(index) {}

	~GridCell() {};

	std::vector<T> Items;

	glm::vec3 mins, maxs;

	int _rowIndex = 0;
	int _collumIndex = 0;
	int _index = 0;
};

template<class T>
class Grid {
public:
	Grid(glm::vec3 mins, glm::vec3 maxs, glm::vec3 cellSize) : _min(mins), _max(maxs), _cellSize(cellSize) {

		auto delta = (maxs - mins);

		_Collumns = delta.x / _cellSize.x;
		_Rows = delta.y / _cellSize.y;
		int index = 0;
		for (size_t i = 0; i < _Collumns; i++)
			for (size_t j = 0; j < _Rows; j++)
			{
				auto cell = GridCell<T>(j, i, index++);
				cell.mins.x = mins.x + cell._collumIndex * _cellSize.x;
				cell.mins.y = mins.y + cell._rowIndex * _cellSize.y;


				cell.maxs = glm::vec3(cell.mins.x + _cellSize.x, cell.mins.y + _cellSize.y, 0);
				cell.Items.reserve(200);
				cells.push_back(cell);
				

			}
	}

	glm::vec3 _min;
	glm::vec3 _max;
	glm::vec3 _cellSize;
	int _Collumns, _Rows = 0;

	std::vector<GridCell<T>> cells;


	std::vector<GridCell<T>*> getNearby(GridCell<T>* cell) {
		std::vector<GridCell<T>*> refs;
		refs.reserve(9);

		for (size_t i = cell->_collumIndex; i < cell->_collumIndex + 3; i++) {
			if (i == 0 || i > _Collumns) continue;

			for (size_t j = cell->_rowIndex; j < cell->_rowIndex + 3; j++)
			{
				if (j == 0 || j > _Rows) continue;

				int index = _Rows * (i - 1) + (j - 1);
				std::clamp(index, 0, (int)(cells.size() - 1));


				refs.push_back(&cells[index]);

			}
		}

		return refs;
	}
	bool isNearby(GridCell<T>* cell, GridCell<T>* cellTarget) {
		
		std::vector<int> numbers;
		numbers.reserve(9);

		for (size_t i = cell->_collumIndex; i < cell->_collumIndex + 3; i++) {
			if (i == 0 || i > _Collumns) continue;

			for (size_t j = cell->_rowIndex; j < cell->_rowIndex + 3; j++)
			{
				if (j == 0 || j > _Rows) continue;

				int index = _Rows * (i - 1) + (j - 1);
				std::clamp(index, 0, (int)(cells.size() - 1));


				numbers.push_back(index);

			}
		}

		return std::find(numbers.begin(), numbers.end(), cellTarget->_index) != numbers.end();
	}
	GridCell<T>* getCell(glm::vec3 position) {




		auto delta = (_max - _min);

		int column = (position.x - _min.x) / _cellSize.x;
		int row = (position.y - _min.y) / _cellSize.y;

		if (column > _Collumns || column < 0) return nullptr;
		if (row > _Rows - 1 || row < 0) return nullptr;
		//std::cout << "Current column " << column << " row " << row << std::endl;

		int index = _Rows * column + row;

	//	std::cout << "cell index " << index << std::endl;
		return index > cells.size() - 1 ? nullptr : &cells[index];
	}
};