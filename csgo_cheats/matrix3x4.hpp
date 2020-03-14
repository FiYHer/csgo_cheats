#pragma once

//¸¨Öú¾ØÕóÀà
class matrix3x4_class
{
private:
	float mat[3][4];
public:
	constexpr auto operator[](int i) const noexcept { return mat[i]; }
};