/*
	This is a stripped-down header based on the MaxAPI
	For use within run-time code generation
*/

#ifndef _MATH_CODEGEN_H_
#define _MATH_CODEGEN_H_

#ifdef __cplusplus
extern "C" {
#endif

// jit.math.h
#define JIT_MATH_F32_PI (3.14159265358979323846f)
#define JIT_MATH_F32_TWO_PI (2.0f * JIT_MATH_F32_PI)
#define JIT_MATH_F32_HALF_PI (0.5f * JIT_MATH_F32_PI)
#define JIT_MATH_F32_INV_PI (1.0f / JIT_MATH_F32_PI)
#define JIT_MATH_F32_DEGTORAD (JIT_MATH_F32_PI / 180.0f)
#define JIT_MATH_F32_RADTODEG (180.0f/JIT_MATH_F32_PI)
#define JIT_MATH_F32_EPS (10e-6f)
#define JIT_MATH_F32_MAXVAL (3.402823466e+38F)

#define JIT_MATH_F64_PI (3.14159265358979323846)
#define JIT_MATH_F64_TWO_PI (2.0 * JIT_MATH_F64_PI)
#define JIT_MATH_F64_HALF_PI (0.5 * JIT_MATH_F64_PI)
#define JIT_MATH_F64_INV_PI (1.0 / JIT_MATH_F64_PI)
#define JIT_MATH_F64_DEGTORAD (JIT_MATH_F64_PI / 180.0)
#define JIT_MATH_F64_RADTODEG (180.0 / JIT_MATH_F64_PI)
#define JIT_MATH_F64_EPS (10e-6f)
#define JIT_MATH_F64_MAXVAL (1.7976931348623158e+308)

#define JIT_MATH_FIXED_PI (DoubleToFixed(JIT_MATH_F64_PI))
#define JIT_MATH_FIXED_TWO_PI (DoubleToFixed(JIT_MATH_F64_TWO_PI))
#define JIT_MATH_FIXED_HALF_PI (DoubleToFixed(JIT_MATH_F64_HALF_PI))
#define JIT_MATH_FIXED_INV_PI (DoubleToFixed(JIT_MATH_F64_INV_PI))
#define JIT_MATH_FIXED_DEGTORAD (DoubleToFixed(JIT_MATH_F64_DEGTORAD))
#define JIT_MATH_FIXED_RADTODEG (DoubleToFixed(JIT_MATH_F64_RADTODEG))
#define JIT_MATH_FIXED_EPS (DoubleToFixed(JIT_MATH_F64_EPS))
#define JIT_MATH_FIXED_MAXVAL (2147483647L)


// math.h:
extern double acos( double );
extern double asin( double );
extern double atan( double );
extern double atan2( double, double );
extern double cos( double );
extern double sin( double );
extern double tan( double );
extern double acosh( double );
extern double asinh( double );
extern double atanh( double );
extern double cosh( double );
extern double sinh( double );
extern double tanh( double );
extern double exp ( double );
extern double exp2 ( double );
extern double log ( double );
extern double log2 ( double );
extern double log10 ( double );
extern double fmod ( double, double );
extern double modf ( double, double * );
extern double fabs( double );
extern double hypot ( double, double );
extern double pow ( double, double );
extern double sqrt( double );
extern double ceil ( double );
extern double floor ( double );
extern double round ( double );
	
#ifndef WIN_VERSION
extern float acosf( float );
extern float asinf( float );
extern float atanf( float );
extern float atan2f( float, float );
extern float cosf( float );
extern float sinf( float );
extern float tanf( float );
extern float acoshf( float );
extern float asinhf( float );
extern float atanhf( float );
extern float coshf( float );
extern float sinhf( float );
extern float tanhf( float );
extern float expf( float );
extern float exp2f( float );
extern float logf( float );
extern float log2f( float );
extern float log10f( float );
extern float fmodf( float, float );
extern float fabsf( float );
extern float hypotf( float, float );
extern float powf( float, float );
extern float sqrtf( float );
extern float ceilf( float );
extern float floorf( float );
extern float roundf( float );

#else

#define acosf(x) ((float)acos((double)(x)))
#define asinf(x) ((float)asin((double)(x)))
#define atanf(x) ((float)atan((double)(x)))
#define atan2f(x,y) ((float)atan2((double)(x),(double)(y)))
#define cosf(x) ((float)cos((double)(x)))
#define sinf(x) ((float)sin((double)(x)))
#define tanf(x) ((float)tan((double)(x)))
#define acoshf(x) ((float)acosh((double)(x)))
#define asinhf(x) ((float)asinh((double)(x)))
#define atanhf(x) ((float)atanh((double)(x)))
#define coshf(x) ((float)cosh((double)(x)))
#define sinhf(x) ((float)sinh((double)(x)))
#define tanhf(x) ((float)tanh((double)(x)))
#define expf(x) ((float)exp((double)(x)))
#define exp2f(x) ((float)exp2((double)(x)))
#define logf(x) ((float)log((double)(x)))
#define log2f(x) ((float)log((double)(x)))
#define log10f(x) ((float)log10((double)(x)))
#define fmodf(x,y) ((float)fmod((double)(x),(double)(y)))
#define fabsf(x) ((float)fabs((double)(x)))
#define hypotf(x,y) ((float)hypot((double)(x),(double)(y)))
#define powf(x,y) ((float)pow((double)(x),(double)(y)))
#define sqrtf(x) ((float)sqrt((double)(x)))
#define ceilf(x) ((float)ceil((double)(x)))
#define floorf(x) ((float)floor((double)(x)))
#define roundf(x) ((float)round((double)(x)))

#endif //MAC_VERSION

#ifdef __cplusplus
}
#endif

#endif /* _MATH_CODEGEN_H_ */
