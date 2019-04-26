#include "stdafx.hpp"
#include "include/SpinningEnemy.hpp"

const D2D1_POINT_2F SpinningEnemy::get_position()
{
	return position_;
}

const bool SpinningEnemy::check_collision(const D2D1_RECT_F & position)
{
	for (auto& base : bases_)
	{
		if (base.check_collision(position))
		{
			return true;
		}
	}

	return false;
}

void SpinningEnemy::set_enemies()
{
	bases_.clear();
	bases_.shrink_to_fit();

	BaseEnemy base;

	base.set_position(position_.x, position_.y);
	bases_.emplace_back(base);

	auto total = spacing * amount;

	for (auto i = spacing; i < total; i += spacing)
	{
		base.set_position(position_.x + i, position_.y);
		bases_.emplace_back(base);

		base.set_position(position_.x - i, position_.y);
		bases_.emplace_back(base);

		base.set_position(position_.x, position_.y + i);
		bases_.emplace_back(base);

		base.set_position(position_.x, position_.y - i);
		bases_.emplace_back(base);
	}
}

void SpinningEnemy::set_position(const float x, const float y)
{
	position_ = D2D1::Point2F(x, y);
}

void SpinningEnemy::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	for (auto& base : bases_)
	{
		auto x = base.get_position().x;
		auto y = base.get_position().y;

		base.set_position(std::cos(speed) * (x - position_.x) - std::sin(speed) * (y - position_.y) + position_.x,
			std::sin(speed) * (x - position_.x) + std::cos(speed) * (y - position_.y) + position_.y);

		base.draw(d2d1_rt, d2d1_solidbrush);
	}
}
