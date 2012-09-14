// This file automagically generated, do not edit
#ifndef JIT_CODEGEN_VEC_HPP
#define JIT_CODEGEN_VEC_HPP 1

#include "jit.codegen.h"
#include "math.codegen.h"

template <typename T>
struct Vec4;
template <typename T>
struct Vec3;
template <typename T>
struct Vec2;

template <typename T, unsigned N>
struct VecN{
public:
	T v[N];
	
	VecN(int n, const T *vv) {
		for(int i=0; i < MIN(N, n); i++) {
			v[i] = vv[i];
		}
	}
	
	VecN(const T *vv) {
		for(unsigned int i=0; i < N; i++) {
			v[i] = vv[i];
		}
	}
	
	VecN(const VecN &vn) {
		for(unsigned int i=0; i < N; i++) {
			v[i] = vn.v[i];
		}
	}
	
	VecN(T t) {
		for(unsigned int i=0; i < N; i++) {
			v[i] = t;
		}
	}
	
	VecN(const Vec4<T> &v4) {
		const T *vv = &(v4.x);
		for(unsigned int i=0; i < MIN(N, 4); i++) {
			v[i] = vv[i];
		}
	}
	
	VecN(const Vec3<T> &v3) {
		const T *vv = &(v3.x);
		for(unsigned int i=0; i < MIN(N, 3); i++) {
			v[i] = vv[i];
		}
	}
	
	VecN(const Vec2<T> &v2) {
		const T *vv = &(v2.x);
		for(unsigned int i=0; i < MIN(N, 2); i++) {
			v[i] = vv[i];
		}
	}
	
	VecN() {
		for(unsigned int i=0; i < N; i++) {
			v[i] = 0;
		}
	}
	
	T * ptr() {
		return v;
	}
	
	const T * ptr() const {
		return v;
	}
	
	void c0(T vv) {
		v[0] = vv;
	}
	
	void c1(T vv) {
		v[1] = vv;
	}
	
	T c0() const {
		return v[0];
	}
	
	T c1() const {
		return v[1];
	}
	
	/*! Negation */
	const VecN operator- () const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = -v[i];
		}
		return res;
	}
	
	/*! Logical Not */
	const VecN operator! () const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = !v[i];
		}
		return res;
	}

	/*! Addition */
	const VecN operator+ (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]+vn.v[i];
		}
		return res;
	}
	
	const VecN operator+ (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]+s;
		}
		return res;
	}
		
	/*! Subtraction */
	const VecN operator- (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]-vn.v[i];
		}
		return res;
	}
	
	const VecN operator- (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]-s;
		}
		return res;
	}
	
	/*! Multiplication */
	const VecN operator* (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]*vn.v[i];
		}
		return res;
	}
	
	const VecN operator* (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]*s;
		}
		return res;
	}
		
	/*! Division */
	const VecN operator/ (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]/vn.v[i];
		}
		return res;
	}
	
	const VecN operator/ (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]/s;
		}
		return res;
	}

	
	
	
	/*! Less Than */
	const VecN operator< (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]<s;
		}
		return res;
	}
	
	/*! Less Than Equal*/
	const VecN operator<= (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]<=vn.v[i];
		}
		return res;
	}
	
	const VecN operator<= (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]<=s;
		}
		return res;
	}
		
	/*! Greater Than */
	const VecN operator> (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]>vn.v[i];
		}
		return res;
	}
	
	const VecN operator> (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]>s;
		}
		return res;
	}
	
	/*! Greater Than Equal */
	const VecN operator>= (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]>=vn.v[i];
		}
		return res;
	}
	
	const VecN operator>= (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]>=s;
		}
		return res;
	}
	
	/*!  Equal */
	const VecN operator== (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]==vn.v[i];
		}
		return res;
	}
	
	const VecN operator== (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]==s;
		}
		return res;
	}
	
	/*!  Not Equal */
	const VecN operator!= (const VecN &vn) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]!=vn.v[i];
		}
		return res;
	}
	
	const VecN operator!= (T s) const {
		VecN res;
		for(unsigned int i=0; i < N; i++) {
			res.v[i] = v[i]!=s;
		}
		return res;
	}
};

/*! Addition */
template <typename T, unsigned N>
const VecN<T, N> operator+ (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s+vn.v[i];
	}
	return res;
}

/*! Subtraction */
template <typename T, unsigned N>
const VecN<T, N> operator- (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s-vn.v[i];
	}
	return res;
}

