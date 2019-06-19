/* FILE NAME   : RNDPRIMS.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Primitive collection support.
 * PROGRAMMER  : DD6
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */
#include <stdio.h>

#include "../anim.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndPrimsCreate( dd6PRIMS *Prs, INT NoofP )
{
  memset(Prs, 0, sizeof(dd6PRIMS));
  if ((Prs->Prims = malloc(sizeof(dd6PRIM) * NoofP)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(dd6PRIM) * NoofP);
  Prs->NumOfPrims = NoofP;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'DD6_RndPrimsCreate' function */
/* Delete array of primitives function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DD6_RndPrimsFree( dd6PRIMS *Prs )
{
  INT i;

  for (i = 0; i < Prs->NumOfPrims; i++)
   DD6_RndPrimFree(&Prs->Prims[i]);
  free(Prs->Prims);
  memset(Prs, 0, sizeof(dd6PRIMS));
} /* End of 'DD6_RndPrimsFree' function */
/* Draw array of primitives function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DD6_RndPrimsDraw( dd6PRIMS *Prs, MATR World )
{
  INT i;

  DD6_RndAddon[0] = Prs->NumOfPrims;
  World = MatrMulMatr(Prs->Trans, World);
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    DD6_RndAddon[1] = i;
    DD6_RndPrimDraw(&Prs->Prims[i], World);
  }
} /* End of 'DD6_RndPrimsDraw' function */

/* Load array of primitives from G3DM file function.
 * ARGUMENTS:
 *   - primitive collection pointer:
 *       dd6PRIMS *Prs;
 *   - file name to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndPrimsLoad(dd6PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, i;
  BYTE *mem, *ptr;
  DWORD Sign;
  INT NumOfPrims, NumOfMtls, NumOfTexs;
  struct tagMTLS
  {
    /* Load material name */
    CHAR Name[300];
    /* Illumination coefficients */    
    VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
    FLT Ph;                   /* Phong power coefficient */
    FLT Trans;                /* Transparency factor */
    INT Tex[8];               /* Texture references from texture table */
    /* Shader string */
    CHAR ShaderStr[300];
    INT ShaderNo;             /* Shader reference in shader table */
  } *mtls;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  rewind(F);
  ptr = mem;
  fread(mem, 1, flen, F);

  Sign = *(DWORD *)ptr, ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
  NumOfPrims = *(INT *)ptr, ptr += 4;
  NumOfMtls = *(INT *)ptr, ptr += 4;
  NumOfTexs = *(INT *)ptr, ptr += 4;
  if (!DD6_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
  /* Primitives */
  for (i = 0; i < NumOfPrims; i++)
  {
    INT nv, ni, mtlno;
    DD6VERTEX *V;
    INT *Ind;

    nv = *(INT *)ptr, ptr += 4;
    ni = *(INT *)ptr, ptr += 4;
    mtlno = *(INT *)ptr, ptr += 4;
    V = (DD6VERTEX *)ptr, ptr += sizeof(DD6VERTEX) * nv;
    Ind = (INT *)ptr, ptr += sizeof(INT) * ni;
    DD6_RndPrimCreate(&Prs->Prims[i], DD6_RND_PRIM_TRIMESH, V, nv, Ind, ni);
    Prs->Prims[i].Mtlno = mtlno + DD6_RndMaterialsSize;
  }

  /* Materials */
  mtls = (struct tagMTLS *)ptr, ptr += sizeof(struct tagMTLS) * NumOfMtls;
  for (i = 0; i < NumOfMtls; i++)
  {
    INT j;
    dd6MATERIAL mtl = {{0}};

    strcpy(mtl.Name, mtls[i].Name);
    mtl.Ka = mtls[i].Ka;
    mtl.Kd = mtls[i].Kd;
    mtl.Ks = mtls[i].Ks;
    mtl.Ph = mtls[i].Ph;
    mtl.Trans = mtls[i].Trans;
    for (j = 0; j < 8; j++)
    {
      if (mtls[i].Tex[j] != -1)
        mtls[i].Tex[j] += DD6_RndTexturesSize;
      mtl.Tex[j] = mtls[i].Tex[j];
    }
    DD6_RndMtlAdd(&mtl);
  }


  /* Textures */
  for (i = 0; i < NumOfTexs; i++)
  {
    CHAR *Name = (CHAR *)ptr;
    INT
      W = *(INT *)(ptr + 300), 
      H = *(INT *)(ptr + 300 + 4), 
      BPP = *(INT *)(ptr + 300 + 4 + 4);

    ptr += 312;
    DD6_RndTexAddImg(Name, W, H, BPP, ptr);
    ptr += W * H * BPP;
  }
  

  free(mem);
  fclose(F);
#if 0
 
#endif /* 0 */

  return TRUE;
} /* End of 'DD6_RndPrimsLoad' function */

/* END OF 'RNDPRIMS.C' FILE */