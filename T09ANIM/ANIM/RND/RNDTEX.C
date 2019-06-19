/* FILE NAME   : RNDTEX.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Resourses functions.
 *               Texture support.
 * PROGRAMMER  : DD6
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */
#include <string.h>
#include "rnd.h"

/* Textures stock */
dd6TEXTURE DD6_RndTextures[DD6_MAX_TEXTURES]; /* Array of textures */
INT DD6_RndTexturesSize;                      /* Textures array store size */

/* Textures support initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndTexInit( VOID )
{
  memset(DD6_RndTextures, 0, sizeof(DD6_RndTextures));
  DD6_RndTexturesSize = 0;
} /* End of 'DD6_RndTexInit' function */

/* Textures support deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < DD6_RndTexturesSize; i++)
    glDeleteTextures(1, &DD6_RndTextures[i].TexId);
  memset(DD6_RndTextures, 0, sizeof(DD6_RndTextures));
  DD6_RndTexturesSize = 0;
} /* End of 'DD6_RndTexClose' function */
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
INT DD6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  if (DD6_RndTexturesSize >= DD6_MAX_TEXTURES)
    return -1;

  /* Allocate texture space */
  glGenTextures(1, &DD6_RndTextures[DD6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, DD6_RndTextures[DD6_RndTexturesSize].TexId);


  /* Upload texture */
  if (C == 4)
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  else
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, W, H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Bits);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  strncpy(DD6_RndTextures[DD6_RndTexturesSize].Name, Name, DD6_STR_MAX - 1);
  DD6_RndTextures[DD6_RndTexturesSize].W = W;
  DD6_RndTextures[DD6_RndTexturesSize].H = H;

  glBindTexture(GL_TEXTURE_2D, 0);

  return DD6_RndTexturesSize++;
} /* End of 'DD6_RndTexAddImg' function */

/* END OF 'RNDTEX.C' FILE */