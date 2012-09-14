#ifndef JIT_CODEGEN_OP_HPP
#define JIT_CODEGEN_OP_HPP 1
#include "jit.codegen.h"
#include "math.codegen.h"

#define JIT_LNOT(x)			(!x)
#define JIT_ABSDIFF(x, y)	(ABS((x)-(y)))
#define JIT_EQ(x, y)		((x) == (y))
#define JIT_EQP(x, y)		((((x) == (y))) ? (x) : 0)
#define JIT_GT(x, y)		((x) > (y))
#define JIT_GTE(x, y)		((x) >= (y))
#define JIT_GTEP(x, y)		((((x) >= (y))) ? (x) : 0)
#define JIT_GTP(x, y)		((((x) > (y))) ? (x) : 0)
#define JIT_LAND(x, y)		((x) && (y))
#define JIT_LOR(x, y)		((x) || (y))
#define JIT_LT(x, y)		((x) < (y))
#define JIT_LTE(x, y)		((x) <= (y))
#define JIT_LTEP(x, y)		((((x) <= (y))) ? (x) : 0)
#define JIT_LTP(x, y)		((((x) < (y))) ? (x) : 0)
#define JIT_NEQ(x, y)		((x) != (y))
#define JIT_NEQP(x, y)		((((x) != (y))) ? (x) : 0)
#define JIT_SWITCH(x, y, z)	((x) ? (y) : (z))
#define JIT_LXOR(x, y)		JIT_LAND(JIT_LNOT(JIT_LAND(x,y)),JIT_LOR(x,y))
#define JIT_SIGN(x)			(((x) == 0) ? 0 : (((x) < 0) ? -1 : 1))


