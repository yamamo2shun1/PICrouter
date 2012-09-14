#ifndef JIT_CODEGEN_OP_GENERIC_HPP
#define JIT_CODEGEN_OP_GENERIC_HPP 1
#include "jit.codegen.op.hpp"
#include "jit.codegen.vec.hpp"
#include "jit.codegen.vec.fixed.hpp"
//#include "jit.codegen.vec.conversion.hpp"


float clamp(const float x, const float a, const float b) {
	return CLAMP(x, a, b);
}

Vec2<float> clamp(const Vec2<float> &x, const Vec2<float> &a, const Vec2<float> &b) {
	Vec2<float> rv;
	rv.x = CLAMP(x.x, a.x, b.x);
	rv.y = CLAMP(x.y, a.y, b.y);

	return rv;
}
	
Vec3<float> clamp(const Vec3<float> &x, const Vec3<float> &a, const Vec3<float> &b) {
	Vec3<float> rv;
	rv.x = CLAMP(x.x, a.x, b.x);
	rv.y = CLAMP(x.y, a.y, b.y);
	rv.z = CLAMP(x.z, a.z, b.z);

	return rv;
}
	
Vec4<float> clamp(const Vec4<float> &x, const Vec4<float> &a, const Vec4<float> &b) {
	Vec4<float> rv;
	rv.x = CLAMP(x.x, a.x, b.x);
	rv.y = CLAMP(x.y, a.y, b.y);
	rv.z = CLAMP(x.z, a.z, b.z);
	rv.w = CLAMP(x.w, a.w, b.w);

	return rv;
}

Fixed clamp(const Fixed x, const Fixed a, const Fixed b) {
	return CLAMP(x, a, b);
}

Vec2<Fixed> clamp(const Vec2<Fixed> &x, const Vec2<Fixed> &a, const Vec2<Fixed> &b) {
	Vec2<Fixed> rv;
	rv.x = CLAMP(x.x, a.x, b.x);
	rv.y = CLAMP(x.y, a.y, b.y);

	return rv;
}
	
Vec3<Fixed> clamp(const Vec3<Fixed> &x, const Vec3<Fixed> &a, const Vec3<Fixed> &b) {
	Vec3<Fixed> rv;
	rv.x = CLAMP(x.x, a.x, b.x);
	rv.y = CLAMP(x.y, a.y, b.y);
	rv.z = CLAMP(x.z, a.z, b.z);

	return rv;
}
	
Vec4<Fixed> clamp(const Vec4<Fixed> &x, const Vec4<Fixed> &a, const Vec4<Fixed> &b) {
	Vec4<Fixed> rv;
	rv.x = CLAMP(x.x, a.x, b.x);
	rv.y = CLAMP(x.y, a.y, b.y);
	rv.z = CLAMP(x.z, a.z, b.z);
	rv.w = CLAMP(x.w, a.w, b.w);

	return rv;
}

template <typename V>
V mix(const V &v1, const V &v2, const double amt) {
	return v1 + (v2-v1)*amt;
}

template <typename V>
V mix(const V &v1, const V &v2, const float amt) {
	return v1 + (v2-v1)*amt;
}

template <typename V>
V mix(const V &v1, const V &v2, const Fixed amt) {
	return v1 + (v2-v1)*amt;
}

template <typename V>
V mix(const V &v1, const V &v2, const V &amt) {
	return v1 + (v2-v1)*amt;
}

Fixed mix(const Fixed v1, const Fixed v2, const Fixed amt) {
	return v1 + FixMul((v2-v1), amt);
}

float mix(const float v1, const float v2, const float amt) {
	return v1 + (v2-v1)*amt;
}

double mix(const double v1, const double v2, const double amt) {
	return v1 + (v2-v1)*amt;
}


/* Geometric Ops */
template<typename T>
float length(const T &v) {
	return (float)v;
}

float length(const Vec2<float> &v) {
	return sqrt(v.x*v.x + v.y*v.y);
}

