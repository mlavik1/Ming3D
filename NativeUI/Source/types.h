#ifndef NATIVEUI_TYPES
#define NATIVEUI_TYPES

namespace NativeUI
{
	struct Point
	{
	public:
		float x;
		float y;

		Point()
			: x(0.0f), y(0.0f)
		{
		}

		Point(const float& arg_x, const float& arg_y)
			: x(arg_x), y(arg_y)
		{
		}

		Point operator+(const Point& arg_other) const
		{
			return Point(x + arg_other.x, y + arg_other.y);
		}
	};

	enum SizeMode
	{
		Absolute,
		Relative
	};
}

#endif // NATIVEUI_TYPES
