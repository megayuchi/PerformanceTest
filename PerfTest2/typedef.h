#pragma once

#include <windows.h>
#include <stdio.h>
#include <Ole2.h>
#include "math.inl"


//#define	_INL_MATH_FUNC


#define ENGINE_DEFAULT_FONT_NAME_A "Tahoma"
#define ENGINE_DEFAULT_FONT_NAME_W L"Tahoma"

#define VECTOR3_SIZE	12
#define VECTOR4_SIZE	16

#define MIN_UNIT							10.0f
#define	ONE_CM								1.0f
#define MAX_NAME_BUFFER_LEN					128
#define MAX_NAME_LEN_						127

//#define	MAX_FILENAME_LEN				32

#define DEFAULT_LIGHT_RADIUS				100.0f
//#define DEFAULT_RENDER_ZORDER_UNIT			-1000.0f
#define DEFAULT_RENDER_ZORDER_UNIT			-1000.0f

#define DEFAULT_RENDER_ZORDER_UNIT_ATTACH	-100.0f

#define WORLDMAP_DEFAULT_TOP			400.0f
#define WORLDMAP_DEFAULT_BOTTOM			0.0f

#define FOG_DISTANCE_START				2000.0f
#define FOG_DISTANCE_END				8000.0f

// EXECUTIVE CONST VALUE
#define	DEFAULT_DECAL_TRI_NUM			64

#define	MAX_MODEL_NUM_PER_GXOBJECT		8
#define MAX_RENDER_OBJECT_NUM			8192
#define MAX_ATTATCH_OBJECTS_NUM			16
#define	MAX_PICK_OBJECT_NUM				256
#define ZORDER_LATEST_RENDER			1100
#define	MAX_SPAWN_POINT_NUM_PER_SCENE		16
#define SPAWN_POINT_NAME_BUFFER_LEN			16



// GEOMETRY CONST VALUE
#define PID_MODEL_LOD					"_LOD"
#define PID_MODEL_lod					"_lod"
#define PID_LEN							4

#define	MAX_PICK_DYNAMIC_RENDER_OBJECT_NUM		1024
#define DEFAULT_MODEL_LOD_UNIT			500.0
#define MAX_LOD_MODEL_NUM				3
#define MAX_HFINDEX_BUFER_NUM			128
#define MAX_OBJECTS_NUM_PER_MODEL		8192
#define	MAX_NOT_RENDER_OBJ_NAME_NUM		128
#define MAX_NOT_RENDER_OBJ_NAME_BUFFER_LEN		32
#define MAX_NOT_RENDER_OBJ_NAME_LEN				31
#define	MAX_SHADOW_SENDER_POINT_NUM		8192
#define LIMITED_TICK_RECYCLE_BIN_FULL	1000*60		// 휴지통이 꽉 찼을 경우 우선적으로 삭제할 오브젝트들의 경과시간 제한(1분지난 놈들 찾아서 삭제)
#define DEFAULT_RECYCLE_BIN_PERCENTAGE	0.5f		// 휴지통 크기 비율. 최대 오브젝트 개수 * 비율

#define MIN_QUAD_NODE_LENGTH				1000.0f		


#define DEFAULT_MAX_POLYGON_NUM_FOR_EDIT	65536*4
#define DEFAULT_MAX_VERTEX_NUM_FOR_EDIT		65536*4
#define DEFAULT_MAX_MEMORY_SIZE_FOR_EDIT	1024*1024*64

#define	DEFAULT_ZERO_VALUE_POLYGON_TOOL		0.05f
#define MAX_FIND_THREAD_NUM		64
//#define MAX_REALTIME_LIGHT_NUM			(MAX_STATIC_RTLIGHT_NUM_PER_SCENE + MAX_DYNAMIC_RTLIGHT_NUM_PER_SCENE)
#define	MAX_EXT_BOUNDING_SPHERE_NUM			32


