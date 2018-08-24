#pragma once
#include <math.h>
#include "Point.h"
class Vector2D {
public:
	double x, y;
	// 构造函数
	// 默认构造函数,不执行任何操作
	Vector2D() {}
	// 复制构造函数
	Vector2D(const Vector2D &a) : x(a.x), y(a.y) {}
	// 带参数的构造函数,值完成初始化
	Vector2D(double nx, double ny) : x(nx), y(ny) {}
	//传入两点构造向量
	Vector2D(Point a,Point b):x(b.x-a.x),y(b.y-a.y){}
	// 重载赋值运算符
	Vector2D &operator =(const Vector2D &a) {
		x = a.x; y = a.y;
		return *this;
	}
	// 重载比较运算符
	bool operator ==(const Vector2D &a) const {
		return x == a.x && y == a.y;
	}
	bool operator !=(const Vector2D &a) const {
		return x != a.x || y != a.y;
	}
	// 向量运算
	// 设置零向量
	void zero() { x = y = 0.0f; }
	// 重载负运算符
	Vector2D operator -() const { return Vector2D(-x, -y); }
	// 重载加减运算符
	Vector2D operator +(const Vector2D &a) const {
		return Vector2D(x + a.x, y + a.y);
	}
	Vector2D operator -(const Vector2D &a) const {
		return Vector2D(this->x-a.x, this->y-a.y);
	}
	// 重载标量乘、除法运算符
	Vector2D operator *(double a) const {
		return Vector2D(x*a, y*a);
	}
	Vector2D operator /(double a) const {
		double oneOverA = 1.0f / a; // 没有对除零检查
		return Vector2D(x*oneOverA, y*oneOverA);
	}
	// 重载？=运算符
	Vector2D &operator +=(const Vector2D &a) {
		x += a.x; y += a.y;
		return *this;
	}
	Vector2D &operator -=(const Vector2D &a) {
		x -= a.x; y -= a.y;
		return *this;
	}
	Vector2D &operator *=(double a) {
		x *= a; y *= a;
		return *this;
	}
	Vector2D &operator /=(double a) {
		double oneOverA = 1.0f / a;
		x *= oneOverA; y *= oneOverA;
		return *this;
	}
	// 向量标准化
	void normalize() {
		double magSq = x * x + y * y;
		if (magSq > 0.0f) { // 检查除零
			double oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
		}
	}
	// 向量点乘,重载乘法运算符
	double operator *(const Vector2D &a) const {
		return x * a.x + y * a.y ;
	}
};

// 求向量模
inline double vectorMag(const Vector2D &a) {
	return sqrt(a.x*a.x + a.y*a.y);
}

// 标量乘法
inline Vector2D operator *(double k, const Vector2D &v) {
	return Vector2D(k*v.x, k*v.y);
}
// 计算两点间距离
inline double distance(const Vector2D &a,const Vector2D &b) {
	double dx = a.x -b.x;
	double dy = a.y -b.y;
	return sqrt(dx*dx + dy * dy);
}

// 全局零向量
extern const Vector2D kZeroVector;