/*! Multiplication */
template <typename T, unsigned N>
const VecN<T, N> operator* (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s*vn.v[i];
	}
	return res;
}

/*! Division */
template <typename T, unsigned N>
const VecN<T, N> operator/ (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s/vn.v[i];
	}
	return res;
}

/*! Less Than */
template <typename T, unsigned N>
const VecN<T, N> operator< (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s<vn.v[i];
	}
	return res;
}

/*! Less Than Equal */
template <typename T, unsigned N>
const VecN<T, N> operator<= (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s<=vn.v[i];
	}
	return res;
}

/*! Greater Than */
template <typename T, unsigned N>
const VecN<T, N> operator> (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s>vn.v[i];
	}
	return res;
}

/*! Greater Than Equal */
template <typename T, unsigned N>
const VecN<T, N> operator>= (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s>=vn.v[i];
	}
	return res;
}


/*! Equal */
template <typename T, unsigned N>
const VecN<T, N> operator== (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s==vn.v[i];
	}
	return res;
}

/*! Not Equal */
template <typename T, unsigned N>
const VecN<T, N> operator!= (T s, const VecN<T, N> &vn) {
	VecN<T, N> res;
	for(unsigned int i=0; i < N; i++) {
		res.v[i] = s!=vn.v[i];
	}
	return res;
}

template <typename T, unsigned N>
const VecN<T, N> vec_from_long_array(t_jit_long n, t_jit_long *v) {
	VecN<T, N> rv;
	T *vv = rv.v;
	for(int i=0; i < MIN(n, N); i++) {
		vv[i] = (T)v[i];
	}
	return rv;
}


template <typename T>
struct Vec4{
public:
	T x;
	T y;
	T z;
	T w;


	Vec4(int n, T *v) {
		T *vv = &x;
		for(int i=0; i < MIN(4, n); i++) {
			vv[i] = v[i];
		}
	}
	
	Vec4(T *v) {
		T *vv = &x;
		for(int i=0; i < 4; i++) {
			vv[i] = v[i];
		}
	}
	
	Vec4(T x, T y, T z, T w)
	:	x(x), y(y), z(z), w(w)
	{}
	
	Vec4(const Vec4<T> &v)
	:	x(v.x), y(v.y), z(v.z), w(v.w)
	{}
	
	Vec4(const Vec3<T> &v)
	:	x(v.x), y(v.y), z(v.z), w(0)
	{}
	
	Vec4(const Vec2<T> &v)
	:	x(v.x), y(v.y), z(0), w(0)
	{}
	
	Vec4(const T v)
	:	x(v), y(v), z(v), w(v)
	{}
	
	Vec4()
	:	x(0), y(0), z(0), w(0)
	{}
	
	template <unsigned N>
	Vec4(const VecN<T, N> &vn) {
		T *vv = &x;
		for(int i=0; i < MIN(4, N); i++) {
			vv[i] = vn.v[i];
		}
		for(int i=MIN(4, N); i < MAX(4, N); i++) {
			vv[i] = 0;
		}
	}
	
	T * ptr() {
		return &x;
	}
	
	const T * ptr() const {
		return &x;
	}
	
	void c0(T vv) {
		x = vv;
	}
	
	void c1(T vv) {
		y = vv;
	}
	
	T c0() const {
		return x;
	}
	
	T c1() const {
		return y;
	}
	
	/*! Negation */
	const Vec4 operator- () const {
		return Vec4(-x, -y, -z, -w);
	}
	
	/*! Logical Not */
	const Vec4 operator! () const {
		return Vec4(!x, !y, !z, !w);
	}
	
	/*! Addition */
	const Vec4 operator+ (const Vec4 &v) const {
		return Vec4(x+v.x, y+v.y, z+v.z, w+v.w);
	}
	
	
	const Vec4 operator+ (T v) const {
		return Vec4(x+v, y+v, z+v, w+v);
	}
	
	
	/*! Subtraction */
	const Vec4 operator- (const Vec4 &v) const {
		return Vec4(x-v.x, y-v.y, z-v.z, w-v.w);
	}
	
	const Vec4 operator- (T v) const {
		return Vec4(x-v, y-v, z-v, w-v);
	}
	
	/*! Multiplication */
	const Vec4 operator* (const Vec4 &v) const {
		return Vec4(x*v.x, y*v.y, z*v.z, w*v.w);
	}
	
	const Vec4 operator* (T v) const {
		return Vec4(x*v, y*v, z*v, w*v);
	}
	
