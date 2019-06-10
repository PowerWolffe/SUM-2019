/* FILE NAME   : MTH.H
 * PROGRAMMER  : Demchenko Dmitriy
 * LAST UPDATE : 10.06.2019.
 * NOTE        : Module prefix 'DD6'.
 */  

#ifndef __MTH_H_
#define __MTH_H_
#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))
#include <math.h>
#include <windows.h>
#include "GLOBE.h"

typedef double DBL;
typedef struct
{
  DOUBLE X, Y, Z;
}VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

static MATR UniteMatr =
{
{
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
}
};

__inline VEC VecSet(DBL X, DBL Y, DBL Z)
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
}

__inline VEC VecAddVec(VEC V1, VEC V2)
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

__inline VEC VecSubVec(VEC V1, VEC V2)
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum(VEC V, DBL N)
{
  return VecSet(V.X * N, V.Y * N, V.Z * N);
}

__inline VEC VecDivNum(VEC V1, DBL N)
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg(VEC V)
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

__inline DBL VecDotVec(VEC V1, VEC V2)
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecCrossVec(VEC V1, VEC V2)
{

}

__inline DBL VecLen2(VEC V)
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline DBL VecLen(VEC V)
{
  DBL len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
      return sqrt(len);
  return len;
}

__inline VEC VecMulMatr(VEC V, MATR M)
{
  return VecSet(V.X * M.A[0][0] + V.X * M.A[0][1] + V.X * M.A[0][2] + +V.X * M.A[0][3], V.Y * M.A[1][0] + V.Y * M.A[1][1] + V.Y * M.A[1][2] + +V.Y * M.A[1][3], V.Z * M.A[2][0] + V.Z * M.A[2][1] + V.Z * M.A[2][2] + +V.Z * M.A[2][3]);
}

__inline VEC PointTransform(VEC V, MATR M)
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}

__inline MATR MatrIdentity(VOID)
{
  MATR m = UniteMatr;

  return m;
}
__inline MATR MatrTranslate(VEC Dv)
{
  MATR m =
  {
    {
      {   1,    0,    0,  0},
      {   0,    1,    0,  0},
      {   0,    0,    1,  0},
      {Dv.X, Dv.Y, Dv.Z,  1}
    }
  };

  return m;
}

__inline MATR SetMatr(DBL A00, DBL A01, DBL A02, DBL A03,
  DBL A10, DBL A11, DBL A12, DBL A13,
  DBL A20, DBL A21, DBL A22, DBL A23,
  DBL A30, DBL A31, DBL A32, DBL A33)
{
  MATR m;

  m.A[0][0] = A00;
  m.A[0][1] = A01;
  m.A[0][2] = A02;
  m.A[0][3] = A03;
  m.A[1][0] = A10;
  m.A[1][1] = A11;
  m.A[1][2] = A12;
  m.A[1][3] = A13;
  m.A[2][0] = A20;
  m.A[2][1] = A21;
  m.A[2][2] = A22;
  m.A[2][3] = A23;
  m.A[3][0] = A30;
  m.A[3][1] = A31;
  m.A[3][2] = A32;
  m.A[3][3] = A33;

  return m;
}
__inline MATR MatrScale(VEC Sv)
{
  MATR m =
  {
    {
      {Sv.X,    0,    0,  0},
      {   0, Sv.Y,    0,  0},
      {   0,    0, Sv.Z,  0},
      {   0,    0,    0,  1}
    }
  };

  return m;
}

__inline VEC VectorTransform(VEC V, MATR M)
{
  return VecSet(V.X * M.A[0][0] + V.X * M.A[0][1] + V.X * M.A[0][2],
      V.Y * M.A[1][0] + V.Y * M.A[1][1] + V.Y * M.A[1][2],
      V.Z * M.A[2][0] + V.Z * M.A[2][1] + V.Z * M.A[2][2]);
}

