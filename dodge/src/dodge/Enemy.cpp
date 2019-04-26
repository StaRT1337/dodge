#include "stdafx.hpp"
#include "include/Enemy.hpp"

constexpr float speed = 4.0f;

void Enemy::add_point(const D2D1_POINT_2F& point)
{
	points_.emplace_back(point);
}

const bool Enemy::check_collision(const D2D1_RECT_F & position)
{
	return base_.check_collision(position);
}

void Enemy::clear()
{
	points_.clear();
	points_.shrink_to_fit();
}

void Enemy::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (pos_ == points_.size())
	{
		auto last_point = points_.front();
		auto point = points_.at(pos_ - 1);

		if (last_point.x != point.x || last_point.y != point.y)
		{
			std::reverse(points_.begin(), points_.end());
		}

		position_.x = -1;
		position_.y = -1;
	}

	if (position_.x == -1 && position_.y == -1)
	{
		auto point = points_.front();

		position_.x = point.x;
		position_.y = point.y;

		pos_ = 1;
	}

	auto point = points_.at(pos_);

	if (position_.x < point.x)
	{
		position_.x += speed;

		if (position_.x >= point.x)
		{
			position_.x = point.x;
			++pos_;
		}
	}
	else if (position_.x > point.x)
	{
		position_.x -= speed;

		if (position_.x <= point.x)
		{
			position_.x = point.x;
			++pos_;
		}
	}

	if (position_.y < point.y)
	{
		position_.y += speed;

		if (position_.y >= point.y)
		{
			position_.y = point.y;
			++pos_;
		}
	}
	else if (position_.y > point.y)
	{
		position_.y -= speed;

		if (position_.y <= point.y)
		{
			position_.y = point.y;
			++pos_;
		}
	}

	base_.set_position(position_.x, position_.y);
	base_.draw(d2d1_rt, d2d1_solidbrush);
}
