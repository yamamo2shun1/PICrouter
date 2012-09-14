#ifndef __JIT_COMMON_H__
#define __JIT_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN_VERSION
#pragma pack(2)
#endif

	// ext.h:
	typedef void *(*method)(void *, ...);
	typedef char *t_ptr;

	typedef struct symbol {
		char *s_name;                   ///< name: a c-string
		struct object *s_thing; ///< possible binding to a t_object
	} Symbol, t_symbol;

	typedef struct object t_object;
	typedef struct atom t_atom;

	extern t_symbol *gensym(const char *s);
	extern void post(char *fmt, ...);
	extern void error(char *fmt, ...);
	
	extern void object_post(t_object *x, char *s, ...);
	extern void object_error(t_object *x, char *s, ...);
	extern void object_warn(t_object *x, char *s, ...);
	
	/*
	extern t_symbol *class_nameget(t_class *c);
	extern t_class *class_findbyname(t_symbol *name_space, t_symbol *classname);
	extern void *object_new(t_symbol *name_space, t_symbol *classname, ...);
	extern void *object_new_typed(t_symbol *name_space, t_symbol *classname, long ac, t_atom *av);
	extern t_max_err object_free(void *x);
	*/
	extern void *object_method(void *x, t_symbol *s, ...);
	/*extern t_max_err object_method_typed(void *x, t_symbol *s, long ac, t_atom *av, t_atom *rv);

	extern method object_getmethod(void *x, t_symbol *s);
	extern t_symbol *object_classname(void *x);
	extern t_symbol *object_namespace(t_object *x);	// return the namespace this object's class is part of
	extern t_symbol *class_namespace(t_class *c);
	extern void *object_attach(t_symbol *name_space, t_symbol *s, void *x);
	extern t_max_err object_attach_byptr(void *x, void *registeredobject);
	extern t_max_err object_detach(t_symbol *name_space, t_symbol *s, void *x);
	extern t_max_err object_detach_byptr(void *x, void *registeredobject);
	extern t_max_err object_notify(void *x, t_symbol *s, void *data);
	extern t_class *object_class(void *x);
	extern t_max_err object_getvalueof(void *x, long *ac, t_atom **av);
	extern t_max_err object_setvalueof(void *x, long ac, t_atom *av);
	*/
	extern long atom_getlong(t_atom *a);
	extern float atom_getfloat(t_atom *a);
	extern t_symbol *atom_getsym(t_atom *a);
	extern void *atom_getobj(t_atom *a);
	extern long atom_gettype(t_atom *a);

	extern t_ptr sysmem_newptr(long size);
	extern t_ptr sysmem_newptrclear(long size);
	extern t_ptr sysmem_resizeptr(void *ptr, long newsize);
	extern t_ptr sysmem_resizeptrclear(void *ptr, long newsize);
	extern long sysmem_ptrsize(void *ptr);
	extern void sysmem_freeptr(void *ptr);
	extern void sysmem_copyptr(const void *src, void *dst, long bytes);


#define MAX_MATRIX (16)

#define FOUR_CHAR(x)	(x)

typedef long t_jit_err;

typedef unsigned char t_jit_char;
typedef long t_jit_long;
typedef int t_jit_int;
typedef float t_jit_float;
typedef float t_jit_float32;
typedef double t_jit_float64;
typedef double t_jit_double;

//errors
#define JIT_ERR_NONE 				0
#define JIT_ERR_GENERIC				FOUR_CHAR('EROR')
#define JIT_ERR_INVALID_OBJECT		FOUR_CHAR('INOB')
#define JIT_ERR_OBJECT_BUSY			FOUR_CHAR('OBSY')
#define JIT_ERR_OUT_OF_MEM			FOUR_CHAR('OMEM')
#define JIT_ERR_INVALID_PTR			FOUR_CHAR('INVP')
#define JIT_ERR_DUPLICATE			FOUR_CHAR('DUPL')
#define JIT_ERR_OUT_OF_BOUNDS		FOUR_CHAR('OBND')
#define JIT_ERR_INVALID_INPUT		FOUR_CHAR('INVI')
#define JIT_ERR_INVALID_OUTPUT		FOUR_CHAR('INVO')
#define JIT_ERR_MISMATCH_TYPE		FOUR_CHAR('MSTP')
#define JIT_ERR_MISMATCH_PLANE		FOUR_CHAR('MSPL')
#define JIT_ERR_MISMATCH_DIM		FOUR_CHAR('MSDM')
#define JIT_ERR_MATRIX_UNKNOWN		FOUR_CHAR('MXUN')
#define JIT_ERR_SUPPRESS_OUTPUT		FOUR_CHAR('SPRS')
#define JIT_ERR_DATA_UNAVAILABLE	FOUR_CHAR('DUVL')
#define JIT_ERR_HW_UNAVAILABLE		FOUR_CHAR('HUVL')

