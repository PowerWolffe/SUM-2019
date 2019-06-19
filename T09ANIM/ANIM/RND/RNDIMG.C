/* FILE NAME   : RNDIMG.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Image handle.
 * LAST UPDATE : 18.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include <stdio.h>

#include "rnd.h"


/* Load image function.
 * ARGUMENTS:
 *   - image structure:
 *      dd6IMAGE *Img;
 *   - image file name (BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DD6_RndImgLoad( dd6IMAGE *Img, CHAR *FileName )
{
  memset(Img, 0, sizeof(dd6IMAGE));

  Img->hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  if (Img->hBm != NULL)
  {
    GetObject(Img->hBm, sizeof(Img->bm), &Img->bm);
    Img->W = Img->bm.bmWidth;
    Img->H = Img->bm.bmHeight;
    return TRUE;
  }
  else
    return FALSE;
} /* End of 'DD6_RndImgLoad' function */

/* Free image function.
 * ARGUMENTS:
 *   - image structure:
 *       dd6IMAGE *Img;
 * RETURNS: None.
 */
VOID DD6_RndImgFree(dd6IMAGE *Img )
{
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);

  memset(Img, 0, sizeof(dd6IMAGE));
} /* End of 'DD6_RndImgFree' function */

/* Image pixel value get function.
 * ARGUMENTS:
 *   - image structure:
 *      dd6IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (VEC) mapped to 0..1 color values.
 */
VEC DD6_RndImgGet( dd6IMAGE *Img, INT X, INT Y )
{
  X = (X + Img->W) % Img->W;
  Y = (Y + Img->H) % Img->H;
  if (Img->bm.bmBitsPixel == 8)
  {
    BYTE c = *((BYTE *)Img->bm.bmBits + Y * Img->bm.bmWidthBytes + X);
    return VecSet(c / 255.0, c / 255.0, c / 255.0);
  }
  else if (Img->bm.bmBitsPixel == 32)
  {
    BYTE *c;

    c = (BYTE *)Img->bm.bmBits + Y * Img->bm.bmWidthBytes + X * 4;
    return VecSet(c[2] / 255.0, c[1] / 255.0, c[0] / 255.0);
  }
  return VecSet(0, 0, 0);
} /* End of 'DD6_RndImgGet' function */

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
BOOL DD6_RndImgToHF( dd6IMAGE *Img, dd6PRIM *Pr, DBL ScaleH )
{
  DD6VERTEX *V;
  INT *Ind;
  INT
    i, j, k,
    size = (Img->H - 1) * (2 * Img->W + 1) * sizeof(INT) + Img->W * Img->H * sizeof(DD6VERTEX);

  memset(Pr, 0, sizeof(dd6PRIM));
  if (Img->W == 0 || Img->H == 0 || Img->hBm == NULL || Img->bm.bmBits == NULL)
    return FALSE;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + Img->W * Img->H);
  memset(V, 0, size);

  /* Setup vertices */
  for (k = 0, i = 0; i < Img->H; i++)
    for (j = 0; j < Img->W; j++)
    {
      VEC4 c = {1, 1, 1, 1};
      VEC v = DD6_RndImgGet(Img, j, i);

      V[k].P = VecSet(j, v.Y * ScaleH, Img->H - 1 - i);
      V[k].T.X = j / (Img->W - 1.0);
      V[k].T.Y = i / (Img->H - 1.0);
      V[k].N = VecSet(0, 1, 0);
      V[k].C = c;
      k++;
    }

  /* Setup indices */
  for (k = 0, i = 0; i < Img->H - 1; i++)
  {
    for (j = 0; j < Img->W; j++)
    {
      Ind[k++] = (i + 1) * Img->W + j;
      Ind[k++] = i * Img->W + j;
    }
    Ind[k++] = -1;
  }

  /*** Evaluate vertex normals ***/

  /* Set all normals to zero */
  for (i = 0; i < Img->W * Img->H; i++)
    V[i].N = VecSet(0, 0, 0);

  /* Evaluate all triangles normals */
  for (i = 0; i < Img->H - 1; i++)
    for (j = 0; j < Img->W - 1; j++)
    {
      DD6VERTEX
        *v00 = V + i * Img->W + j,
        *v01 = V + i * Img->W + j + 1,
        *v10 = V + (i + 1) * Img->W + j,
        *v11 = V + (i + 1) * Img->W + j + 1;
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(v00->P, v10->P), VecSubVec(v11->P, v10->P)));
      v00->N = VecAddVec(v00->N, N);
      v10->N = VecAddVec(v10->N, N);
      v11->N = VecAddVec(v11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(v11->P, v01->P), VecSubVec(v00->P, v01->P)));
      v00->N = VecAddVec(v00->N, N);
      v01->N = VecAddVec(v01->N, N);
      v11->N = VecAddVec(v11->N, N);
    }


  /* Normalize all normals to zero */
  for (i = 0; i < Img->W * Img->H; i++)
    V[i].N = VecNormalize(V[i].N);

 DD6_RndPrimCreate(Pr, DD6_RND_PRIM_GRID, V, Img->W * Img->H, Ind, (Img->H - 1) * (2 * Img->W + 1));
  return TRUE;
} /* End of 'DD6_RndImgToHF' function */

/* Load image function.
 * ARGUMENTS:
 *   - image file name (BMP):
 *       CHAR *FileName;
  
 * RETURNS:
 *   (INT) add texture stock index.
 */
INT DD6_RndTexLoad( CHAR *FileName )
{
  INT n;
  dd6IMAGE img;

  if (!DD6_RndImgLoad(&img, FileName))
    return -1;
  n = DD6_RndTexAddImg(FileName, img.W, img.H, img.bm.bmBitsPixel / 8, img.bm.bmBits);
  DD6_RndImgFree(&img);
  return n;
} /* End of 'DD6_RndTexLoad' function */

/* END OF 'RNDIMG.C' FILE */