	/*! Division */
	const Vec4 operator/ (const Vec4 &v) const {
		return Vec4(x/v.x, y/v.y, z/v.z, w/v.w);
	}
	
	const Vec4 operator/ (T _v) const {
		float v = 1/_v;
		return Vec4(x*v, y*v, z*v, w*v);
	}
	
	/*! Less Than */
	const Vec4 operator< (const Vec4 &v) const {
		return Vec4(x<v.x, y<v.y, z<v.z, w<v.w);
	}
	
	const Vec4 operator< (T v) const {
		return Vec4(x<v, y<v, z<v, w<v);
	}
	
	/*! Less Than Equal */
	const Vec4 operator<= (const Vec4 &v) const {
		return Vec4(x<=v.x, y<=v.y, z<=v.z, w<=v.w);
	}
	
	const Vec4 operator<= (T v) const {
		return Vec4(x<=v, y<=v, z<=v, w<=v);
	}
	
	/*! Greater Than */
	const Vec4 operator> (const Vec4 &v) const {
		return Vec4(x>v.x, y>v.y, z>v.z, w>v.w);
	}
	
	const Vec4 operator> (T v) const {
		return Vec4(x>v, y>v, z>v, w>v);
	}
		
	/*! Greater Than Equal */
	const Vec4 operator>= (const Vec4 &v) const {
		return Vec4(x>=v.x, y>=v.y, z>=v.z, w>=v.w);
	}
	
	const Vec4 operator>= (T v) const {
		return Vec4(x>=v, y>=v, z>=v, w>=v);
	}
	
	/*! Equal */
	const Vec4 operator== (const Vec4 &v) const {
		return Vec4(x==v.x, y==v.y, z==v.z, w==v.w);
	}
	
	const Vec4 operator== (T v) const {
		return Vec4(x==v, y==v, z==v, w==v);
	}
	
	/*! Not Equal */
	const Vec4 operator!= (const Vec4 &v) const {
		return Vec4(x!=v.x, y!=v.y, z!=v.z, w!=v.w);
	}
	
	const Vec4 operator!= (T v) const {
		return Vec4(x!=v, y!=v, z!=v, w!=v);
	}
};

/* Addition */
template <typename T>
const Vec4<T> operator+ (const T s, const Vec4<T> &v) {
	return Vec4<T>(s+v.x, s+v.y, s+v.z, s+v.w);
}

/* Subtraction */
template <typename T>
const Vec4<T> operator- (const T s, const Vec4<T> &v) {
	return Vec4<T>(s-v.x, s-v.y, s-v.z, s-v.w);
}

/* Multiplication */
template <typename T>
const Vec4<T> operator* (const T s, const Vec4<T> &v) {
	return Vec4<T>(s*v.x, s*v.y, s*v.z, s*v.w);
}

/* Division */
template <typename T>
const Vec4<T> operator/ (const T s, const Vec4<T> &v) {
	return Vec4<T>(s/v.x, s/v.y, s/v.z, s/v.w);
}

/* Less Than */
template <typename T>
const Vec4<T> operator< (const T s, const Vec4<T> &v) {
	return Vec4<T>(s<v.x, s<v.y, s<v.z, s<v.w);
}

/* Less Than Equal */
template <typename T>
const Vec4<T> operator<= (const T s, const Vec4<T> &v) {
	return Vec4<T>(s<=v.x, s<=v.y, s<=v.z, s<=v.w);
}

/* Greater Than */
template <typename T>
const Vec4<T> operator> (const T s, const Vec4<T> &v) {
	return Vec4<T>(s>v.x, s>v.y, s>v.z, s>v.w);
}

/* Greater Than Equal */
template <typename T>
const Vec4<T> operator>= (const T s, const Vec4<T> &v) {
	return Vec4<T>(s>=v.x, s>=v.y, s>=v.z, s>=v.w);
}

/* Equal */
template <typename T>
const Vec4<T> operator== (const T s, const Vec4<T> &v) {
	return Vec4<T>(s==v.x, s==v.y, s==v.z, s==v.w);
}

/* Not Equal */
template <typename T>
const Vec4<T> operator!= (const T s, const Vec4<T> &v) {
	return Vec4<T>(s!=v.x, s!=v.y, s!=v.z, s!=v.w);
}

template <typename T>
const Vec4<T> vec_from_long_array4(t_jit_long n, t_jit_long *v) {
	Vec4<T> rv;
	T *vv = &rv.x;
	for(int i=0; i < MIN(n, 4); i++) {
		vv[i] = (T)v[i];
	}
	return rv;
}
template <typename T>
struct Vec3{
public:
	T x;
	T y;
	T z;


