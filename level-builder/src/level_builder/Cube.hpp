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
	unsigned int width_;
	unsigned int height_;

	int x_;
	int y_;

	std::uint32_t pos_;

	cube_type type_;

	const D2D1_COLOR_F calculate_color();
public:
	Cube() : pos_(-1) {};
	bool operator==(const Cube& cube);

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
