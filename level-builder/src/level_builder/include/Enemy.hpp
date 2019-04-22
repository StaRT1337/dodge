#pragma once
#include "BaseEnemy.hpp"
#include "Cube.hpp"

class Enemy
{
private:
	BaseEnemy base_;

	std::vector<D2D1_POINT_2F> points_;
	std::vector<D2D1_POINT_2F> original_points_;
	D2D1_POINT_2F position_;

	unsigned int pos_;

public:
	bool has_points;
	bool finished;

	Enemy() : finished(false), has_points(false), pos_(-1), position_(D2D1_POINT_2F{ -1, -1 }) {};

	const bool has_point(const D2D1_POINT_2F& point);
	const D2D1_POINT_2F last_point();

	void add_point(const D2D1_POINT_2F& point);

	const std::vector<D2D1_POINT_2F> get_points();
	const D2D1_POINT_2F get_position();

	void save();
	void clear();

	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush, const POINT& mouse_position);
};