template<typename T>
T JIT_SMOOTHSTEP(T edge0, T edge1, T x) {
	T t = CLAMP((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}


// UNOPS
#define abs_fixed ABS
#define abs_long ABS
#define abs_float ABS
#define abs_double ABS

#define jit_gen_acos_float acosf
#define jit_gen_acos_double acos

#define jit_gen_acosh_float acoshf
#define jit_gen_acosh_double acosh

#define jit_gen_asin_float asinf
#define jit_gen_asin_double asin

#define jit_gen_asinh_float asinhf
#define jit_gen_asinh_double asinh

#define jit_gen_atan_float atanf
#define jit_gen_atan_double atan

#define jit_gen_atanh_float atanhf
#define jit_gen_atanh_double atanh

#define jit_gen_ceil_float ceilf
#define jit_gen_ceil_double ceil

#define jit_gen_cos_float cosf
#define jit_gen_cos_double cos

#define jit_gen_cosh_float coshf
#define jit_gen_cosh_double cosh

#define jit_gen_exp_float expf
#define jit_gen_exp_double exp

#define jit_gen_exp2_float exp2f
#define jit_gen_exp2_double exp2

#define jit_gen_floor_float floorf
#define jit_gen_floor_double floor

#define fract_float(x) ((x) - jit_gen_floor_float(x))
#define fract_double(x) ((x) - jit_gen_floor_double(x))

#define jit_gen_log_float logf
#define jit_gen_log_double log

#define jit_gen_log2_float log2f
#define jit_gen_log2_double log2f

#define jit_gen_log10_float log10f
#define jit_gen_log10_double log10f

#define lnot_fixed(x) fixedLogic1(JIT_LNOT(x))
#define lnot_long JIT_LNOT
#define lnot_float JIT_LNOT
#define lnot_double JIT_LNOT

#define jit_gen_round_float roundf
#define jit_gen_round_double round

#define sign_fixed JIT_SIGN
#define sign_long JIT_SIGN
#define sign_float JIT_SIGN
#define sign_double JIT_SIGN

#define jit_gen_sin_float sinf
#define jit_gen_sin_double sin

#define jit_gen_sinh_float sinhf
#define jit_gen_sinh_double sinh

#define jit_gen_sqrt_float sqrtf
#define jit_gen_sqrt_double sqrt

#define jit_gen_tan_float tanf
#define jit_gen_tan_double tan

#define jit_gen_tanh_float tanhf
#define jit_gen_tanh_double tanh

#define trunc_float jit_math_trunc
#define trunc_double jit_math_trunc


// BINOPS
#define absdiff_fixed JIT_ABSDIFF
#define absdiff_long JIT_ABSDIFF
#define absdiff_float JIT_ABSDIFF
#define absdiff_double JIT_ABSDIFF

#define jit_gen_atan2_float atan2f
#define jit_gen_atan2_double atan2f

#define eq_fixed JIT_EQ
#define eq_long JIT_EQ
#define eq_float JIT_EQ
#define eq_double JIT_EQ

#define eqp_fixed JIT_EQP
#define eqp_long JIT_EQP
#define eqp_float JIT_EQP
#define eqp_double JIT_EQP

#define gt_fixed JIT_GT
#define gt_long JIT_GT
#define gt_float JIT_GT
#define gt_double JIT_GT

#define gte_fixed JIT_GTE
#define gte_long JIT_GTE
#define gte_float JIT_GTE
#define gte_double JIT_GTE

#define gtep_fixed JIT_GTEP
#define gtep_long JIT_GTEP
#define gtep_float JIT_GTEP
#define gtep_double JIT_GTEP

#define gtp_fixed JIT_GTP
#define gtp_long JIT_GTP
#define gtp_float JIT_GTP
#define gtp_double JIT_GTP

#define hypot_float(x, y) jit_gen_sqrt_float((x)*(x)+(y)*(y))
#define hypot_double(x, y) jit_gen_sqrt_double((x)*(x)+(y)*(y))

#define land_fixed(x, y) fixedLogic1(JIT_LAND(x, y))
#define land_long JIT_LAND
#define land_float JIT_LAND
#define land_double JIT_LAND

#define lor_fixed(x, y) fixedLogic1(JIT_LOR(x, y))
#define lor_long JIT_LOR
#define lor_float JIT_LOR
#define lor_double JIT_LOR

#define lt_fixed JIT_LT
#define lt_long JIT_LT
#define lt_float JIT_LT
#define lt_double JIT_LT

#define lte_fixed JIT_LTE
#define lte_long JIT_LTE
#define lte_float JIT_LTE
#define lte_double JIT_LTE

#define ltep_fixed JIT_LTEP
#define ltep_long JIT_LTEP
#define ltep_float JIT_LTEP
#define ltep_double JIT_LTEP

#define ltp_fixed JIT_LTP
#define ltp_long JIT_LTP
#define ltp_float JIT_LTP
#define ltp_double JIT_LTP

#define min_fixed MIN
#define min_long MIN
#define min_float MIN
#define min_double MIN

#define max_fixed MAX
#define max_long MAX
#define max_float MAX
#define max_double MAX

#define mod_float jit_math_fmod
#define mod_double jit_math_fmod

#define neq_fixed JIT_NEQ
#define neq_long JIT_NEQ
#define neq_float JIT_NEQ
#define neq_double JIT_NEQ

#define neqp_fixed JIT_NEQP
#define neqp_long JIT_NEQP
#define neqp_float JIT_NEQP
#define neqp_double JIT_NEQP

#define jit_gen_pow_float powf
#define jit_gen_pow_double pow

#define lxor_fixed(x, y) fixedLogic1(JIT_LXOR(x, y))
#define lxor_long JIT_LXOR
#define lxor_float JIT_LXOR
#define lxor_double JIT_LXOR

// TERNOPS
#define foldop_fixed(x, hi, lo) FloatToFixed(jit_math_fold(FixedToFloat(x), FixedToFloat(hi), FixedToFloat(lo)))
#define foldop_long(x, hi, lo) long(jit_math_fold(float(x), float(hi), float(lo)))
#define foldop_float jit_math_fold
#define foldop_double jit_math_fold

#define smoothstep_fixed(edge0, edge1, x) FloatToFixed(JIT_SMOOTHSTEP(FixedToFloat(edge0), FixedToFloat(edge1), FixedToFloat(x)))
#define smoothstep_long(edge0, edge1, x) long(JIT_SMOOTHSTEP(float(edge0), float(edge1), float(x)))
#define smoothstep_float JIT_SMOOTHSTEP
#define smoothstep_double JIT_SMOOTHSTEP

#define switchop_fixed JIT_SWITCH
#define switchop_long JIT_SWITCH
#define switchop_float JIT_SWITCH
#define switchop_double JIT_SWITCH

#define wrapop_fixed(x, hi, lo) FloatToFixed(jit_math_wrap(FixedToFloat(x), FixedToFloat(hi), FixedToFloat(lo)))
#define wrapop_long(x, hi, lo) long(jit_math_wrap(float(x), float(hi), float(lo)))
#define wrapop_float jit_math_wrap
#define wrapop_double jit_math_wrap



#endif // JIT_CODEGEN_OP_HPP
