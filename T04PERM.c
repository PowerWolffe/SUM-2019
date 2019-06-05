/* Demchenko Dmitriy, 05.06.2019 */
#include <stdio.h>
#include <windows.h>
#define MAX 3

INT P[MAX];
BOOL IsParity = 1;

VOID Store( VOID )
{
  FILE *f;

  int i = 0;

  f = fopen("PERM.txt", "a");

  if (f == NULL)
    return;
  for (i = 0; i < MAX - 1; i++)
    fprintf(f, "%d,", P[i]);
  fprintf(f, "%d\n parity %s\n", P[MAX - 1], IsParity ? "even" : "odd");
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

VOID main( void )
{
  int i;

  for (i = 0; i < MAX; i++)
    P[i] = i + 1;

  Go(0);
  
}