__inline MATR MatrMulMatr(MATR M1, MATR M2)
{
  MATR r;

  r.A[0][0] = M1.A[0][0] * M2.A[0][0] + M1.A[0][1] * M2.A[1][0] + M1.A[0][2] * M2.A[2][0] + M1.A[0][3] * M2.A[3][0];
  r.A[0][1] = M1.A[0][0] * M2.A[0][1] + M1.A[0][1] * M2.A[1][1] + M1.A[0][2] * M2.A[2][1] + M1.A[0][3] * M2.A[3][1];
  r.A[0][2] = M1.A[0][0] * M2.A[0][2] + M1.A[0][1] * M2.A[1][2] + M1.A[0][2] * M2.A[2][2] + M1.A[0][3] * M2.A[3][2];
  r.A[0][3] = M1.A[0][0] * M2.A[0][3] + M1.A[0][1] * M2.A[1][3] + M1.A[0][2] * M2.A[2][3] + M1.A[0][3] * M2.A[3][3];

  r.A[1][0] = M1.A[1][0] * M2.A[0][0] + M1.A[1][1] * M2.A[1][0] + M1.A[1][2] * M2.A[2][0] + M1.A[1][3] * M2.A[3][0];
  r.A[1][1] = M1.A[1][0] * M2.A[0][1] + M1.A[1][1] * M2.A[1][1] + M1.A[1][2] * M2.A[2][1] + M1.A[1][3] * M2.A[3][1];
  r.A[1][2] = M1.A[1][0] * M2.A[0][2] + M1.A[1][1] * M2.A[1][2] + M1.A[1][2] * M2.A[2][2] + M1.A[1][3] * M2.A[3][2];
  r.A[1][3] = M1.A[1][0] * M2.A[0][3] + M1.A[1][1] * M2.A[1][3] + M1.A[1][2] * M2.A[2][3] + M1.A[1][3] * M2.A[3][3];

  r.A[2][0] = M1.A[2][0] * M2.A[0][0] + M1.A[2][1] * M2.A[1][0] + M1.A[2][2] * M2.A[2][0] + M1.A[2][3] * M2.A[3][0];
  r.A[2][1] = M1.A[2][0] * M2.A[0][1] + M1.A[2][1] * M2.A[1][1] + M1.A[2][2] * M2.A[2][1] + M1.A[2][3] * M2.A[3][1];
  r.A[2][2] = M1.A[2][0] * M2.A[0][2] + M1.A[2][1] * M2.A[1][2] + M1.A[2][2] * M2.A[2][2] + M1.A[2][3] * M2.A[3][2];
  r.A[2][3] = M1.A[2][0] * M2.A[0][3] + M1.A[2][1] * M2.A[1][3] + M1.A[2][2] * M2.A[2][3] + M1.A[2][3] * M2.A[3][3];

  r.A[3][0] = M1.A[3][0] * M2.A[0][0] + M1.A[3][1] * M2.A[1][0] + M1.A[3][2] * M2.A[2][0] + M1.A[3][3] * M2.A[3][0];
  r.A[3][1] = M1.A[3][0] * M2.A[0][1] + M1.A[3][1] * M2.A[1][1] + M1.A[3][2] * M2.A[2][1] + M1.A[3][3] * M2.A[3][1];
  r.A[3][2] = M1.A[3][0] * M2.A[0][2] + M1.A[3][1] * M2.A[1][2] + M1.A[3][2] * M2.A[2][2] + M1.A[3][3] * M2.A[3][2];
  r.A[3][3] = M1.A[3][0] * M2.A[0][3] + M1.A[3][1] * M2.A[1][3] + M1.A[3][2] * M2.A[2][3] + M1.A[3][3] * M2.A[3][3];

  return r;
}

__inline MATR MatrTranspose(MATR M)
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++)
          r.A[i][j] = M.A[j][i];
  return r;
}

__inline DBL MatrDeterm3x3(DBL A11, DBL A12, DBL A13,
  DBL A21, DBL A22, DBL A23,
  DBL A31, DBL A32, DBL A33)
{
  return A11 * A22 * A33 -
      A11 * A23 * A32 -
      A12 * A21 * A33 +
      A12 * A23 * A31 +
      A13 * A21 * A32 -
      A13 * A22 * A31;
}