	Vec3(int n, T *v) {
		T *vv = &x;
		for(int i=0; i < MIN(3, n); i++) {
			vv[i] = v[i];
		}
	}
	
	Vec3(T *v) {
		T *vv = &x;
		for(int i=0; i < 3; i++) {
			vv[i] = v[i];
		}
	}
	
	Vec3(T x, T y, T z)
	:	x(x), y(y), z(z)
	{}
	
	Vec3(const Vec4<T> &v)
	:	x(v.x), y(v.y), z(v.z)
	{}
	
	Vec3(const Vec3<T> &v)
	:	x(v.x), y(v.y), z(v.z)
	{}
	
	Vec3(const Vec2<T> &v)
	:	x(v.x), y(v.y), z(0)
	{}
	
	Vec3(const T v)
	:	x(v), y(v), z(v)
	{}
	
	Vec3()
	:	x(0), y(0), z(0)
	{}
	
	template <unsigned N>
	Vec3(const VecN<T, N> &vn) {
		T *vv = &x;
		for(int i=0; i < MIN(3, N); i++) {
			vv[i] = vn.v[i];
		}
		for(int i=MIN(3, N); i < MAX(3, N); i++) {
			vv[i] = 0;
		}
	}
	
	T * ptr() {
		return &x;
	}
	
	const T * ptr() const {
		return &x;
	}
	
	void c0(T vv) {
		x = vv;
	}
	
	void c1(T vv) {
		y = vv;
	}
	
	T c0() const {
		return x;
	}
	
	T c1() const {
		return y;
	}
	
	/*! Negation */
	const Vec3 operator- () const {
		return Vec3(-x, -y, -z);
	}
	
	/*! Logical Not */
	const Vec3 operator! () const {
		return Vec3(!x, !y, !z);
	}
	
	/*! Addition */
	const Vec3 operator+ (const Vec3 &v) const {
		return Vec3(x+v.x, y+v.y, z+v.z);
	}
	
	
	const Vec3 operator+ (T v) const {
		return Vec3(x+v, y+v, z+v);
	}
	
	
	/*! Subtraction */
	const Vec3 operator- (const Vec3 &v) const {
		return Vec3(x-v.x, y-v.y, z-v.z);
	}
	
	const Vec3 operator- (T v) const {
		return Vec3(x-v, y-v, z-v);
	}
	
	/*! Multiplication */
	const Vec3 operator* (const Vec3 &v) const {
		return Vec3(x*v.x, y*v.y, z*v.z);
	}
	
	const Vec3 operator* (T v) const {
		return Vec3(x*v, y*v, z*v);
	}
	
	/*! Division */
	const Vec3 operator/ (const Vec3 &v) const {
		return Vec3(x/v.x, y/v.y, z/v.z);
	}
	
	const Vec3 operator/ (T _v) const {
		float v = 1/_v;
		return Vec3(x*v, y*v, z*v);
	}
	
	/*! Less Than */
	const Vec3 operator< (const Vec3 &v) const {
		return Vec3(x<v.x, y<v.y, z<v.z);
	}
	
	const Vec3 operator< (T v) const {
		return Vec3(x<v, y<v, z<v);
	}
	
	/*! Less Than Equal */
	const Vec3 operator<= (const Vec3 &v) const {
		return Vec3(x<=v.x, y<=v.y, z<=v.z);
	}
	
	const Vec3 operator<= (T v) const {
		return Vec3(x<=v, y<=v, z<=v);
	}
	
	/*! Greater Than */
	const Vec3 operator> (const Vec3 &v) const {
		return Vec3(x>v.x, y>v.y, z>v.z);
	}
	
	const Vec3 operator> (T v) const {
		return Vec3(x>v, y>v, z>v);
	}
		
	/*! Greater Than Equal */
	const Vec3 operator>= (const Vec3 &v) const {
		return Vec3(x>=v.x, y>=v.y, z>=v.z);
	}
	
	const Vec3 operator>= (T v) const {
		return Vec3(x>=v, y>=v, z>=v);
	}
	
	/*! Equal */
	const Vec3 operator== (const Vec3 &v) const {
		return Vec3(x==v.x, y==v.y, z==v.z);
	}
	
	const Vec3 operator== (T v) const {
		return Vec3(x==v, y==v, z==v);
	}
	
