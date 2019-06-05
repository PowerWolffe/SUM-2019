/* Demchenko Dmitriy, 05.06.2019 */
#include <stdio.h>
#include <windows.h>
#include <math.h>
#define MAX 

INT P[MAX];
BOOL IsParity = 1;
INT N;
DOUBLE A[MAX][MAX], Det, GDet;
int i, j;
FILE *F;


VOID Store( VOID )
{
  int i = 0;
  double prod;

  for( i = 0, prod = 1; i < N; i++)
    prod *= A[i][P[i]];
  Det += (IsParity ? 1 : -1) * prod;
}


VOID Swap (INT *a, INT *b)
{
  INT tmp = *a;

  *a = *b;
  *b = tmp;
}

VOID Go( int pos)
{
  INT i, x;
  BOOL save_parity;

  if (pos == MAX)
    Store();
  else 
  {
    save_parity = IsParity;
    Go(pos + 1);
    for (i = pos + 1; i < MAX; i++ )
    {
      Swap (&P[pos], &P[i]);
      if (IsParity == 0 && i != pos )
        IsParity = 1;
      else if (IsParity == 1 && i != pos)
        IsParity = 0;
      Go(pos + 1);
    }
    x = P[pos];
    for (i = pos + 1; i < MAX; i++)
    P[i -1] = P[i];
    P[MAX - 1] = x;
    IsParity = save_parity;
  }
  
}

BOOL LOADMATRIX ( CHAR *FNAME )
{
  FILE *F;
  N = 0;

  if ((F = fopen (FNAME, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N < 0)
    N = 0;
  else 
    if (N > MAX)
      N = MAX;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
}

VOID GAUSSDET ( VOID )
{

  INT i, y, x, k, max_col, max_row;
  double coef;
  GDet = 1;

  for ( i = 0; i < N; i++)
  {
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for(x = i; x < N; x++)
        if(fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_col = x, max_row = y;
    if (A[max_row][max_col] == 0)
    {
      GDet = 0;
      break;
    }
    if ( max_row != i)
    {
      for (x = i; x < N; x++)
        Swap(&A[i][x], &A[max_row][x]);
      GDet = -GDet;
    }
     if ( max_col != i)
    {
      for (y = i; y < N; y++)
        Swap(&A[y][max_col], &A[y][max_col]);
      GDet = -GDet;
    }
    
     coef = A[j][i] / A[i][i];
     A[j][i] = 0;
     for(k = i + 1; k < N; k++)
       A[j][k] -= A[i][k] * coef;
  }

GDet *= A[i][i];
 
}
VOID main( void )
{
  int i;

  for (i = 0; i < MAX; i++)
    P[i] = i;
  LOADMATRIX("IN.txt");
  Go(0);
  printf("Non-Gauss Determination:%lf\n", Det);
  printf("Gauss-Determination: %lf", GDet);
  getch();
}