float length(const Vec3<float> &v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float length(const Vec4<float> &v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

double length(const Vec2<double> &v) {
	return sqrt(v.x*v.x + v.y*v.y);
}

double length(const Vec3<double> &v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

double length(const Vec4<double> &v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

Fixed length(const Fixed &v) {
	return v;
}

Fixed length(const Vec2<Fixed> &v_) {
	Vec2<float> v;
	v.x = FixedToFloat(v_.x);
	v.y = FixedToFloat(v_.y);
	return FloatToFixed(sqrt(v.x*v.x + v.y*v.y));
}

Fixed length(const Vec3<Fixed> &v_) {
	Vec3<float> v;
	v.x = FixedToFloat(v_.x);
	v.y = FixedToFloat(v_.y);
	v.z = FixedToFloat(v_.z);
	return FloatToFixed(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
}

Fixed length(const Vec4<Fixed> &v_) {
	Vec4<float> v;
	v.x = FixedToFloat(v_.x);
	v.y = FixedToFloat(v_.y);
	v.z = FixedToFloat(v_.z);
	v.w = FixedToFloat(v_.w);
	return FloatToFixed(sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w));
}

template<typename T>
float dot(const T &v1, const T &v2) {
	return (float)v1*v2;
}

template<>
float dot<Vec2<float> >(const Vec2<float> &v1, const Vec2<float> &v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

template<>
float dot<Vec3<float> >(const Vec3<float> &v1, const Vec3<float> &v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<>
float dot<Vec4<float> >(const Vec4<float> &v1, const Vec4<float> &v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

double dot(const Vec2<double> &v1, const Vec2<double> &v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

double dot(const Vec3<double> &v1, const Vec3<double> &v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

double dot(const Vec4<double> &v1, const Vec4<double> &v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

Fixed dot(const Fixed &v1, const Fixed &v2) {
	return FixMul(v1, v2);
}

Fixed dot(const Vec2<Fixed> &v1, const Vec2<Fixed> &v2) {
	return FixMul(v1.x, v2.x) + FixMul(v1.y, v2.y);
}

Fixed dot(const Vec3<Fixed> &v1, const Vec3<Fixed> &v2) {
	return FixMul(v1.x, v2.x) + FixMul(v1.y, v2.y) + FixMul(v1.z, v2.z);
}

Fixed dot(const Vec4<Fixed> &v1, const Vec4<Fixed> &v2) {
	return FixMul(v1.x, v2.x) + FixMul(v1.y, v2.y) + FixMul(v1.z, v2.z) + FixMul(v1.w, v2.w);
}
	
	
template <typename T>
Vec3<T> cross(const Vec3<T> &v1, const Vec3<T> &v2) {
	return Vec3<T>(
		v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x
	);
}

template <>
Vec3<t_jit_fixed> cross(const Vec3<t_jit_fixed> &v1, const Vec3<t_jit_fixed> &v2) {
	return Vec3<t_jit_fixed>(
		FixMul(v1.y, v2.z) - FixMul(v1.z, v2.y),
		FixMul(v1.z, v2.x) - FixMul(v1.x, v2.z),
		FixMul(v1.x, v2.y) - FixMul(v1.y, v2.x)
	);
}

template<typename T>
T normalize(const T &v) {
	float magsq = dot(v, v);
	if(magsq < JIT_MATH_F32_EPS) {
		return T(JIT_MATH_F32_EPS);
	}
	else {
		float mag = sqrt(magsq);
		return v/mag;
	}
}

template<typename T>
T faceforward(const T &N, const T& I, const T& Nref) {
	return dot(Nref, I) < 0. ? N : -N;
}

template<typename T>
T reflect(const T &I, const T& N) {
	return I - 2*dot(N, I)*N;
}

template<typename T>
T refract(const T &I, const T& N, float eta) {
	float NI = dot(N, I);
	float k = 1.0f - eta*eta*(1.f - NI*NI);
	if(k < 0.f) {
		return T(0.f);
	}
	else {
		return eta*I - (eta*NI+sqrt(k))*N;
	}
}


template <typename T>
Vec4<T> argb_to_rgba(const Vec4<T> &v) {
	return Vec4<T>(v.y, v.z, v.w, v.x);
}

template <typename T>
Vec4<T> rgb_to_rgba(const Vec3<T> &v) {
	return Vec4<T>(v.x, v.y, v.z, 1.);
}

template <typename T>
Vec4<T> lumalpha_to_rgba(const Vec2<T> &v) {
	return Vec4<T>(v.x, v.x, v.x, v.y);
}

template <typename T>
Vec4<T> luminance_to_rgba(const T &v) {
	return Vec4<T>(v, v, v, 1.);
}

template <typename T>
Vec4<T> alpha_to_rgba(const T &v) {
	return Vec4<T>(1., 1., 1., v);
}

template <typename T>
Vec4<T> rgba_to_argb(const Vec4<T> &v) {
	return Vec4<T>(v.w, v.x, v.y, v.z);
}


/******************************************************
*	Noise Functions
*/
template <typename T>
struct Noise {
	unsigned long idum;
	unsigned long idum2;
	
	Noise() { init(); }
	
	void init() {
		idum = jit_rand();
		idum2 = idum;
	}

	inline T operator()() {
		return 0;
	}
};

template <>
struct Noise<Fixed> {
	unsigned long idum;
	unsigned long idum2;
	
	Noise() { init(); }
	
	void init() {
		idum = jit_rand();
		idum2 = idum;
	}

	inline Fixed operator()() {
		idum  = 1664525L * idum2 + 1013904223L;
		idum2 = 1664525L * idum + 1013904223L;
		//return (idum2&0xFFFF0000)|(idum>>16L); //shift right to remove low order correlation
		//return (idum2&fixedChar255)|(idum>>16L); //shift right to remove low order correlation
		return (idum2&fixedChar255)|(idum>>16L); //shift right to remove low order correlation
		
		//return (idum>>16L); //shift right to remove low order correlation
	}
};

template<>
struct Noise< Vec2<Fixed> > {
	Noise<Fixed> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec2<Fixed> operator()() {
		return Vec2<Fixed>(
			noise(), noise()
		);
	}
};

template<>
struct Noise< Vec3<Fixed> > {
	Noise<Fixed> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec3<Fixed> operator()() {
		return Vec3<Fixed>(
			noise(), noise(), noise()
		);
	}
};

template<>
struct Noise< Vec4<Fixed> > {
	Noise<Fixed> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec4<Fixed> operator()() {
		return Vec4<Fixed>(
			noise(), noise(), noise(), noise()
		);
	}
};

#define jflone 	0x3f800000
#define jflmsk  0x007fffff

template<>
struct Noise<t_jit_float32> {
	unsigned long idum;
	unsigned long itemp;
	
	Noise() { init(); }
	
	void init() {
		idum = jit_rand();
	}

	inline t_jit_float32 operator()() {
		idum = 1664525L * idum + 1013904223L;
		itemp = jflone | (jflmsk & idum);
		return ((*(t_jit_float32 *)&itemp) - 1.0); 
	}
};

template<>
struct Noise< Vec2<t_jit_float32> > {
	Noise<t_jit_float32> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec2<t_jit_float32> operator()() {
		return Vec2<t_jit_float32>(
			noise(), noise()
		);
	}
};

template<>
struct Noise< Vec3<t_jit_float32> > {
	Noise<t_jit_float32> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec3<t_jit_float32> operator()() {
		return Vec3<t_jit_float32>(
			noise(), noise(), noise()
		);
	}
};


template<>
struct Noise< Vec4<t_jit_float32> > {
	Noise<t_jit_float32> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec4<t_jit_float32> operator()() {
		return Vec4<t_jit_float32>(
			noise(), noise(), noise(), noise()
		);
	}
};

template<>
struct Noise<t_jit_float64> {
	unsigned long idum;
	unsigned long itemp;
	
	Noise() { init(); }
	
	void init() {
		idum = jit_rand();
	}

	inline t_jit_float64 operator()() {
		idum = 1664525L * idum + 1013904223L;
		itemp = jflone | (jflmsk & idum);
		return (t_jit_float64)((*(t_jit_float32 *)&itemp) - 1.0); 
	}
};

template<>
struct Noise< Vec2<t_jit_float64> > {
	Noise<t_jit_float64> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec2<t_jit_float64> operator()() {
		return Vec2<t_jit_float64>(
			noise(), noise()
		);
	}
};

template<>
struct Noise< Vec3<t_jit_float64> > {
	Noise<t_jit_float64> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec3<t_jit_float64> operator()() {
		return Vec3<t_jit_float64>(
			noise(), noise(), noise()
		);
	}
};

template<>
struct Noise< Vec4<t_jit_float64> > {
	Noise<t_jit_float64> noise;
	
	Noise() { init(); }
	
	void init() {
		noise.init();
	}

	inline Vec4<t_jit_float64> operator()() {
		return Vec4<t_jit_float64>(
			noise(), noise(), noise(), noise()
		);
	}
};

template <typename T, unsigned N>
struct NoiseN {
	Noise<T> noise;
	
	NoiseN() { init(); }
	
	void init() {
		noise.init();
	}

	inline VecN<T,N> operator()() {
		T v[N];
		for(int i=0; i < N; i++) {
			v[i] = noise();
		}
		return VecN<T,N>(
			N, v
		);
	}
};

/******************************************************
*	RGB <-> HSL Functions
*/
template <typename T>
Vec3<T> rgb2hsl(const Vec3<T> &rgb) {
	Vec3<T> hsl;

	T vmin = MIN(MIN(rgb.x, rgb.y), rgb.z);
	T vmax = MAX(MAX(rgb.x, rgb.y), rgb.z);
	T delta = vmax-vmin;
	T deltasum = vmax+vmin;
	
	hsl.z = deltasum*0.5;

	if(delta == 0) {
		// grey
		hsl.x = 0;
		hsl.y = 0;
	}
	else {
		if(hsl.z < 0.5 ) {
			hsl.y = delta/deltasum;
		}
		else {
			hsl.y = delta/(2-vmax-vmin);
		}

		Vec3<T> drgb = ((vmax-rgb)*0.16666666666667 + 0.5*delta)/delta;
		
		if(rgb.x == vmax) {
			hsl.x = drgb.z-drgb.y;
		}
		else if(rgb.y == vmax) {
			hsl.x = 0.3333333333333 + drgb.x - drgb.z;
		}
		else {
			hsl.x = 0.6666666666667 + drgb.y - drgb.x;
		}
		
		
		if(hsl.x < 0) {
			hsl.x += 1;
		}
		else if(hsl.x > 1) {
			hsl.x -= 1;
		}
	}

	return hsl;
}

#define fixedOneThird FloatToFixed(0.3333333333333)
#define fixedTwoThird FloatToFixed(0.6666666666667)
#define fixedOneSixth FloatToFixed(0.16666666666667)
#define fixed2	(fixed1<<1)
#define fixed3	FloatToFixed(3)
#define fixed6	FloatToFixed(6)

template <>
Vec3<Fixed> rgb2hsl(const Vec3<Fixed> &rgb) {
	Vec3<Fixed> hsl;

	Fixed vmin = MIN(MIN(rgb.x, rgb.y), rgb.z);
	Fixed vmax = MAX(MAX(rgb.x, rgb.y), rgb.z);
	Fixed delta = vmax-vmin;
	Fixed deltasum = vmax+vmin;
	
	hsl.z = deltasum>>1;	// divide by half

	if(delta == 0) {
		// grey
		hsl.x = 0;
		hsl.y = 0;
	}
	else {
		if(hsl.z < fixedhalf ) {
			hsl.y = FixDiv(delta, deltasum);
		}
		else {
			hsl.y = FixDiv(delta, (fixed2-vmax-vmin));
		}

		Vec3<Fixed> drgb = ((vmax-rgb)*fixedOneSixth + (delta>>1))/delta;
		
		if(rgb.x == vmax) {
			hsl.x = drgb.z-drgb.y;
		}
		else if(rgb.y == vmax) {
			hsl.x = fixedOneThird + drgb.x - drgb.z;
		}
		else {
			hsl.x = fixedTwoThird + drgb.y - drgb.x;
		}
		
		
		if(hsl.x < 0) {
			hsl.x += fixed1;
		}
		else if(hsl.x > fixed1) {
			hsl.x -= fixed1;
		}
	}

	return hsl;
}

template <typename T>
Vec4<T> rgb2hsl(const Vec4<T> &rgba) {
	Vec4<T> hsla;

	T vmin = MIN(MIN(rgba.x, rgba.y), rgba.z);
	T vmax = MAX(MAX(rgba.x, rgba.y), rgba.z);
	T delta = vmax-vmin;
	T deltasum = vmax+vmin;
	
	hsla.z = deltasum*0.5;

	if(delta == 0) {
		// grey
		hsla.x = 0;
		hsla.y = 0;
	}
	else {
		if(hsla.z < 0.5 ) {
			hsla.y = delta/deltasum;
		}
		else {
			hsla.y = delta/(2-vmax-vmin);
		}

		Vec3<T> drgba = ((vmax-rgba)/6 + 0.5*delta)/delta;
		
		if(rgba.x == vmax) {
			hsla.x = drgba.z-drgba.y;
		}
		else if(rgba.y == vmax) {
			hsla.x = 0.3333333333333 + drgba.x - drgba.z;
		}
		else {
			hsla.x = 0.6666666666667 + drgba.y - drgba.x;
		}
		
		
		if(hsla.x < 0) {
			hsla.x += 1;
		}
		else if(hsla.x > 1) {
			hsla.x -= 1;
		}
	}

	hsla.w = rgba.w;
	return hsla;
}

template <>
Vec4<Fixed> rgb2hsl(const Vec4<Fixed> &rgba) {
	Vec3<Fixed> hsla;

	Fixed vmin = MIN(MIN(rgba.x, rgba.y), rgba.z);
	Fixed vmax = MAX(MAX(rgba.x, rgba.y), rgba.z);
	Fixed delta = vmax-vmin;
	Fixed deltasum = vmax+vmin;
	
	hsla.z = deltasum>>1;	// divide by half

	if(delta == 0) {
		// grey
		hsla.x = 0;
		hsla.y = 0;
	}
	else {
		if(hsla.z < fixedhalf ) {
			hsla.y = FixDiv(delta, deltasum);
		}
		else {
			hsla.y = FixDiv(delta, (fixed2-vmax-vmin));
		}

		Vec4<Fixed> drgba = ((vmax-rgba)*fixedOneSixth + (delta>>1))/delta;
		
		if(rgba.x == vmax) {
			hsla.x = drgba.z-drgba.y;
		}
		else if(rgba.y == vmax) {
			hsla.x = fixedOneThird + drgba.x - drgba.z;
		}
		else {
			hsla.x = fixedTwoThird + drgba.y - drgba.x;
		}
		
		
		if(hsla.x < 0) {
			hsla.x += fixed1;
		}
		else if(hsla.x > fixed1) {
			hsla.x -= fixed1;
		}
	}

	return hsla;
}

template <typename T>
T rgbhue(T v1, T v2, T h) {
	if(h < 0) {
		h += 1;
	}
	else if(h > 1) {
		h -= 1;
	}
	if((6*h) < 1) {
		return v1+(v2-v1)*6*h;
	}
	else if((2*h) < 1) {
		return v2;
	}
	else if((3*h) < 2) {
		return v1+(v2-v1)*(0.6666666666667-h)*6;
	}
	return v1;
}

template <>
Fixed rgbhue(Fixed v1, Fixed v2, Fixed h) {
	if(h < 0) {
		h += fixed1;
	}
	else if(h > fixed1) {
		h -= fixed1;
	}
	if(FixMul(fixed6, h) < fixed1) {
		return v1+FixMul(FixMul((v2-v1), fixed6), h);
	}
	else if((h<<1) < fixed1) {
		return v2;
	}
	else if(FixMul(fixed3, h) < fixed2) {
		return v1+FixMul(FixMul((v2-v1), (fixedTwoThird-h)), fixed6);
	}
	return v1;
}

template <typename T>
Vec3<T> hsl2rgb(const Vec3<T> &hsl) {
	Vec3<T> rgb;
	if(hsl.y == 0) {
		rgb.x = hsl.z;
		rgb.y = hsl.z;
		rgb.z = hsl.z;
	}
	else {
		T v2;
		if(hsl.z < 0.5 ) {
			v2 = hsl.z*(1+hsl.y);
		}
		else {
			v2 = (hsl.z+hsl.y) - (hsl.y*hsl.z);
		}

		T v1 = 2*hsl.z-v2;

		rgb.x = rgbhue<T>(v1, v2, hsl.x+0.3333333333333);
		rgb.y = rgbhue<T>(v1, v2, hsl.x);
		rgb.z = rgbhue<T>(v1, v2, hsl.x-0.3333333333333);
	}
	return rgb;
}

template <>
Vec3<Fixed> hsl2rgb(const Vec3<Fixed> &hsl) {
	Vec3<Fixed> rgb;
	if(hsl.y == 0) {
		rgb.x = hsl.z;
		rgb.y = hsl.z;
		rgb.z = hsl.z;
	}
	else {
		Fixed v2;
		if(hsl.z < fixedhalf ) {
			v2 = FixMul(hsl.z, (fixed1+hsl.y));
		}
		else {
			v2 = (hsl.z+hsl.y) - FixMul(hsl.y, hsl.z);
		}

		Fixed v1 = FixMul(fixed2, hsl.z)-v2;		
		rgb.x = rgbhue<Fixed>(v1, v2, hsl.x+fixedOneThird);
		rgb.y = rgbhue<Fixed>(v1, v2, hsl.x);
		rgb.z = rgbhue<Fixed>(v1, v2, hsl.x-fixedOneThird);
	}
	return rgb;
}

template <typename T>
Vec4<T> hsl2rgb(const Vec4<T> &hsla) {
	Vec4<T> rgba;
	if(hsla.y == 0) {
		rgba.x = hsla.z;
		rgba.y = hsla.z;
		rgba.z = hsla.z;
	}
	else {
		T v2;
		if(hsla.z < 0.5 ) {
			v2 = hsla.z*(1+hsla.y);
		}
		else {
			v2 = (hsla.z+hsla.y) - (hsla.y*hsla.z);
		}

		T v1 = 2*hsla.z-v2;

		rgba.x = rgbhue<T>(v1, v2, hsla.x+0.3333333333333);
		rgba.y = rgbhue<T>(v1, v2, hsla.x);
		rgba.z = rgbhue<T>(v1, v2, hsla.x-0.3333333333333);
	}
	
	rgba.w = hsla.w;
	return rgba;
}

template <>
Vec4<Fixed> hsl2rgb(const Vec4<Fixed> &hsla) {
	Vec4<Fixed> rgba;
	if(hsla.y == 0) {
		rgba.x = hsla.z;
		rgba.y = hsla.z;
		rgba.z = hsla.z;
	}
	else {
		Fixed v2;
		if(hsla.z < fixedhalf ) {
			v2 = FixMul(hsla.z, (fixed1+hsla.y));
		}
		else {
			v2 = (hsla.z+hsla.y) - FixMul(hsla.y, hsla.z);
		}

		Fixed v1 = FixMul(fixed2, hsla.z)-v2;		
		rgba.x = rgbhue<Fixed>(v1, v2, hsla.x+fixedOneThird);
		rgba.y = rgbhue<Fixed>(v1, v2, hsla.x);
		rgba.z = rgbhue<Fixed>(v1, v2, hsla.x-fixedOneThird);
	}
	rgba.w = hsla.w;
	return rgba;
}

#endif // JIT_CODEGEN_OP_GENERIC_HPP
