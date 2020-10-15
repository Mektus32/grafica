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
	void rotate_top_point(double degrees);
};

point_t operator*(const point_t& lhs, const point_t& rhs) {
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

point_t operator*(const point_t& lhs, double rhs) {
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

point_t operator+(const point_t& lhs, const point_t& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

point_t operator-(const point_t& lhs, const point_t& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

point_t operator/(const point_t& lhs, double rhs) {
	if (rhs == 0) {
		return { lhs.x / rhs, lhs.y / rhs, 0};
	}
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}


void s_point::rotate_top_point(double degrees) {
	double radians = degrees * M_PI / 180;
	point_t tmp = { x * cos(radians) - y * sin(radians), x * sin(radians) + y * cos(radians), z };
	*this = tmp;
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

using delta_t = struct s_delta {
	point_t figure;
	point_t texture;
};

delta_t operator*(const delta_t& lhs, double delta) {
	return {
		point_t(lhs.figure.x * delta, lhs.figure.y * delta, lhs.figure.z * delta),
		point_t(lhs.texture.x * delta, lhs.texture.y * delta, lhs.figure.z * delta)
	};
}

class vertex {
public:
	point_t figure_bot;
	point_t figure_top;
	point_t texture_extreme_bot;
	point_t texture_extreme_top;
	point_t texture_side_bot;
	point_t texture_side_top;
	vertex() = default;
	vertex(const point_t& fig_bot);
	vertex(const point_t& fig_bot, const point_t& tex_extreme_bot);
	vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t tex_extreme_top);
	vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_side_bot, const point_t& tex_side_top);
	vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_extreme_top, const point_t& tex_side_bot, const point_t& tex_side_top);
};

vertex::vertex(const point_t& fig_bot) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = point_t();
	texture_extreme_top = point_t();
	texture_side_bot = point_t();
	texture_side_top = point_t();
}

vertex::vertex(const point_t& fig_bot, const point_t& tex_extreme_bot) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = tex_extreme_bot;
	texture_extreme_top = point_t();
	texture_side_bot = point_t();
	texture_side_top = point_t();
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
	texture_extreme_top = tex_extreme_top;
	texture_side_bot = tex_side_bot;
	texture_side_top = tex_side_top;
}

vertex::vertex(const point_t& fig_bot, const point_t& tex_extreme_bot, const point_t& tex_side_bot, const point_t& tex_side_top) {
	figure_bot = fig_bot;
	figure_top = point_t();
	texture_extreme_bot = tex_extreme_bot;
	texture_extreme_top = point_t();
	texture_side_bot = tex_side_bot;
	texture_side_top = tex_side_top;
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
		{8, 20},//0
		{39, 8},//1
		{53, 155},//2
		{85, 144},//3
		{121, 358},//4
		{153, 348},//5
		{192, 574},//6
		{224, 563},//7
		{357, 416},//8
		{387, 401},//9
		{458, 619},//10
		{488, 603},//11
		{255, 213},//12
		{375, 310},//13
		{390, 280},//14
		{578, 412},//15
		{593, 382},//16
		{390, 76},//17
		{390, 43},//18
		{709, 76},//19
		{709, 43},//20
		{255, 76},//21
		{255, 43},//22
		{72, 76},//23
		{72, 43},//24
		{187, 246},//25
		{661, 670},//26 (294, 226)
		{661, 466},//27 (294, 90)
		{797, 467},//28 (204, 90)
		{864, 636},//29 (159, 202)
		{966, 534},//30 (91, 135)
		{898, 738},//31 (136, 271)
		{697, 803},//32 (272, 316)
		{797, 602}//33 (204, 181)

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