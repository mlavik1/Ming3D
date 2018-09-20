#include "table_layout_panel.h"

namespace NativeUI
{
	TableLayoutPanel::TableLayoutPanel(Control* arg_parent, int arg_columns, int arg_rows)
		: Panel(arg_parent)
	{
		mNumRows = arg_rows;
		mNumColumns = arg_columns;

		mColumnWidths = new float[mNumColumns];
		mRowHeights = new float[mNumRows];

		SetSize(1.0f, 1.0f);
		SetVerticalSizeMode(SizeMode::Relative);
		SetHorizontalSizeMode(SizeMode::Relative);

		const int dimension = mNumRows * mNumColumns;
		mCells = new TableLayoutCell[dimension];

		for (size_t x = 0; x < mNumColumns; x++)
		{
			mColumnWidths[x] = 1.0f / mNumColumns;
		}
		for (size_t y = 0; y < mNumRows; y++)
		{
			mRowHeights[y] = 1.0f / mNumRows;
		}
		for (size_t i = 0; i < dimension; i++)
		{
			mCells[i].mContainerPanel = new Panel(this);
		}

		UpdateCellTransforms();
	}

	void TableLayoutPanel::UpdateCellTransforms()
	{
		float currColX = 0.0f;
		for (size_t x = 0; x < mNumColumns; x++)
		{
			float rurrRowY = 0.0f;
			for (size_t y = 0; y < mNumRows; y++)
			{
				const float w = mColumnWidths[x];
				const float h = mRowHeights[y];

				const size_t i = x + (y*mNumColumns);
				TableLayoutCell& cell = mCells[i];
				Panel* panel = cell.mContainerPanel;

				panel->SetSize(w, h);
				panel->SetHorizontalSizeMode(SizeMode::Relative);
				panel->SetVerticalSizeMode(SizeMode::Relative);
				panel->SetPosition(currColX, rurrRowY);
				panel->SetHorizontalPositionMode(SizeMode::Relative);
				panel->SetVerticalPositionMode(SizeMode::Relative);

				rurrRowY += mRowHeights[y];
			}
			currColX += mColumnWidths[x];
		}
	}

	TableLayoutCell* TableLayoutPanel::GetCell(const int arg_col, const int arg_row)
	{
		return &mCells[arg_col + (arg_row * mNumColumns)];
	}

	void TableLayoutPanel::SetColumnWidth(const int arg_column, const float arg_width)
	{
		mColumnWidths[arg_column] = arg_width;
		UpdateCellTransforms();
	}

	void TableLayoutPanel::SetRowHeight(const int arg_row, const float arg_height)
	{
		mRowHeights[arg_row] = arg_height;
		UpdateCellTransforms();
	}
}
