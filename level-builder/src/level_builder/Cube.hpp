#pragma once

enum class cube_type : std::uint32_t
{
	BORDER_CUBE,
	REGULAR_CUBE,
	SPAWN_CUBE,
	END_CUBE
};

class Cube
{
private:
	unsigned int _width;
	unsigned int _height;

	int _x;
	int _y;

	std::uint32_t _pos;

	cube_type _type;

	const D2D1_COLOR_F calculate_color();
public:
	Cube() : _pos(-1) {};

	void set_size(std::uintptr_t width, std::uint32_t height);
	void set_position(int x, int y);

	void set_type(const cube_type& type);
	void set_pos(const std::uint32_t pos);
	void update(std::vector<Cube>* cubes);

	const std::uint32_t get_vec_pos();
	const D2D1_POINT_2F get_position();
	const cube_type get_type();

	void draw(ID2D1RenderTarget* render_target, ID2D1SolidColorBrush* solid_brush);
};
