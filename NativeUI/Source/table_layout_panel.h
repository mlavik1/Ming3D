#ifndef NATIVEUI_TABLELAYOUTPANEL
#define NATIVEUI_TABLELAYOUTPANEL

#include "panel.h"

namespace NativeUI
{
	struct TableLayoutCell
	{
		Panel* mContainerPanel = nullptr;
		//Point mRelativeSize;
	};

	class TableLayoutPanel : public Panel
	{
	private:
		int mNumRows;
		int mNumColumns;
		TableLayoutCell* mCells;
		float* mColumnWidths;
		float* mRowHeights;

	public:
		TableLayoutPanel(Control* arg_parent, int arg_columns, int arg_rows);
		void UpdateCellTransforms();
		TableLayoutCell* GetCell(const int arg_col, const int arg_row);
		void SetColumnWidth(const int arg_column, const float arg_width);
		void SetRowHeight(const int arg_row, const float arg_height);
	};
}

#endif