#define MAX_BSP_NODE_NUM_ON_FIND		4096
#define DEFAULT_COLLISION_MESH_TREE_STACK_SIZE 256
#define MAX_CAMERA_NUM_PER_SCENE		128
#define MAX_MOTION_NUM					1024
#define	DEFAULT_AMBIENT_COLOR			0xff202020
#define	DEFAULT_EMISSIVE_COLOR			0xffffffff
#define MAX_RENDER_STATICMODEL_OBJ_NUM	4096
#define MAX_RENDER_HFIELD_OBJ_NUM		1024
//#define	DEFAULT_NEAR					100.0f
#define DEFAULT_FAR						80000.0f
//#define	DEFAULT_FOV					PI/4.0f
#define MAX_FILEITEM_NUM				8192
#define MAX_FILEBUCKET_NUM				256
#define MAX_VIEWPORT_NUM				256
#define MAX_MATRIX_NUM_IN_POOL			4096
#define MAX_LIGHT_INDEX_NUM_IN_POOL		4096
#define MAX_MODEL_REF_INDEX_NUM			(8192*4)
#define DEFAULT_RESOURCE_SCHDULE_DISTANCE	16.0f
#define	DEFAULT_OVERLAP_TILES_NUM		4
#define MAX_LAYER_NAME_LEN				16
#define MAX_RENDER_OBJECT_PER_TYPE				8192
#define MAX_OBJECT_NUM_IN_SPACE_PARTITION		65536

#define DEFAULT_SUB_COLMESH_WIDTH		1200.0f
#define DEFAULT_SUB_COLMESH_HEIGHT		1200.0f
#define MAX_EXTERIOR_ENTER_COUNT		64
#define MAX_VOXEL_OBJECT_NUM			65536

// RENDERER CONST VALUE
#define	MAX_RESOURCE_NUM				(8192*4)
#define	DEFAULT_VBCACHE_VERTEX_BUFFER_NUM		8
#define	DEFAULT_IBCACHE_INDEX_BUFFER_NUM		1
#define DEFAULT_VBCACHE_VERTEXNUM_PER_BUFFER	32768
#define	DEFAULT_IBCACHE_INDEXNUM_PER_BUFFER		32768

#define	DEFAULT_ALPHA_REF_VALUE			127
#define DEFAULT_LMMESH_ALPHA_REF_VALUE	250
#define DEFAULT_RENDER_TEXTURE_NUM		4
#define MAX_RENDER_TEXTURE_NUM			8
#define DEFAULT_RENDER_TEXTURE_SIZE		256
#define MAX_RENDER_TEXTURE_SIZE			1024
#define MAX_SPRITE_ZORDER_NUM			256
#define MAX_HFIELD_DETAIL_NUM			8
#define MAX_TILE_TEXTURE_NUM			65536				
#define MAX_SPRITE_FRAME_NUM			1024
#define MAX_RENDER_TARGET_NUM			8
#define	MAX_TEXTURE_NUM					25600
#define MAX_TEXBUCKET_NUM				256
#define WATER_FADE_DISTANCE				15.0f
#define DEFAULT_WATER_DEPTH				100.0f

const	DWORD MAX_RENDER_MESHOBJ_NUM = 8192;
const	DWORD MAX_RENDER_IMM_MESHOBJ_NUM = 8192;
const	DWORD MAX_RENDER_SHADOW_CASTER_NUM = 4096;
const	DWORD MAX_LATER_RENDER_MESHOBJ_NUM = 128;
const	DWORD DEFAULT_VOXEL_CB_NUM = 8192;