__inline DBL MatrDeterm(MATR M)
{
  return
      M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
          M.A[2][1], M.A[2][2], M.A[2][3],
          M.A[3][1], M.A[3][2], M.A[3][3]) -
      M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
          M.A[2][0], M.A[2][2], M.A[2][3],
          M.A[3][0], M.A[3][2], M.A[3][3]) +
      M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
          M.A[2][0], M.A[2][1], M.A[2][3],
          M.A[3][0], M.A[3][1], M.A[3][3]) -
      M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
          M.A[2][0], M.A[2][1], M.A[2][2],
          M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse(MATR M)
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
  {
      MATR m = UniteMatr;

      return m;
  }

  r.A[0][0] =
      MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
          M.A[2][1], M.A[2][2], M.A[2][3],
          M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
      -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
          M.A[2][0], M.A[2][2], M.A[2][3],
          M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
      MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
          M.A[2][0], M.A[2][1], M.A[2][3],
          M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
      -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
          M.A[2][0], M.A[2][1], M.A[2][2],
          M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] =
      -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
          M.A[2][1], M.A[2][2], M.A[2][3],
          M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
      MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
          M.A[2][0], M.A[2][2], M.A[2][3],
          M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
      -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
          M.A[2][0], M.A[2][1], M.A[2][3],
          M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
      MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
          M.A[2][0], M.A[2][1], M.A[2][2],
          M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] =
      MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
          M.A[1][1], M.A[1][2], M.A[1][3],
          M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
      -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
          M.A[1][0], M.A[1][2], M.A[1][3],
          M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
      MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
          M.A[1][0], M.A[1][1], M.A[1][3],
          M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
      -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
          M.A[1][0], M.A[1][1], M.A[1][2],
          M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] =
      -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
          M.A[1][1], M.A[1][2], M.A[1][3],
          M.A[2][1], M.A[2][2], M.A[2][3]) / det;

  r.A[1][3] =
      MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
          M.A[1][0], M.A[1][2], M.A[1][3],
          M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
      -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
          M.A[1][0], M.A[1][1], M.A[1][3],
          M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
      MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
          M.A[1][0], M.A[1][1], M.A[1][2],
          M.A[2][0], M.A[2][1], M.A[2][2]) / det;
  return r;
}

__inline MATR MatrRotateX( DBL AngleInDegree )
{
  MATR MatrRotateX = MatrIdentity();
  DBL A = (DBL)D2R(AngleInDegree), si = sin(A), co = cos(A);

  MatrRotateX.A[1][1] = co;
  MatrRotateX.A[1][2] = si;
  MatrRotateX.A[2][1] = -si;
  MatrRotateX.A[2][2] = co;

  return MatrRotateX;
} 

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  MATR MatrRotateY = MatrIdentity();
  DBL A = (DBL)D2R(AngleInDegree), si = sin(A), co = cos(A);
  MatrRotateY.A[0][0] = co;
  MatrRotateY.A[0][2] = -si;
  MatrRotateY.A[2][0] = si;
  MatrRotateY.A[2][2] = co;
  return MatrRotateY;
} 

__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  MATR MatrRotateZ = MatrIdentity();
  DBL A = (DBL)D2R(AngleInDegree), si = sin(A), co = cos(A);
  MatrRotateZ.A[0][0] = co;
  MatrRotateZ.A[1][0] = -si;
  MatrRotateZ.A[0][1] = si;
  MatrRotateZ.A[1][1] = co;

  return MatrRotateZ;
}


__inline VEC VecNormalise(VEC V)
{
  if (VecLen(V) == 1 || 0)
      return V;
  else
      return VecDivNum(V, VecLen(V));
}

__inline MATR MatrRotate(DBL AngleInDegree, VEC R)
{
  DBL A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalise(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),
        V.X * V.Y * (1 - co) + V.Z * si,
          V.X * V.Z * (1 - co) - V.Y * si, 0},
      {V.Y * V.X * (1 - co) - V.Z * si,
        co + V.Y * V.Y * (1 - co),
          V.Y * V.Z * (1 - co) + V.X * si, 0},
      {V.Z * V.X * (1 - co) + V.Y * si,
        V.Z * V.Y * (1 - co) - V.Y * si,
        co + V.Z * V.Z * (1 - co), 0},
      {0, 0, 0, 1}
    }
  };
}

#endif