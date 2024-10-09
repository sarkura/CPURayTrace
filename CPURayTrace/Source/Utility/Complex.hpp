#pragma once
#include <cmath>

struct Complex
{
	union 
	{
		float A;
		float C;
	};
	union
	{
		float B;
		float D;
	};
	Complex(float InA, float InB) : A(InA), B(InB){}
	Complex(float InA) : A(InA), B(0.f) {}
};

// (a+bi) + (c + di) = (a + c) + (b + d)i
inline Complex operator+(Complex X, Complex Y)
{
	return { X.A + Y.C, X.B + Y.D };
}

// (a+bi) - (c + di) = (a - c) + (b - d)i
inline Complex operator-(Complex X, Complex Y)
{
	return { X.A - Y.C, X.B - Y.D };
}

// (a+bi) * (c + di) = (ac - bd) + (ad + bc)i
inline Complex operator*(Complex X, Complex Y)
{
	return { (X.A * Y.C - X.B * Y.D), (X.A * Y.D + X.B * Y.C) };
}

inline Complex operator*(Complex X, float Value)
{
	return { X.A * Value, X.B * Value };
}

// (a+bi) / (c + di) = ((ac - bd) + (bc - ad)) / (c * c + d * d)
inline Complex operator/(Complex X, Complex Y)
{
	float InverseCCDD = 1.0f / (Y.C * Y.C + Y.D * Y.D);

	return { (X.A * Y.C - X.B * Y.D) * InverseCCDD, (X.B * Y.C - X.A * Y.D) * InverseCCDD };
}

inline Complex operator/(Complex X, float Value)
{
	return { X.A / Value, X.B / Value };
}

inline float Normalize(Complex X)
{
	return std::sqrt(X.A * X.A + X.B * X.B);
}

inline Complex Sqrt(Complex X)
{
	float Norm = Normalize(X);
	return { std::sqrt((Norm + X.A) * 0.5f),std::sqrt((Norm - X.A) * 0.5f) };
}