#define JIT_EX_DATA_DECL extern

//stringulation-station
JIT_EX_DATA_DECL t_symbol *_jit_sym_nothing;
JIT_EX_DATA_DECL t_symbol *_jit_sym_new;
JIT_EX_DATA_DECL t_symbol *_jit_sym_free;
JIT_EX_DATA_DECL t_symbol *_jit_sym_classname;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getname;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getmethod;
JIT_EX_DATA_DECL t_symbol *_jit_sym_get;
JIT_EX_DATA_DECL t_symbol *_jit_sym_set;
JIT_EX_DATA_DECL t_symbol *_jit_sym_register;
JIT_EX_DATA_DECL t_symbol *_jit_sym_char;
JIT_EX_DATA_DECL t_symbol *_jit_sym_long;
JIT_EX_DATA_DECL t_symbol *_jit_sym_float32;
JIT_EX_DATA_DECL t_symbol *_jit_sym_float64;
JIT_EX_DATA_DECL t_symbol *_jit_sym_symbol;
JIT_EX_DATA_DECL t_symbol *_jit_sym_pointer;
JIT_EX_DATA_DECL t_symbol *_jit_sym_object;
JIT_EX_DATA_DECL t_symbol *_jit_sym_atom;
JIT_EX_DATA_DECL t_symbol *_jit_sym_list;
JIT_EX_DATA_DECL t_symbol *_jit_sym_type;
JIT_EX_DATA_DECL t_symbol *_jit_sym_dim;
JIT_EX_DATA_DECL t_symbol *_jit_sym_planecount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_val;
JIT_EX_DATA_DECL t_symbol *_jit_sym_plane;
JIT_EX_DATA_DECL t_symbol *_jit_sym_cell;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_matrix;
JIT_EX_DATA_DECL t_symbol *_jit_sym_class_jit_matrix;
JIT_EX_DATA_DECL t_symbol *_jit_sym_togworld;
JIT_EX_DATA_DECL t_symbol *_jit_sym_fromgworld;
JIT_EX_DATA_DECL t_symbol *_jit_sym_frommatrix;
JIT_EX_DATA_DECL t_symbol *_jit_sym_class_jit_attribute;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_attribute;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_attr_offset;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_attr_offset_array;
JIT_EX_DATA_DECL t_symbol *_jit_sym_rebuilding;
JIT_EX_DATA_DECL t_symbol *_jit_sym_modified;
JIT_EX_DATA_DECL t_symbol *_jit_sym_lock;
JIT_EX_DATA_DECL t_symbol *_jit_sym_setinfo;
JIT_EX_DATA_DECL t_symbol *_jit_sym_setinfo_ex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getinfo;
JIT_EX_DATA_DECL t_symbol *_jit_sym_data;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getdata;
JIT_EX_DATA_DECL t_symbol *_jit_sym_outputmatrix;
JIT_EX_DATA_DECL t_symbol *_jit_sym_clear;
JIT_EX_DATA_DECL t_symbol *_jit_sym_clear_custom;
JIT_EX_DATA_DECL t_symbol *_jit_sym_err_calculate;
JIT_EX_DATA_DECL t_symbol *_jit_sym_max_jit_classex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_setall;
JIT_EX_DATA_DECL t_symbol *_jit_sym_chuck;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getsize;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getindex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_objptr2index;
JIT_EX_DATA_DECL t_symbol *_jit_sym_append;
JIT_EX_DATA_DECL t_symbol *_jit_sym_insertindex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_deleteindex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_chuckindex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_makearray;
JIT_EX_DATA_DECL t_symbol *_jit_sym_reverse;
JIT_EX_DATA_DECL t_symbol *_jit_sym_rotate;
JIT_EX_DATA_DECL t_symbol *_jit_sym_shuffle;
JIT_EX_DATA_DECL t_symbol *_jit_sym_swap;
JIT_EX_DATA_DECL t_symbol *_jit_sym_findfirst;
JIT_EX_DATA_DECL t_symbol *_jit_sym_findall;
JIT_EX_DATA_DECL t_symbol *_jit_sym_methodall;
JIT_EX_DATA_DECL t_symbol *_jit_sym_methodindex;
JIT_EX_DATA_DECL t_symbol *_jit_sym_sort;
JIT_EX_DATA_DECL t_symbol *_jit_sym_matrix_calc;
JIT_EX_DATA_DECL t_symbol *_jit_sym_genframe;
JIT_EX_DATA_DECL t_symbol *_jit_sym_filter;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_mop;
JIT_EX_DATA_DECL t_symbol *_jit_sym_newcopy;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_linklist;
JIT_EX_DATA_DECL t_symbol *_jit_sym_inputcount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_outputcount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getinput;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getoutput;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getinputlist;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getoutputlist;
JIT_EX_DATA_DECL t_symbol *_jit_sym_ioname;
JIT_EX_DATA_DECL t_symbol *_jit_sym_matrixname;
JIT_EX_DATA_DECL t_symbol *_jit_sym_outputmode;
JIT_EX_DATA_DECL t_symbol *_jit_sym_matrix;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getmatrix;
JIT_EX_DATA_DECL t_symbol *_jit_sym_typelink;
JIT_EX_DATA_DECL t_symbol *_jit_sym_dimlink;
JIT_EX_DATA_DECL t_symbol *_jit_sym_planelink;
JIT_EX_DATA_DECL t_symbol *_jit_sym_restrict_type;
JIT_EX_DATA_DECL t_symbol *_jit_sym_restrict_planecount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_restrict_dim;
JIT_EX_DATA_DECL t_symbol *_jit_sym_special;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getspecial;
JIT_EX_DATA_DECL t_symbol *_jit_sym_adapt;
JIT_EX_DATA_DECL t_symbol *_jit_sym_decorator;
JIT_EX_DATA_DECL t_symbol *_jit_sym_frommatrix_trunc;
JIT_EX_DATA_DECL t_symbol *_jit_sym_ioproc;
JIT_EX_DATA_DECL t_symbol *_jit_sym_getioproc;
JIT_EX_DATA_DECL t_symbol *_jit_sym_name;
JIT_EX_DATA_DECL t_symbol *_jit_sym_types;
JIT_EX_DATA_DECL t_symbol *_jit_sym_minplanecount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_maxplanecount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_mindimcount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_maxdimcount;
JIT_EX_DATA_DECL t_symbol *_jit_sym_mindim;
JIT_EX_DATA_DECL t_symbol *_jit_sym_maxdim;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_points;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_point_sprite;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_lines;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_line_strip;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_line_loop;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_triangles;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_tri_strip;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_tri_fan;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_quads;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_quad_strip;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_polygon;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_tri_grid;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gl_quad_grid;
JIT_EX_DATA_DECL t_symbol *_jit_sym_err_lockout_stack;

