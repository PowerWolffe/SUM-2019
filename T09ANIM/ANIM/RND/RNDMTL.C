/* FILE NAME   : RNDMTL.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Resourses functions.
 *               Material support.
 * PROGRAMMER  : DD6.
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include <string.h>

#include "rnd.h"

INT DD6_RndShadersSize;
dd6SHADER DD6_RndShaders[DD6_MAX_SHADERS];

/* Material stock */
dd6MATERIAL DD6_RndMaterials[DD6_MAX_MATERIALS]; /* Array of materials */
INT DD6_RndMaterialsSize;                        /* Materials array store size */

/* Materials support initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndMtlInit( VOID )
{
  dd6MATERIAL mtl =
  {
    "Default",
    0,
    {0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, {0, 0, 0},
    30,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1}
  };

  memset(DD6_RndMaterials, 0, sizeof(DD6_RndMaterials));
  DD6_RndMaterialsSize = 0;
  DD6_RndMtlAdd(&mtl);
} /* End of 'DD6_RndMtlInit' function */

/* Add new material to stock function.
 * ARGUMENTS:
 *   - material description structure pointer:
 *       dd6MATERIAL *Mtl;
 * RETURNS:
 *   (INT) added material stock index.
 */
INT DD6_RndMtlAdd( dd6MATERIAL *Mtl )
{
  /*
  INT i;

  for (i = 0; i < DD6_RndMaterialsSize; i++)
    if (strcmp(DD6_RndMaterials[i].Name, Mtl->Name) == 0)
    {
      DD6_RndMaterials[i] = *Mtl;
      return i;
    }
  */
  if (DD6_RndMaterialsSize >= DD6_MAX_MATERIALS)
    return 0;
  DD6_RndMaterials[DD6_RndMaterialsSize] = *Mtl;
  return DD6_RndMaterialsSize++;
} /* End of 'DD6_RndMtlAdd' function */

/* Add new material to stock function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) applied material shader program Id.
 */
INT DD6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  dd6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DD6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DD6_RndMaterials[MtlNo];

  /* Set program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= DD6_RndShadersSize)
    prg = 0;
  else
    prg = DD6_RndShaders[prg].ProgId;

  glUseProgram(prg);
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Textures handle */
  for (i = 0; i < 8; i++)
  {
    CHAR is[18] = "IsTexture0";

    /* Set texture presence flag */
    is[9] = '0' + i;
    if ((loc = glGetUniformLocation(prg, is)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);

    glActiveTexture(GL_TEXTURE0 + i);
    if (mtl->Tex[i] == -1)
      glBindTexture(GL_TEXTURE_2D, 0);
    else
    {
      INT tn = mtl->Tex[i];

      if (tn < 0 || tn >= DD6_RndTexturesSize)
        tn = 0;
      else
        tn = DD6_RndTextures[tn].TexId;

      glBindTexture(GL_TEXTURE_2D, tn);
    }
  }
  return prg;
} /* End of 'DD6_RndMtlApply' function */

/* END OF 'RNDMTL.C' FILE */