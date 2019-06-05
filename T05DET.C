/* Demchenko Dmitriy, 05.06.2019 */
#include <stdio.h>
#include <windows.h>
#define MAX 3

INT P[MAX];
BOOL IsParity = 1;
INT N;
DOUBLE A[MAX][MAX], Det;
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


VOID swap (INT *a, INT *b)
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
      swap (&P[pos], &P[i]);
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

VOID main( void )
{
  int i;

  for (i = 0; i < MAX; i++)
    P[i] = i;
  LOADMATRIX("IN.txt");
  Go(0);
  printf("%lf", Det);
  _getch();
}