JIT_EX_DATA_DECL t_symbol *_jit_sym_class_jit_namespace;
JIT_EX_DATA_DECL t_symbol *_jit_sym_jit_namespace;
JIT_EX_DATA_DECL t_symbol *_jit_sym_findsize;
JIT_EX_DATA_DECL t_symbol *_jit_sym_attach;
JIT_EX_DATA_DECL t_symbol *_jit_sym_detach;
JIT_EX_DATA_DECL t_symbol *_jit_sym_add;
JIT_EX_DATA_DECL t_symbol *_jit_sym_replace;
JIT_EX_DATA_DECL t_symbol *_jit_sym_gettype;
JIT_EX_DATA_DECL t_symbol *_jit_sym_ob_sym;
JIT_EX_DATA_DECL t_symbol *_jit_sym_resolve_name;
JIT_EX_DATA_DECL t_symbol *_jit_sym_resolve_raw;
JIT_EX_DATA_DECL t_symbol *_jit_sym_notifyall;
JIT_EX_DATA_DECL t_symbol *_jit_sym_block;
JIT_EX_DATA_DECL t_symbol *_jit_sym_unblock;

#ifndef TRUE
#define TRUE 	1
#endif
#ifndef FALSE
#define FALSE 	0
#endif

#define JIT_ATTR_GET_OPAQUE			0x00000001	///< private getter (all)
#define JIT_ATTR_SET_OPAQUE			0x00000002	///< private setter (all)
#define JIT_ATTR_GET_OPAQUE_USER	0x00000100	///< private getter (user)
#define JIT_ATTR_SET_OPAQUE_USER	0x00000200	///< private setter (user)
#define JIT_ATTR_GET_DEFER			0x00010000	///< defer getter (deprecated)
#define JIT_ATTR_GET_USURP			0x00020000	///< usurp getter (deprecated)
#define JIT_ATTR_GET_DEFER_LOW		0x00040000	///< defer getter 
#define JIT_ATTR_GET_USURP_LOW		0x00080000	///< usurp getter 
#define JIT_ATTR_SET_DEFER			0x01000000	///< defer setter (deprecated)
#define JIT_ATTR_SET_USURP			0x02000000	///< usurp setter (deprecated)
#define JIT_ATTR_SET_DEFER_LOW		0x04000000	///< defer setter  
#define JIT_ATTR_SET_USURP_LOW		0x08000000	///< usurp setter

