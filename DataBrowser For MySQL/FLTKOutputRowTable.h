#ifndef _FLTK_OUTPUT_ROW_TABLE_H_
#define _FLTK_OUTPUT_ROW_TABLE_H_
#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include <Fl\fl_draw.H>
#include <boost\array.hpp>
#include <boost\assign.hpp>
#include <Fl\Fl_Table_Row.H>
#include <boost\multi_array.hpp>

/* 注意行名称和列名称如果存在中文,类内不做调整,需要在外面进行调整 */
class Fl_Output_Table_Row :public Fl_Table_Row{
public:
	// 着色类型
	enum ColorSetType{ROW_TYPE, COL_TYPE};
private:
	// 选中行的背景颜色和前景颜色
	Fl_Color sel_bg_color_;
	Fl_Color sel_fg_color_;
	// 显示数据
	boost::multi_array<std::string, 2> data_;
	std::vector<std::string> col_name_;
	std::vector<std::string> row_name_;
	// 展示顺序的颜色
	std::vector<Fl_Color> order_color_;
	int order_color_len_;
	// 着色
	ColorSetType color_set_ty_;
	// 字体大小
	Fl_Fontsize font_size_;
public:
	void setColorSetType(Fl_Output_Table_Row::ColorSetType);
	// 构造函数
	Fl_Output_Table_Row(int, int, int, int, const char* lab_ = NULL);
	// 选中行颜色设置
	void setSelectedBgColor(Fl_Color);
	void setSelectedFgColor(Fl_Color);
	// 设置宽度
	void setColWidth(int);
	void setRowWidth(int);
	// 设置字体大小
	void setFontSize(Fl_Fontsize);
	// 设置数据
	void setDefaultData();
	void setData(boost::multi_array<std::string, 2>&);
	void setData(
		boost::multi_array<std::string, 2>&, // 数据
		std::vector<std::string>&,			 // 列名称 
		std::vector<std::string>&);          // 行名称
	/// draw cell 函数
	void draw_cell(Fl_Table::TableContext context_,	
		int r = 0, int c = 0, int x = 0, int y = 0, int w = 0, int h = 0);
};







#endif