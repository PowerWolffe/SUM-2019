/* FILE NAME   : RNDSHD.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Base functions.
 * PROGRAMMER  : DD6.
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */

#include <stdio.h>
#include <string.h>
#include "../anim.h"
#include "rnd.h"

/* Global shader program Id */
INT DD6_RndProgId;

/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */
static CHAR * DD6_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  /* Allocate memory for file text */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }

  /* Load text from file to memory */
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
} /* End of 'DD6_RndShdLoadTextFromFile' function */

/* Store log to file function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *PartName, *Text;
 * RETURNS: None.
 */
static VOID DD6_RndShdLog( CHAR *Prefix, CHAR *Type, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/DD6{30}SHD.LOG", "a")) == NULL)
    return;
  fprintf(F, "%s/%s.GLSL:\n%s\n", Prefix, Type, Text);
  fclose(F);
} /* End of 'DD6_RndShdLog' function */

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT DD6_RndShdLoad( CHAR *ShaderFileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader file prefix name */
    INT Type;   /* Shader OpenGL type */
    INT Id;     /* Shader Id created by OpenGL */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER},
    {"FRAG", GL_FRAGMENT_SHADER}
  };
  INT i, Ns = sizeof(shd) / sizeof(shd[0]), res, prg = 0;
  CHAR *txt;
  BOOL is_ok = TRUE;
  static CHAR Buf[3000];

  for (i = 0; is_ok && i < Ns; i++)
  {
    /* Load shader text from file */
    sprintf(Buf, "BIN/SHADERS/%s/%s.glsl", ShaderFileNamePrefix, shd[i].Name);
    if ((txt = DD6_RndShdLoadTextFromFile(Buf)) == NULL)
    {
      DD6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      DD6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error create shader");
      free(txt);
      is_ok = FALSE;
      break;
    }
    /* Attach shader source code to shader */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      DD6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      is_ok = FALSE;
      DD6_RndShdLog(ShaderFileNamePrefix, "PROG", "Error create program");
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < Ns; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        DD6_RndShdLog(ShaderFileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
  }

  if (!is_ok)
  {
    /* Remove all shaders */
    for (i = 0; i < Ns; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Remove program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'DD6_RndShdLoad' function */

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID DD6_RndShdDelete( INT ProgId )
{
  INT shdrs[5], n, i;

  if (ProgId == 0)
    return;
  glGetAttachedShaders(ProgId, 5, &n, shdrs);
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'DD6_RndShdDelete' function */

/* Shaders support initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndShdInit( VOID )
{
  memset(DD6_RndShaders, 0, sizeof(DD6_RndShaders));
  DD6_RndShadersSize = 0;
  DD6_RndShdAdd("DEFAULT");
} /* End of 'DD6_RndShdInit' function */

/* Shaders support dinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < DD6_RndShadersSize; i++)
    DD6_RndShdDelete(DD6_RndShaders[i].ProgId);
  memset(DD6_RndShaders, 0, sizeof(DD6_RndShaders));
  DD6_RndShadersSize = 0;
} /* End of 'DD6_RndShdClose' function */

/* Load shader program and add to stock function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader stock index.
 */
INT DD6_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < DD6_RndShadersSize; i++)
    if (strcmp(DD6_RndShaders[i].Name, ShaderFileNamePrefix) == 0)
    {
      DD6_RndShdDelete(DD6_RndShaders[i].ProgId);
      DD6_RndShaders[i].ProgId = DD6_RndShdLoad(ShaderFileNamePrefix);
      return i;
    }
  if (DD6_RndShadersSize >= DD6_MAX_SHADERS)
    return 0;
  strncpy(DD6_RndShaders[DD6_RndShadersSize].Name,
          ShaderFileNamePrefix, DD6_STR_MAX - 1);
  DD6_RndShaders[DD6_RndShadersSize].ProgId = DD6_RndShdLoad(ShaderFileNamePrefix);
  return DD6_RndShadersSize++;
} /* End of 'DD6_RndShdAdd' function */

/* Shaders stock update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DD6_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < DD6_RndShadersSize; i++)
  {
    DD6_RndShdDelete(DD6_RndShaders[i].ProgId);
    DD6_RndShaders[i].ProgId = DD6_RndShdLoad(DD6_RndShaders[i].Name);
  }
} /* End of 'DD6_RndShdUpdate' function */

/* END OF 'RNDSHD.C' FILE */