// t_jit_matrix_info flags
#define JIT_MATRIX_DATA_HANDLE		0x00000002	///< data is handle
#define JIT_MATRIX_DATA_REFERENCE	0x00000004 	///< data is reference to outside memory
#define JIT_MATRIX_DATA_PACK_TIGHT	0x00000008 	///< data is tightly packed (doesn't use standard 16 byte alignment)
#define JIT_MATRIX_DATA_FLAGS_USE	0x00008000 	/**< necessary if using handle/reference data flags when creating
												 * jit_matrix, however, it is never stored in matrix */

#define JIT_MATRIX_MAX_DIMCOUNT		32 			///< maximum dimension count
#define JIT_MATRIX_MAX_PLANECOUNT	32 			///< maximum plane count

// t_matrix_conv_info flags
#define JIT_MATRIX_CONVERT_CLAMP	0x00000001  ///< not currently used
#define JIT_MATRIX_CONVERT_INTERP	0x00000002	///< use interpolation 
#define JIT_MATRIX_CONVERT_SRCDIM	0x00000004	///< use source dimensions
#define JIT_MATRIX_CONVERT_DSTDIM	0x00000008	///< use destination dimensions

typedef unsigned long 	ulong;
typedef unsigned int 	uint;
typedef unsigned short 	ushort;
typedef unsigned char 	uchar;
	

/**
 * Matrix information struct. 
 * Used to get/set multiple matrix attributes at once.
 *
 * @ingroup matrixfun
 */
typedef struct _jit_matrix_info
{
	long			size;			///< in bytes (0xFFFFFFFF=UNKNOWN)
	t_symbol		*type;			///< primitifve type (char, long, float32, or float64)
	long			flags;			///< flags to specify data reference, handle, or tightly packed
	long			dimcount;		///< number of dimensions
	long			dim[JIT_MATRIX_MAX_DIMCOUNT];		///< dimension sizes
	long			dimstride[JIT_MATRIX_MAX_DIMCOUNT]; ///< stride across dimensions in bytes
	long			planecount;		///< number of planes
} t_jit_matrix_info;


/**
 * Matrix conversion struct. 
 * Used to copy data from one matrix to another with special characteristics.
 *
 * @ingroup matrixfun
 */
typedef struct _matrix_conv_info
{
	long 	flags;									///< flags for whether or not to use interpolation, or source/destination dimensions
	long	planemap[JIT_MATRIX_MAX_PLANECOUNT];	///< plane mapping
	long	srcdimstart[JIT_MATRIX_MAX_DIMCOUNT];	///< source dimension start	
	long	srcdimend[JIT_MATRIX_MAX_DIMCOUNT];		///< source dimension end
	long	dstdimstart[JIT_MATRIX_MAX_DIMCOUNT];	///< destination dimension start	
	long	dstdimend[JIT_MATRIX_MAX_DIMCOUNT];		///< destination dimension end
} t_matrix_conv_info;


