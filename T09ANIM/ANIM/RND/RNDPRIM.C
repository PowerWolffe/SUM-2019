/* FILE NAME   : RNDPRIM.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */  

#include <string.h>
#include <stdio.h>

#include "../anim.h"

/* Primitive addon data */
FLT DD6_RndAddon[3];

BOOL DD6_RndPrimCreate( dd6PRIM *Pr, dd6PRIM_TYPE Type, DD6VERTEX *V, INT NoofV, INT *I, INT NoofI )
{
  memset(Pr, 0, sizeof(dd6PRIM));

  /* Vertex */
  if (V != NULL && NoofV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DD6VERTEX) * NoofV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(DD6VERTEX), (VOID *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(DD6VERTEX), (VOID *)(4 * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(DD6VERTEX), (VOID *)(4 * 5));
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(DD6VERTEX), (VOID *)(4 * 8));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
  }

  /* Index */
  if (I != NULL && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);

    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;

  Pr->Type = Type;
  Pr->Trans = MatrIdentity();
  return TRUE;
} /* End of 'DD6_RndPrimCreate' function */

VOID DD6_RndPrimFree( dd6PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  glDeleteBuffers(1, &Pr->IBuf);

  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(dd6PRIM));
} /* End of 'DD6_RndPrimFree' function */

VOID DD6_RndPrimDraw( dd6PRIM *Pr, MATR World )
{
  INT loc;
  /* vertex projections */
  MATR M = MatrMulMatr3(Pr->Trans, World, DD6_RndMatrVP);

  /* Draw all triangles */
  glLoadMatrixf(M.A[0]);

  /* Apply materials/shaders */

  DD6_RndProgId = DD6_RndMtlApply(Pr->Mtlno);

  if ((loc = glGetUniformLocation(DD6_RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
  if ((loc = glGetUniformLocation(DD6_RndProgId, "Time")) != -1)
    glUniform1f(loc, DD6_Anim.Time);
  if ((loc = glGetUniformLocation(DD6_RndProgId, "Addon0")) != -1)
    glUniform1f(loc, DD6_RndAddon[0]);
  if ((loc = glGetUniformLocation(DD6_RndProgId, "Addon1")) != -1)
    glUniform1f(loc, DD6_RndAddon[1]);
  if ((loc = glGetUniformLocation(DD6_RndProgId, "Addon2")) != -1)
    glUniform1f(loc, DD6_RndAddon[2]);

  /* Draw all triangles */
  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(GL_TRIANGLES, 0, Pr->NumOfElements);
  else 
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(GL_TRIANGLE_STRIP, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
  }
  glUseProgram(0);
} /* End of 'DD6_RndPrimDraw' function */


BOOL DD6_RndPrimLoad( dd6PRIM *Pr, CHAR *FileName )
{
  INT nv, nf;
  INT *I;
  DD6VERTEX *V;
  FILE *F;
  CHAR Buf[1000];

  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(dd6PRIM));

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertex and facet quantity */
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  /* Allocate memory */
  V = malloc(nv * sizeof(DD6VERTEX) + 3 * nf * sizeof(INT));
  if (V == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Set index array pointer */
  I = (INT *)(V + nv);

  /* Fill all allocated memory by 0 */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d%//%*d %d%//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      I[nf++] = n1 - 1;
      I[nf++] = n2 - 1;
      I[nf++] = n3 - 1;
    }
  fclose(F);
  DD6_RndPrimCreate(Pr, DD6_RND_PRIM_TRIMESH, V, nv, I, nf);
  free(V);
  return TRUE;
} /* End of 'DD6_RndPrimLoad' function */

