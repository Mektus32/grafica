using point_t = struct s_point {
	s_point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	s_point(double x, double y) {
		this->x = x;
		this->y = y;
		this->z = 0;
	}
	s_point() = default;
	s_point(const s_point& point) = default;
	double x;
	double y;
	double z;
};

point_t operator*(const point_t& lhs, const point_t& rhs) {
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

bool operator==(const point_t& lhs, const point_t& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

using vector_t = struct s_vector {
	s_vector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	double x;
	double y;
	double z;
};

class vertex {
public:
	point_t figure_bot;
	point_t figure_top;
	point_t texture_extreme_bot;
	point_t texture_extreme_top;
	point_t texture_side_bot;
	point_t texture_side_top;
	vertex() = default;
	explicit vertex(const point_t& fig_bot);
	explicit vertex(const point_t& fig_bot, const point_t& tex_extreme_bot);
	explicit vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t tex_extreme_top);
	explicit vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_side_bot, const point_t& tex_side_top);
	explicit vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_extreme_top, const point_t& tex_side_bot, const point_t& tex_side_top);
	void rotate_top_point(double degrees);
};

vertex::vertex(const point_t& fig_bot) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = point_t();
	texture_extreme_top = point_t();
	point_t texture_side_bot = point_t();
	point_t texture_side_top = point_t();
}

vertex::vertex(const point_t& fig_bot, const point_t& tex_extreme_bot) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = tex_extreme_bot;
	texture_extreme_top = point_t();
	point_t texture_side_bot = point_t();
	point_t texture_side_top = point_t();
}

vertex::vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t tex_extreme_top) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = tex_extreme_bot;
	texture_extreme_top = tex_extreme_top;
	point_t texture_side_bot = point_t();
	point_t texture_side_top = point_t();
}

vertex::vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_extreme_top, const point_t& tex_side_bot, const point_t& tex_side_top) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = tex_extreme_bot;
	texture_extreme_top = point_t();
	point_t texture_side_bot = tex_side_bot;
	point_t texture_side_top = tex_side_top;
}

vertex::vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_side_bot, const point_t& tex_side_top) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = tex_extreme_bot;
	texture_extreme_top = point_t();
	point_t texture_side_bot = tex_side_bot;
	point_t texture_side_top = tex_side_top;
}

void vertex::rotate_top_point(double degrees) {
	double radians = degrees * M_PI / 180;
	figure_top = { figure_top.x * cos(radians) - figure_top.y * sin(radians), figure_top.x * sin(radians) + figure_top.y * cos(radians), figure_top.z };
}

bool operator==(const vertex& lhs, const vertex& rhs) {
	return lhs.figure_bot == rhs.figure_bot &&
		lhs.figure_top == rhs.figure_top &&
		lhs.texture_extreme_bot == rhs.texture_extreme_bot &&
		lhs.texture_extreme_top == rhs.texture_extreme_top &&
		lhs.texture_side_bot == rhs.texture_side_bot &&
		lhs.texture_side_top == rhs.texture_side_top;
}

using points = std::vector<point_t>;
using vertexes = std::vector<vertex>;


/*
		{40, 52},//0
		{60, 45},//1
		{70, 142},//2
		{91, 135},//3
		{115, 277},//4
		{136, 271},//5
		{162, 420},//6
		{183, 413},//7
		{272, 316},//8
		{292, 306},//9
		{339, 451},//10
		{358, 440},//11
		{204, 181},//12
		{285, 245},//13
		{294, 226},//14
		{418, 313},//15
		{428, 293},//16
		{294, 90},//17
		{294, 67},//18
		{506, 90},//19
		{506, 67},//20
		{204, 90},//21
		{204, 67},//22
		{83, 90},//23
		{83, 67},//24
		{159, 202},//25
		{0, 0},//26 (294, 226)
		{0, 0},//27 (294, 90)
		{0, 0},//28 (204, 90)
		{0, 0},//29 (159, 202)
		{0, 0},//30 (91, 135)
		{0, 0},//31 (136, 271)
		{0, 0},//32 (272, 316)
		{0, 0}//33 (204, 181)

		{4, -2},//0 yellow side circle
		{4, -2}//1 orange side panel
		{4, 4},//2
		{0, 0},//3 orange side panel 
		{0, 0},//4 pink side panel
		{0, 4},//5
		{-2, -1},//6 red side panel
		{-2, -1},//7 blue side panel
		{0, 0},//8
		{3, -6},//9 purple side circle
		{3, -6},//10 pink side panel
		{-2, -1},//11
		{-5, 2},//12
		{-3, -4}//13

		{
		{4, -2},//0
		{4, 4},//1
		{0, 0},//2
		{0, 4},//3
		{-2, -1},//4
		{0, 0},//5
		{3, -6},//6
		{-2, -1},//7
		{-5, 2},//8
		{-3, -4}//9
	};
*/