#define JIT_PARALLEL_NDIM_MAX_IO				32
#define JIT_PARALLEL_NDIM_FLAGS_FULL_MATRIX		0x00000001

typedef struct _jit_parallel_ndim_io
{
	long				flags;
	t_jit_matrix_info 	*minfo;
	char		 		*bp;
} t_jit_parallel_ndim_io;

typedef struct _jit_parallel_ndim
{
	long flags;
	void *data;
	long dimcount;
	long *dim;
	long planecount;
	long iocount;
	t_jit_parallel_ndim_io	io[JIT_PARALLEL_NDIM_MAX_IO];
	method fn;
} t_jit_parallel_ndim;

typedef struct _jit_parallel_ndim_worker
{
	t_jit_parallel_ndim 	*paralleldata;
	long					workercount;
	long					workerid;
	long					offset[2];	
	long					extent[2];	
} t_jit_parallel_ndim_worker;

void jit_parallel_ndim_calc(t_jit_parallel_ndim *p);




t_symbol *jit_class_nameget(void *c);
void *jit_class_findbyname(t_symbol *classname);

void *jit_object_alloc(void *c);
void *jit_object_new(t_symbol *classname, ...);
t_jit_err jit_object_free(void *x);
void *jit_object_method(void *x, t_symbol *s, ...);
void *jit_object_method_typed(void *x, t_symbol *s, long ac, t_atom *av, t_atom *rv);
t_symbol *jit_object_classname(void *x);
void * jit_object_register(void *x, t_symbol *s);
void *jit_object_findregistered(t_symbol *s);
t_symbol *jit_object_findregisteredbyptr(void *x);
t_jit_err jit_object_unregister(void *x);
void *jit_object_attach(t_symbol *s, void *x);
t_jit_err jit_object_detach(t_symbol *s, void *x);
t_jit_err jit_object_notify(void *x, t_symbol *s, void *data);
void *jit_object_class(void *x);
long jit_object_attr_usercanget(void *x,t_symbol *s);
long jit_object_attr_usercanset(void *x,t_symbol *s);
void *jit_object_attr_get(void *x, t_symbol *attrname);
t_jit_err jit_object_importattrs(void *x, t_symbol *s, long argc, t_atom *argv);
t_jit_err jit_object_exportattrs(void *x, t_symbol *s, long argc, t_atom *argv);
t_jit_err jit_object_exportsummary(void *x, t_symbol *s, long argc, t_atom *argv);
t_symbol *jit_object_method_argsafe_get(void *x, t_symbol *s);

//memory functions
void *jit_getbytes(long size);
void jit_freebytes(void *ptr,long size);
void **jit_handle_new(long size);
void jit_handle_free(void **handle);
long jit_handle_size_get(void **handle);
t_jit_err jit_handle_size_set(void **handle, long size);
long jit_handle_lock(void **handle, long lock);
void jit_copy_bytes(void *dest, const void *src, long bytes);
long jit_freemem(void);
char *jit_newptr(long size);
void jit_disposeptr(char *ptr);


//atom functions
t_jit_err jit_atom_setlong(t_atom *a, long b);
t_jit_err jit_atom_setfloat(t_atom *a, double b);
t_jit_err jit_atom_setsym(t_atom *a, t_symbol *b);				
t_jit_err jit_atom_setobj(t_atom *a, void *b);
long jit_atom_getlong(t_atom *a);
double jit_atom_getfloat(t_atom *a);
t_symbol *jit_atom_getsym(t_atom *a);
void *jit_atom_getobj(t_atom *a);
long jit_atom_getcharfix(t_atom *a);
//the following are useful for setting the values _only_ if there is an arg
//rather than setting it to 0 or _jit_sym_nothing
long jit_atom_arg_getlong(long *c, long idx, long ac, t_atom *av);
long jit_atom_arg_getfloat(float *c, long idx, long ac, t_atom *av);
long jit_atom_arg_getdouble(double *c, long idx, long ac, t_atom *av);
long jit_atom_arg_getsym(t_symbol **c, long idx, long ac, t_atom *av);

//matrix info utils
t_jit_err jit_matrix_info_default(t_jit_matrix_info *info);
long jit_matrix_info_typesize(t_jit_matrix_info *minfo); 