	/*! Not Equal */
	const Vec3 operator!= (const Vec3 &v) const {
		return Vec3(x!=v.x, y!=v.y, z!=v.z);
	}
	
	const Vec3 operator!= (T v) const {
		return Vec3(x!=v, y!=v, z!=v);
	}
};

/* Addition */
template <typename T>
const Vec3<T> operator+ (const T s, const Vec3<T> &v) {
	return Vec3<T>(s+v.x, s+v.y, s+v.z);
}

/* Subtraction */
template <typename T>
const Vec3<T> operator- (const T s, const Vec3<T> &v) {
	return Vec3<T>(s-v.x, s-v.y, s-v.z);
}

/* Multiplication */
template <typename T>
const Vec3<T> operator* (const T s, const Vec3<T> &v) {
	return Vec3<T>(s*v.x, s*v.y, s*v.z);
}

/* Division */
template <typename T>
const Vec3<T> operator/ (const T s, const Vec3<T> &v) {
	return Vec3<T>(s/v.x, s/v.y, s/v.z);
}

/* Less Than */
template <typename T>
const Vec3<T> operator< (const T s, const Vec3<T> &v) {
	return Vec3<T>(s<v.x, s<v.y, s<v.z);
}

/* Less Than Equal */
template <typename T>
const Vec3<T> operator<= (const T s, const Vec3<T> &v) {
	return Vec3<T>(s<=v.x, s<=v.y, s<=v.z);
}

/* Greater Than */
template <typename T>
const Vec3<T> operator> (const T s, const Vec3<T> &v) {
	return Vec3<T>(s>v.x, s>v.y, s>v.z);
}

/* Greater Than Equal */
template <typename T>
const Vec3<T> operator>= (const T s, const Vec3<T> &v) {
	return Vec3<T>(s>=v.x, s>=v.y, s>=v.z);
}

/* Equal */
template <typename T>
const Vec3<T> operator== (const T s, const Vec3<T> &v) {
	return Vec3<T>(s==v.x, s==v.y, s==v.z);
}

/* Not Equal */
template <typename T>
const Vec3<T> operator!= (const T s, const Vec3<T> &v) {
	return Vec3<T>(s!=v.x, s!=v.y, s!=v.z);
}

template <typename T>
const Vec3<T> vec_from_long_array3(t_jit_long n, t_jit_long *v) {
	Vec3<T> rv;
	T *vv = &rv.x;
	for(int i=0; i < MIN(n, 3); i++) {
		vv[i] = (T)v[i];
	}
	return rv;
}
template <typename T>
struct Vec2{
public:
	T x;
	T y;


	Vec2(int n, T *v) {
		T *vv = &x;
		for(int i=0; i < MIN(2, n); i++) {
			vv[i] = v[i];
		}
	}
	
	Vec2(T *v) {
		T *vv = &x;
		for(int i=0; i < 2; i++) {
			vv[i] = v[i];
		}
	}
	
	Vec2(T x, T y)
	:	x(x), y(y)
	{}
	
	Vec2(const Vec4<T> &v)
	:	x(v.x), y(v.y)
	{}
	
	Vec2(const Vec3<T> &v)
	:	x(v.x), y(v.y)
	{}
	
	Vec2(const Vec2<T> &v)
	:	x(v.x), y(v.y)
	{}
	
	Vec2(const T v)
	:	x(v), y(v)
	{}
	
	Vec2()
	:	x(0), y(0)
	{}
	
	template <unsigned N>
	Vec2(const VecN<T, N> &vn) {
		T *vv = &x;
		for(int i=0; i < MIN(2, N); i++) {
			vv[i] = vn.v[i];
		}
		for(int i=MIN(2, N); i < MAX(2, N); i++) {
			vv[i] = 0;
		}
	}
	
	T * ptr() {
		return &x;
	}
	
	const T * ptr() const {
		return &x;
	}
	
	void c0(T vv) {
		x = vv;
	}
	
	void c1(T vv) {
		y = vv;
	}
	
	T c0() const {
		return x;
	}
	
	T c1() const {
		return y;
	}
	
	/*! Negation */
	const Vec2 operator- () const {
		return Vec2(-x, -y);
	}
	
	/*! Logical Not */
	const Vec2 operator! () const {
		return Vec2(!x, !y);
	}
	
