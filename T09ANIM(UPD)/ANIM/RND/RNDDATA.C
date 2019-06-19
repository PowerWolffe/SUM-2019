/* FILE NAME   : RNDDATA.C
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 17.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */  

#include "../../DEF.H"

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

DBL
  DD6_RndProjSize = 0.1,  /* Project plane fit square */
  DD6_RndProjDist = 0.1,  /* Distance to project plane from viewer (near) */
  DD6_RndProjFarClip = 1000;  /* Distance to project far clip plane (far) */

MATR
  DD6_RndMatrView, /* View coordinate system matrix */
  DD6_RndMatrProj, /* Projection coordinate system matrix */
  DD6_RndMatrVP;   /* Stored (View * Proj) matrix */