//mop utils
t_jit_err jit_mop_single_type(void *x, t_symbol *s);
t_jit_err jit_mop_single_planecount(void *x, long c);
t_jit_err jit_mop_methodall(void *x, t_symbol *s, ...);
t_jit_err jit_mop_input_nolink(void *mop, long c);
t_jit_err jit_mop_output_nolink(void *mop, long c);
t_jit_err jit_mop_ioproc_copy_adapt(void *mop, void *mop_io, void *matrix);
t_jit_err jit_mop_ioproc_copy_trunc(void *mop, void *mop_io, void *matrix);
t_jit_err jit_mop_ioproc_copy_trunc_zero(void *mop, void *mop_io, void *matrix);
t_symbol *jit_mop_ioproc_tosym(void *ioproc);

//attr functions
long max_jit_attr_args_offset(short ac, t_atom *av);
void max_jit_attr_args(void *x, short ac, t_atom *av);
//for easy access of simple attributes
long jit_attr_getlong(void *x, t_symbol *s);
t_jit_err jit_attr_setlong(void *x, t_symbol *s, long c);
float jit_attr_getfloat(void *x, t_symbol *s);
t_jit_err jit_attr_setfloat(void *x, t_symbol *s, float c);
t_symbol *jit_attr_getsym(void *x, t_symbol *s);
t_jit_err jit_attr_setsym(void *x, t_symbol *s, t_symbol *c);
long jit_attr_getlong_array(void *x, t_symbol *s, long max, long *vals);
t_jit_err jit_attr_setlong_array(void *x, t_symbol *s, long count, long *vals);
long jit_attr_getchar_array(void *x, t_symbol *s, long max, uchar *vals);
t_jit_err jit_attr_setchar_array(void *x, t_symbol *s, long count, uchar *vals);
long jit_attr_getfloat_array(void *x, t_symbol *s, long max, float *vals);
t_jit_err jit_attr_setfloat_array(void *x, t_symbol *s, long count, float *vals);
long jit_attr_getdouble_array(void *x, t_symbol *s, long max, double *vals);
t_jit_err jit_attr_setdouble_array(void *x, t_symbol *s, long count, double *vals);
long jit_attr_getsym_array(void *x, t_symbol *s, long max, t_symbol **vals);
t_jit_err jit_attr_setsym_array(void *x, t_symbol *s, long count, t_symbol **vals);