	/*! Addition */
	const Vec2 operator+ (const Vec2 &v) const {
		return Vec2(x+v.x, y+v.y);
	}
	
	
	const Vec2 operator+ (T v) const {
		return Vec2(x+v, y+v);
	}
	
	
	/*! Subtraction */
	const Vec2 operator- (const Vec2 &v) const {
		return Vec2(x-v.x, y-v.y);
	}
	
	const Vec2 operator- (T v) const {
		return Vec2(x-v, y-v);
	}
	
	/*! Multiplication */
	const Vec2 operator* (const Vec2 &v) const {
		return Vec2(x*v.x, y*v.y);
	}
	
	const Vec2 operator* (T v) const {
		return Vec2(x*v, y*v);
	}
	
	/*! Division */
	const Vec2 operator/ (const Vec2 &v) const {
		return Vec2(x/v.x, y/v.y);
	}
	
	const Vec2 operator/ (T _v) const {
		float v = 1/_v;
		return Vec2(x*v, y*v);
	}
	
	/*! Less Than */
	const Vec2 operator< (const Vec2 &v) const {
		return Vec2(x<v.x, y<v.y);
	}
	
	const Vec2 operator< (T v) const {
		return Vec2(x<v, y<v);
	}
	
	/*! Less Than Equal */
	const Vec2 operator<= (const Vec2 &v) const {
		return Vec2(x<=v.x, y<=v.y);
	}
	
	const Vec2 operator<= (T v) const {
		return Vec2(x<=v, y<=v);
	}
	
	/*! Greater Than */
	const Vec2 operator> (const Vec2 &v) const {
		return Vec2(x>v.x, y>v.y);
	}
	
	const Vec2 operator> (T v) const {
		return Vec2(x>v, y>v);
	}
		
	/*! Greater Than Equal */
	const Vec2 operator>= (const Vec2 &v) const {
		return Vec2(x>=v.x, y>=v.y);
	}
	
	const Vec2 operator>= (T v) const {
		return Vec2(x>=v, y>=v);
	}
	
	/*! Equal */
	const Vec2 operator== (const Vec2 &v) const {
		return Vec2(x==v.x, y==v.y);
	}
	
	const Vec2 operator== (T v) const {
		return Vec2(x==v, y==v);
	}
	
	/*! Not Equal */
	const Vec2 operator!= (const Vec2 &v) const {
		return Vec2(x!=v.x, y!=v.y);
	}
	
	const Vec2 operator!= (T v) const {
		return Vec2(x!=v, y!=v);
	}
};

/* Addition */
template <typename T>
const Vec2<T> operator+ (const T s, const Vec2<T> &v) {
	return Vec2<T>(s+v.x, s+v.y);
}

/* Subtraction */
template <typename T>
const Vec2<T> operator- (const T s, const Vec2<T> &v) {
	return Vec2<T>(s-v.x, s-v.y);
}

/* Multiplication */
template <typename T>
const Vec2<T> operator* (const T s, const Vec2<T> &v) {
	return Vec2<T>(s*v.x, s*v.y);
}

/* Division */
template <typename T>
const Vec2<T> operator/ (const T s, const Vec2<T> &v) {
	return Vec2<T>(s/v.x, s/v.y);
}

/* Less Than */
template <typename T>
const Vec2<T> operator< (const T s, const Vec2<T> &v) {
	return Vec2<T>(s<v.x, s<v.y);
}

/* Less Than Equal */
template <typename T>
const Vec2<T> operator<= (const T s, const Vec2<T> &v) {
	return Vec2<T>(s<=v.x, s<=v.y);
}

/* Greater Than */
template <typename T>
const Vec2<T> operator> (const T s, const Vec2<T> &v) {
	return Vec2<T>(s>v.x, s>v.y);
}

/* Greater Than Equal */
template <typename T>
const Vec2<T> operator>= (const T s, const Vec2<T> &v) {
	return Vec2<T>(s>=v.x, s>=v.y);
}

/* Equal */
template <typename T>
const Vec2<T> operator== (const T s, const Vec2<T> &v) {
	return Vec2<T>(s==v.x, s==v.y);
}

/* Not Equal */
template <typename T>
const Vec2<T> operator!= (const T s, const Vec2<T> &v) {
	return Vec2<T>(s!=v.x, s!=v.y);
}

template <typename T>
const Vec2<T> vec_from_long_array2(t_jit_long n, t_jit_long *v) {
	Vec2<T> rv;
	T *vv = &rv.x;
	for(int i=0; i < MIN(n, 2); i++) {
		vv[i] = (T)v[i];
	}
	return rv;
}


#endif // JIT_CODEGEN_VEC_HPP

