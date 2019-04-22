#include "stdafx.hpp"

#include "include/Enemy.hpp"
#include "include/Utils.hpp"

constexpr float speed = 4.0f;

const bool Enemy::has_point(const D2D1_POINT_2F& point)
{
	if (points_.size() > 0)
	{
		auto last_point = points_.back();

		if (point.x == last_point.x && point.y == last_point.y)
		{
			return true;
		}
	}

	auto count = 0u;

	for (const auto& vec_point : points_)
	{
		if (point.x == vec_point.x && point.y == vec_point.y)
		{
			++count;
		}
	}

	return count > 2;
}

const D2D1_POINT_2F Enemy::last_point()
{
	return points_.back();
}

void Enemy::add_point(const D2D1_POINT_2F& point)
{
	has_points = true;
	points_.emplace_back(point);
}

const std::vector<D2D1_POINT_2F> Enemy::get_points()
{
	return original_points_;
}

const D2D1_POINT_2F Enemy::get_position()
{
	return position_;
}

void Enemy::save()
{
	original_points_ = points_;
}

void Enemy::clear()
{
	points_.clear();
	points_.shrink_to_fit();
}

void Enemy::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush, const POINT& mouse_position)
{
	if (!has_points) return;

	if (finished)
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
	else
	{
		d2d1_solidbrush->SetColor(Utils::create_d2d1_color(0, 0, 0, 255));

		D2D1_POINT_2F last_point;
		D2D1_POINT_2F next_point;

		for (auto iter = points_.begin(); iter != points_.end(); ++iter)
		{
			if (iter + 1 != points_.end())
			{
				next_point = *(iter + 1);

				d2d1_rt->DrawLine(
					*iter,
					next_point,
					d2d1_solidbrush,
					2.0f
				);
			}
			else
			{
				d2d1_rt->DrawLine(
					*iter,
					D2D1::Point2F(mouse_position.x, mouse_position.y),
					d2d1_solidbrush,
					2.0f
				);
			}
		}
	}
}