#define MAX_RENDER_TEXTBUFFER_SIZE		16384*4
#define MAX_LINES_BUFFER_SIZE			8192
#define MAX_TRIS_BUFFER_SIZE			8192
#define MAX_RENDER_TRIBUFFER_SIZE		8192
#define MAX_RENDER_SPRITE_NUM			1024
#define MAX_FONT_INFO_NUM				128	// 각각 다른 종류의 폰트 가지수
#define DEFULAT_CIRCLE_PIECES_NUM		32
#define MAX_CIRCLE_PIECES_NUM			64
#define MAX_MIPMAP_LEVEL_NUM			12
#define MAX_MIPMAP_SIZE					1
#define MAX_MATERIAL_NUM				8192
#define MAX_MATERIAL_SET_NUM			2048
#define	MAX_D3DRESOURCE_NUM				65536
#define	MAX_LAYER_NUM					65536
#define	DEFAULT_D3DRESOURCE_NUM			128
#define DEFAULT_VBCACHE_NUM						1024
#define DEFAULT_PHYSIQUE_OBJECT_NUM_PER_SCENE	32
#define DEFAULT_MOST_PHYSIQUE_VERTEX_NUM		800
#define MAX_PHYSIQUE_VERTEX_NUM					32768
#define MAX_PRJMESH_INDICES_NUM					16384
#define MAXSHINESTR_TO_SS3D_VAL					20.0f
#define DEFAULT_FREE_VBCACHE_RATE				0.25f
#define MAX_EFFECT_SHADER_NUM					1024	
#define DEFAULT_LIMITED_VERTEXTBUFFER_INDICES	65536
#define MAX_RESOURCE_POOL_NUM					32
#define MAX_MASK_TEX_NUM						2
#define MAX_POST_EFFECT_NUM						8
#define	DEFAULT_HW_OCCLUSION_TEST_NEAR_OFFSET	50.0f
#define	MAX_DYNAMIC_LIGHT_NUM_PER_SCENE			128
#define DEFAULT_GLOW_POW_CONSTANT				16.0f
//#define MAX_TRI_NUM_PER_COLLISION_CHECK			1024
//#define MAX_COLLISION_CHECK_NUM_PER_SAME_TIME	1024

#define MAX_ROOM_NUM_PER_SCENE					65536
#define DEFAULT_ROOM_NUM_PER_SCENE				65536

#define	MAX_OCCLUSION_NUM_PER_SCENE				1024

#define DEFAULT_SHADOW_MAP_WIDTH				1024
#define DEFAULT_SHADOW_MAP_HEIGHT				1024

#define DEFAULT_HEIGHT_MAP_WIDTH				1024
#define DEFAULT_HEIGHT_MAP_HEIGHT				1024

#define MAX_MIRROR_RENDER_TEXTURE_WIDTH_HEIGHT	1024

#define DEFAULT_POSTEFFECT_TEX_WIDTH			512
#define DEFAULT_POSTEFFECT_TEX_HEIGHT			512
#define	MAX_DYNAMIC_LIGHT_NUM_PER_OBJ			8
#define	MAX_RENDER_PARTICLE_BOX_NUM				1024
#define	MAX_RENDER_ELLIPSOID_MESH_NUM			256

#define DEFAULT_OUTLINE_THICK_CONST_VL			0.75f
#define DEFAULT_OUTLINE_THICK_CONST_LM			1.4f
#define DEFAULT_OUTLINE_THICK_CONST_HFM			16.0f

#define DEFAULT_TOON_COLOR_TONE_R	1.275f
#define DEFAULT_TOON_COLOR_TONE_G	1.15f
#define DEFAULT_TOON_COLOR_TONE_B	1.1f

#define DEFAULT_VL_COLOR_TONE_R	1.45f
#define DEFAULT_VL_COLOR_TONE_G	1.3f
#define DEFAULT_VL_COLOR_TONE_B	1.3f


enum AXIS_TYPE
{

	AXIS_TYPE_NONE = 0,
	AXIS_TYPE_X = 1,
	AXIS_TYPE_Y = 2,
	AXIS_TYPE_Z = 3
};


#define PI			3.14159265358979323846f // Pi
#define PI_MUL_2	6.28318530717958623200f // 2 * Pi
#define PI_DIV_2	1.57079632679489655800f // Pi / 2
#define PI_DIV_4	0.78539816339744827900f // Pi / 4
#define INV_PI		0.31830988618379069122f // 1 / Pi
#define EPSILON		1.0e-5f;                 // Tolerance for FLOATs

#define R_565_MASK	0x0000F800
#define G_565_MASK	0x000007E0
#define B_565_MASK	0x0000001F

union COLORVALUE
{
	struct
	{
		float	b;
		float	g;
		float	r;
		float	a;
	};
	float		value[4];
};
union COLOR3
{
	struct
	{
		float	r;
		float	g;
		float	b;

	};
	float		value[3];
	void	Set(float rr, float gg, float bb) { r = rr; g = gg; b = bb; }
};

struct TVERTEX
{
	float u;
	float v;
};
struct DOUBLE_PLANE
{
	DOUBLE_VECTOR3		v3Up;
	float				D;
};

