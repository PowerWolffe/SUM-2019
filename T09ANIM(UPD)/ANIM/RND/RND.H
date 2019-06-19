/* FILE NAME   : RND.H
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#ifndef __RND_H_
#define __RND_H_

#define GLEW_STATIC
#include <glew.h>

#include "..\..\DEF.H"

VOID DD6_RndStart(VOID);
VOID DD6_RndCopyFrame(HDC hDC);
VOID DD6_RndEnd(VOID);
VOID DD6_RndClose(VOID);
VOID DD6_RndResize(INT W, INT H);
VOID DD6_RndInit(HWND hWnd);
VOID DD6_RndProjSet(VOID);

extern DBL
  DD6_RndProjSize,
  DD6_RndProjDist,
  DD6_RndProjFarClip;
extern MATR
  DD6_RndMatrView,
  DD6_RndMatrProj,
  DD6_RndMatrVP;

/* Global shader program Id */
extern INT DD6_RndProgId;

typedef struct tagDD6VERTEX
{
  VEC P;   /* Vertex position */
  VEC2 T;  /* Vertex position */
  VEC N;   /* Vertex position */
  VEC4 C;  /* Vertex position */
} DD6VERTEX;

typedef enum tagdd6PRIM_TYPE
{
  DD6_RND_PRIM_TRIMESH, 
  DD6_RND_PRIM_GRID 
} dd6PRIM_TYPE;

typedef struct tagDD6PRIM
{
  dd6PRIM_TYPE Type;
  INT VA, VBuf, IBuf;  /* Number of vertices */
  INT NumOfElements;   /* Number of indices */
  MATR Trans;          /* Additional transformation matrix */

  INT Mtlno;

} dd6PRIM;

VOID DD6_RndInit( HWND hWnd );
VOID DD6_RndClose( VOID );

VOID DD6_RndCopyFrame( HDC hDC );

VOID DD6_RndStart( VOID );
VOID DD6_RndEnd( VOID );

VOID DD6_RndProjSet( VOID );
VOID DD6_RndCamSet( VEC Loc, VEC At, VEC Up1 );
VOID DD6_RndResize( INT W, INT H );

BOOL DD6_RndPrimLoad( dd6PRIM *Pr, CHAR *FileName );
VOID DD6_RndPrimDraw( dd6PRIM *Pr, MATR World );
VOID DD6_RndPrimFree( dd6PRIM *Pr );
BOOL DD6_RndPrimCreate( dd6PRIM *Pr, dd6PRIM_TYPE Type, DD6VERTEX *V, INT NoofV, INT *I, INT NoofI );

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT DD6_RndShdLoad( CHAR *ShaderFileNamePrefix );

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID DD6_RndShdDelete( INT ProgId );

/* Maximum resourse name size */
#define DD6_STR_MAX 300

/* Shader program store type */
typedef struct tagdd6SHADER
{
  CHAR Name[DD6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} dd6SHADER;

/* Shaders stock */
#define DD6_MAX_SHADERS 30                        /* Shaders stock size */
extern dd6SHADER DD6_RndShaders[DD6_MAX_SHADERS]; /* Array of shaders */
extern INT DD6_RndShadersSize;                    /* Shadres array store size */

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT DD6_RndShdLoad( CHAR *ShaderFileNamePrefix );

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID DD6_RndShdDelete( INT ProgId );

/* Shaders support initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndShdInit( VOID );

/* Shaders support dinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndShdClose( VOID );

/* Load shader program and add to stock function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader stock index.
 */
INT DD6_RndShdAdd( CHAR *ShaderFileNamePrefix );

/* Shaders stock update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndShdUpdate( VOID );

/***
 * Texture support functions
 ***/

/* Texture representation type */
typedef struct tagdd6TEXTURE
{
  CHAR Name[DD6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} dd6TEXTURE;

/* Textures stock */
#define DD6_MAX_TEXTURES 30                         /* Textures stock size */
extern dd6TEXTURE DD6_RndTextures[DD6_MAX_TEXTURES]; /* Array of textures */
extern INT DD6_RndTexturesSize;                     /* Textures array store size */

/* Textures support initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndTexInit( VOID );

/* Textures support deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndTexClose( VOID );

/* Add texture image to stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size in pixels:
 *       INT W, H;
 *   - number of texture image components (3 or 4):
 *       INT C;
 *   - pointer to image pixel colors:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) add texture stock index.
 */
INT DD6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

/***
 * Material support functions
 ***/

/* Material store type */
typedef struct tagdd6MATERIAL
{
  CHAR Name[DD6_STR_MAX]; /* Material name */
  INT ShdNo;              /* Shader number in shader array */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */
  FLT Trans;                /* Transparency factor */
  INT Tex[8];               /* Texture references from texture table */
} dd6MATERIAL;

/* Material stock */
#define DD6_MAX_MATERIALS 30                            /* Materials stock size */
extern dd6MATERIAL DD6_RndMaterials[DD6_MAX_MATERIALS]; /* Array of materials */
extern INT DD6_RndMaterialsSize;                        /* Materials array store size */

/* Materials support initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndMtlInit( VOID );

/* Add new material to stock function.
 * ARGUMENTS:
 *   - material description structure pointer:
 *       dd6MATERIAL *Mtl;
 * RETURNS:
 *   (INT) added material stock index.
 */
INT DD6_RndMtlAdd( dd6MATERIAL *Mtl );

/* Add new material to stock function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) applied material shader program Id.
 */
INT DD6_RndMtlApply( INT MtlNo );

/***
 * Primitive collections support
 ***/

/* Array of primitives representation type */
typedef struct tagdd6PRIMS
{
  dd6PRIM *Prims; /* Primitive array */
  INT NumOfPrims; /* Primitive array size */
  MATR Trans; /* All primitives transformation matrix */

} dd6PRIMS;

/* Primitive addon data */
extern FLT DD6_RndAddon[3];

/* Create array of primitives function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndPrimsCreate( dd6PRIMS *Prs, INT NoofP );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DD6_RndPrimsFree( dd6PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DD6_RndPrimsDraw( dd6PRIMS *Prs, MATR World );

/* Load array of primitives from G3DM file function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 *   - file name to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndPrimsLoad( dd6PRIMS *Prs, CHAR *FileName );
/* Image representation structure */
typedef struct tagdd6IMAGE
{
  /* Image size */
  INT W, H;

  /* Image handle */
  HBITMAP hBm;

  /* Image description */
  BITMAP bm;
} dd6IMAGE;

/* Load image function.
 * ARGUMENTS:
 *   - image structure:
 *       dd6IMAGE *Img;
 *   - image file name (BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndImgLoad( dd6IMAGE *Img, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image structure:
 *       dd6IMAGE *Img;
 * RETURNS: None.
 */
VOID DD6_RndImgFree( dd6IMAGE *Img );

/* Image pixel value get function.
 * ARGUMENTS:
 *   - image structure:
 *       dd6IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (VEC) mapped to 0..1 color values.
 */
VEC DD6_RndImgGet( dd6IMAGE *Img, INT X, INT Y );

/* Image to height fireld primitive creation function.
 * ARGUMENTS:
 *   - image structure:
 *       dd6IMAGE *Img;
 *   - primitive to be create:
 *       dd6PRIM *Pr;
 *   - height scale:
 *       DBL ScaleH;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndImgToHF( dd6IMAGE *Img, dd6PRIM *Pr, DBL ScaleH );

/* Load image function.
 * ARGUMENTS:
 *   - image file name (BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) add texture stock index.
 */
INT DD6_RndTexLoad( CHAR *FileName );

#endif