//attr filters util
t_jit_err jit_attr_addfilterset_clip(void *x, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterset_clip_scale(void *x, double scale, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterget_clip(void *x, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterget_clip_scale(void *x, double scale, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilter_clip(void *x, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilter_clip_scale(void *x, double scale, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterset_proc(void *x, method proc);
t_jit_err jit_attr_addfilterget_proc(void *x, method proc);

//more util functions
void jit_rand_setseed(long n);
long jit_rand(void);
t_symbol *jit_symbol_unique();
void jit_error_code(void *x,t_jit_err v); //interrupt safe
void jit_error_sym(void *x,t_symbol *s); //interrupt safe
void jit_post_sym(void *x,t_symbol *s);  //interrupt safe


//util macros
#define CLIP(x,a,b) (x)=(x)<(a)?(a):(x)>(b)?(b):(x)		
#define CLAMP(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define ABS(x) ((x)<0?-(x):(x))
#define	SET_FLAG(word, flag)	((word) |= (flag))
#define	CLEAR_FLAG(word, flag)	((word) &= ~(flag))
#define GET_FLAG(word, flag)	((word) & (flag))


// from jit.fixmath.h
#ifdef fixed1
#undef fixed1
#endif
#ifdef IntToFixed                    
#undef IntToFixed                    
#endif
#ifdef FloatToFixed                
#undef FloatToFixed                
#endif
#ifdef FixedToInt 
#undef FixedToInt 
#endif
#ifdef FixedToFloat        
#undef FixedToFloat        
#endif

typedef unsigned int UINT;
typedef UINT UFixed;
typedef int Fixed;
typedef Fixed t_jit_Fixed;
typedef Fixed t_jit_fixed;

#define fract1			(1<<30L) 
#define FloatToFract(x) ((Fixed) ((x) * (float)fract1))
#define FractToFloat(x) ((float) (x) * 1./(float)fract1)
#define FractToFixed(x) (x>>14L)


#define fixed2		(1<<17L)
#define fixed1		(1<<16L)
#define fixedhalf	(1<<15L)
#define IntToFixed(x) ((Fixed)((x)<<16L)) 
#define FloatToFixed(x) ((Fixed)((x) * (float)fixed1))
#define DoubleToFixed(x) ((Fixed)((x) * (double)fixed1))                  
#define FixedToInt(x) ((x)>>16L)
#define FixedToFloat(x) ((float) (x) * 1./(float)fixed1)
#define FixedToDouble(x) ((double)(x) * 1./(double)fixed1)


static t_jit_fixed charmapping[] = {
0, 257, 514, 771, 1028, 1285, 1542, 1799, 2056, 2313, 2570, 2827, 3084, 3341, 3598, 3855, 
4112, 4369, 4626, 4883, 5140, 5397, 5654, 5911, 6168, 6425, 6682, 6939, 7196, 7453, 7710, 7967, 
8224, 8481, 8738, 8995, 9252, 9509, 9766, 10023, 10280, 10537, 10794, 11051, 11308, 11565, 11822, 12079, 
12336, 12593, 12850, 13107, 13364, 13621, 13878, 14135, 14392, 14649, 14906, 15163, 15420, 15677, 15934, 16191, 
16448, 16705, 16962, 17219, 17476, 17733, 17990, 18247, 18504, 18761, 19018, 19275, 19532, 19789, 20046, 20303, 
20560, 20817, 21074, 21331, 21588, 21845, 22102, 22359, 22616, 22873, 23130, 23387, 23644, 23901, 24158, 24415, 
24672, 24929, 25186, 25443, 25700, 25957, 26214, 26471, 26728, 26985, 27242, 27499, 27756, 28013, 28270, 28527, 
28784, 29041, 29298, 29555, 29812, 30069, 30326, 30583, 30840, 31097, 31354, 31611, 31868, 32125, 32382, 32639, 
32897, 33154, 33411, 33668, 33925, 34182, 34439, 34696, 34953, 35210, 35467, 35724, 35981, 36238, 36495, 36752, 
37009, 37266, 37523, 37780, 38037, 38294, 38551, 38808, 39065, 39322, 39579, 39836, 40093, 40350, 40607, 40864, 
41121, 41378, 41635, 41892, 42149, 42406, 42663, 42920, 43177, 43434, 43691, 43948, 44205, 44462, 44719, 44976, 
45233, 45490, 45747, 46004, 46261, 46518, 46775, 47032, 47289, 47546, 47803, 48060, 48317, 48574, 48831, 49088, 
49345, 49602, 49859, 50116, 50373, 50630, 50887, 51144, 51401, 51658, 51915, 52172, 52429, 52686, 52943, 53200, 
53457, 53714, 53971, 54228, 54485, 54742, 54999, 55256, 55513, 55770, 56027, 56284, 56541, 56798, 57055, 57312, 
57569, 57826, 58083, 58340, 58597, 58854, 59111, 59368, 59625, 59882, 60139, 60396, 60653, 60910, 61167, 61424, 
61681, 61938, 62195, 62452, 62709, 62966, 63223, 63480, 63737, 63994, 64251, 64508, 64765, 65022, 65279, 65536
};

#define fixedhalf_255 FloatToFixed(0.5/255)
#define fixed1_255 FloatToFixed(1./255)
//#define UCharToFixed(x) (((Fixed)(x))<<8L)
//#define FixedToUChar(x) ((unsigned char)((x)>>8L))
//#define UCharToFixed(x) (((Fixed)(x))*fixed1_255)
#define UCharToFixed(x) (charmapping[x])
#define FixedToUChar(x) ((unsigned char)((x+fixedhalf_255)/fixed1_255))

#define LongToCharClamped(v) (t_jit_char(CLAMP(v, 0, 255)))


//#define fixed1_255 UCharToFixed(1)
//#define fixedChar255 (fixed1-fixed1_255)
#define fixedChar255 (fixed1)
#define FixedToUCharClamped(x) FixedToUChar(CLAMP(x, 0, fixedChar255))

#define FixMul(x,y) (((y)>>8L) * ((x)>>8L))   //both are large(>1)         	
#define FixMul2(x,y) (((y) * ((x)>>8L))>>8L)  //y is small          
#define FixMul3(x,y) ((((y)>>8L) * (x))>>8L)  //x is small
#define FixMul4(x,y) (((y) * (x))>>16L)       //both are small          

#define FixDiv(x,y) ((((y)>>8L)!=0)?((((x)<<4L) / ((y)>>8L))<<4L):0)
#define FixedFraction(x) ((x)&0xffffL)        //unsigned fraction    
#define FixedFractionSigned(x) ((x)&0x8000ffffL)         

#define FixedEps ((long) 1)
#define FixedOneMinusEps (fixed1 - FixedEps)
#define FixedFrac(x) ((x) & FixedOneMinusEps)
#define FixedFloor(x) ((x) & ~FixedOneMinusEps)
#define FixedCeil(x) FixedFloor((x) + FixedOneMinusEps)
#define FixedMod1(x) ((x) & (FixedOneMinusEps))
#define FixedRange1(x) ((x)==fixed1 ? fixed1 : FixedMod1(x))
#define FixedMod2(x) ((x) & (fixed1 | FixedOneMinusEps))
#define FixedRound(x) ((FixedFrac(x) < fixedhalf) ? FixedFloor(x) : FixedCeil(x))
//#define FixedWrap1(x) ((FixedMod2(x) < FixedOneMinusEps) ? FixedMod2(x) : ((fixed1 | FixedOneMinusEps) - FixedMod2(x)))

#define FixedWrap1(x) ((x) <= fixed1 ? FixedMod2(x) : ((fixed1 | FixedOneMinusEps) - FixedMod2(x)))
//#define FixedWrap1(x) ((x) <= fixed1 ? FixedMod2(x) : (FixedMod2(x)))
// return 1.-abs((mod(v, 2.)-1.));
#define FixedWrap1(x) (fixed1-ABS(FixedMod2(x)-fixed1))


//#define FixedClamp1(x)	((x < 0) ? 0 : ((x > FixedOneMinusEps) ? FixedOneMinusEps : x))
#define FixedClamp1(x)	((x < 0) ? 0 : ((x > fixed1) ? fixed1 : x))

#define FloatToUFixed(x) ((UFixed)((x) * (float)fixed1))
#define UFixedToFloat(x) ((float) (x) * 1./(float)fixed1)
#define UFixedToUInt(x) ((x)>>16L)

#define fixedLogic1(x) (((x) == 1) ? (fixedChar255) : 0)


// from jit.matrix.util.h
t_jit_err jit_matrix_list_get_matrices(void *list, long n, void **matrices);
t_jit_err jit_matrix_array_lock(void **matrices, long n, long *savelock);
t_jit_err jit_matrix_array_unlock(void **matrices, long n, long *savelock);
t_jit_err jit_matrix_array_get_matrix_info(void **matrices, long n, t_jit_matrix_info *matrix_info);
t_jit_err jit_matrix_array_get_data(void **matrices, long n, char **data);
t_jit_err jit_matrix_info_uniform_planecount(t_jit_matrix_info *matrix_info, long n, t_jit_matrix_info *info_list);
t_jit_err jit_matrix_info_uniform_type(t_jit_matrix_info *matrix_info, long n, t_jit_matrix_info *info_list);
t_jit_err jit_matrix_info_uniform_dim(t_jit_matrix_info *matrix_info, long n, t_jit_matrix_info *info_list);



	extern double jit_math_wrap(double x, double lo, double hi);
	extern double jit_math_fold(double x, double lo, double hi);
	extern double jit_math_fmod(double x, double y);
	extern double jit_math_trunc(double x);
	extern long jit_rand(void);

	// for debugging:
	extern int printf(const char *, ...);
	
#define jit_math_fmod1(x)	jit_math_fmod(x, 1.)
#define jit_math_wrap1(x)	jit_math_wrap(x, 0., 1.)
#define jit_math_fold1(x)	jit_math_fold(x, 0., 1.)
#define CLAMP1(x)			(CLAMP(x, 0, 1))

#ifdef WIN_VERSION
#pragma pack()
#endif

#ifdef __cplusplus
}
#endif


#endif // __JIT_COMMON_H__