struct AABB
{
	VECTOR3	Min;
	VECTOR3	Max;
};
struct PLANE
{
	VECTOR3		v3Up;
	float		D;
};
#define PLANE_D_OFFSET	VECTOR3_SIZE


struct PLANE_EXT : PLANE
{
	VECTOR3		v3Tri[3];
};
#pragma pack(push,4)

struct TRIANGLE
{
	VECTOR3		v3Point[3];
};
struct TEX_TRIANGLE
{
	TVERTEX				tv[3];
};

#pragma pack(pop)

#if !defined(ARM64)
__declspec(align(16)) union TRIANGLE_A
{
	VECTOR4	v4Point[3];
	__m128	Point[3];
};
#else
union TRIANGLE_A
{
	VECTOR4	v4Point[3];
	__m128	Point[3];
};
#endif


struct BOUNDING_BOX
{
	VECTOR3			v3Oct[8];
	void SetWithAABB(const VECTOR3* pv3Min, const VECTOR3* pv3Max)
	{
		float	min_x = pv3Min->x;
		float	min_y = pv3Min->y;
		float	min_z = pv3Min->z;

		float	max_x = pv3Max->x;
		float	max_y = pv3Max->y;
		float	max_z = pv3Max->z;

		v3Oct[0].x = min_x;
		v3Oct[0].y = max_y;
		v3Oct[0].z = max_z;

		v3Oct[1].x = min_x;
		v3Oct[1].y = min_y;
		v3Oct[1].z = max_z;

		v3Oct[2].x = max_x;
		v3Oct[2].y = min_y;
		v3Oct[2].z = max_z;

		v3Oct[3].x = max_x;
		v3Oct[3].y = max_y;
		v3Oct[3].z = max_z;

		v3Oct[4].x = min_x;
		v3Oct[4].y = max_y;
		v3Oct[4].z = min_z;

		v3Oct[5].x = min_x;
		v3Oct[5].y = min_y;
		v3Oct[5].z = min_z;

		v3Oct[6].x = max_x;
		v3Oct[6].y = min_y;
		v3Oct[6].z = min_z;

		v3Oct[7].x = max_x;
		v3Oct[7].y = max_y;
		v3Oct[7].z = min_z;
	}
};

struct BOUNDING_SPHERE
{
	VECTOR3			v3Point;
	float			fRs;
};

struct BOUNDING_CYLINDER
{
	VECTOR3			v3Point;
	float			fRs;
	float			fAy;
};

struct BOUNDING_CAPSULE
{
	VECTOR3			v3From;
	VECTOR3			v3To;
	float			fRadius;
};

struct COLLISION_MESH_OBJECT_DESC_SAVELOAD
{
	BOUNDING_BOX		boundingBox;
	BOUNDING_SPHERE		boundingSphere;
	BOUNDING_CYLINDER	boundingCylinder;
	DWORD				dwObjIndex;
	char				szObjName[MAX_NAME_BUFFER_LEN];
};

struct COLLISION_MESH_OBJECT_DESC
{
	BOUNDING_BOX		boundingBox;
	BOUNDING_SPHERE		boundingSphere;
	BOUNDING_CYLINDER	boundingCylinder;
	DWORD				dwObjIndex;
};


struct QUADRANGLE
{
	VECTOR3	v3Point[4];
};

union MIN_MAX_AXIS
{
	struct
	{
		float		max_x;
		float		max_y;
		float		max_z;
		float		min_x;
		float		min_y;
		float		min_z;
	};
	float	fStream[6];

};


struct UINT4
{
	UINT x;
	UINT y;
	UINT z;
	UINT w;
};
union COLOR_VALUE
{
	struct
	{
		float r;
		float g;
		float b;
		float a;
	};
	float	rgba[4];
	void Set(float rr, float gg, float bb, float aa) { r = rr; g = gg; b = bb; a = aa; }
	void Set(DWORD dwColor)
	{
		r = (float)((dwColor & 0x00ff0000) >> 16) / 255.0f;		// R
		g = (float)((dwColor & 0x0000ff00) >> 8) / 255.0f;		// G
		b = (float)((dwColor & 0x000000ff)) / 255.0f;			// B;
		a = (float)((dwColor & 0xff000000) >> 24) / 255.0f;		// A
	}
};



