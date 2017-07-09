/*
 * Filename: permgen.c
 * Purpose : Permutation generator written in ANSI/ISO C.
 *           All algorithms are based on:
 *           Donald E. Knuth, "The Art of Computer Programming, volume 4,
 *           fascicle 2B, Chapter 7.2.1.2: Generating all permutations".
 * Author  : Nikolaos Kavvadias (C) 2013
 * Date    : 03-Mar-2013
 * Revision: 0.0.1 (13/03/03)
 *           Initial version.
 */
/* Input file format:
 *
 * The input file for this program holds the values of the elements of the
 * set, for which all permutations will be generated.
 * Here is a sample input file:
 * 1 2 3 4 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DIM
#define DIM         10
#endif
#ifndef FACT_DIM
#define FACT_DIM   1*2*3*4*5*6*7*8*9*10
#endif

#define SWAP(x, y)   do { x ^= y; y ^= x; x ^= y; } while (0)

#ifdef NOMALLOC
int values[DIM];
#endif
FILE *file_i, *file_o;
int enable_algo_l=1, enable_algo_p=0, enable_algo_c=0, enable_algo_e=0;
int enable_incremental_set=0;
int perm_cnt=0;


/* print_perm:
 * Prints a permutation to file f.
 */
void print_perm(FILE *f, int *perm, int n, int start_pos)
{
  int i;
  for (i = start_pos; i < n; i++) {
    fprintf(f, "%d ", perm[i]);
  }
  fprintf(f, "\n");
}

/* fact: Single-precision integer factorial calculation.
 */
int fact(int n)
{
  int res, x, i;
  x = n;
  res = 1;
  for (i = 1; i <= x; i++)
  {
    res = res * i;
  }
  return res;
}

/* TAOCP_V4F2B_L:
 * Generate all permutations of a set or multiset using lexicographic
 * permutation generation.
 * Given a sequence of n elements a1,a2,...,an, initially sorted so that
 * a1 <= a2 <= ... <= an,
 * this algorithm generates all permutations of {a1,a2,...,an}, visiting them
 * in lexicographic order. For example the permutations of {1,2,2,3} are
 * 1223, 1232, 1322, 2123, 2132, 2213, 2231, 2312, 2321, 3122, 3212, 3221,
 * ordered lexicographically. An auxiliary element a0 is assumed to present
 * for convenience, a0 must be strictly less than the largets element an.
 * The algorithm proceeds as follows:
 * L1. [Visit.] Visit the permutation a1a2...an.
 * L2. [Find j.] Set j <- n - 1. If aj >= a_{j+1}, decrease j by 1 repeatedly
 * until aj < a_{j+1}. Terminate the algorithm if j == 0. (At this point j is
 * the smallest subscript such that we have already visited all permutations
 * beginning with a1...aj. Therefore the lexicographically next permutation
 * will increase the value of aj.
 * L3. [Increase aj.] Set l <- n. If aj >= al, decrease l by 1 repeatedly until
 * aj < al. Then interchange aj <-> al. (Since a_{j+1} >= ... >= an, element
 * al is the smallest element greater than aj that can legitimately follow
 * a1...a_{j-1} in a permutation. Before the interchange we had
 * a_{j+1} >= ... >= a_{l-1} >= al > aj >= a_{l+1} >= ... >= an; after the
 * interchange, we have
 * a_{j+1} >= ... >= a_{l-1} >= aj > al >= a_{l+1} >= ... >= an.)
 * L4. [Reverse a_{j+1}...an.] Set k <- j + 1 and l <- n. Then, if k < l,
 * interchange ak <-> al, set k <- k + 1, l <- l - 1, and repeat until k >= l.
 *
 * Based on Donald E. Knuth, "The Art of Computer Programming, volume 4,
 * fascicle 2B: Algorithm L - Lexicographic permutation generation".
 */
#ifdef NOARRIF
void TAOCP_V4F2B_L(int elems)
#else
void TAOCP_V4F2B_L(int *setarr, int elems)
#endif
{
#ifdef NOMALLOC
  int a[DIM];
#else
  int *a;
#endif
  int i, j, k, l, n;
  int size;

  /* L0: Copy setarr to the perm working array. */
  size = elems;
  printf("n = %d\n", size-1);
#ifndef NOMALLOC
  a = malloc(size * sizeof(int));
#endif
  for (i = 0; i < size; i++) {
#ifdef NOARRIF
    a[i] = values[i];
#else
    a[i] = setarr[i];
#endif
  }

  /* L1. [Visit.] Visit the permutation a1a2...an. */
L1:
#ifndef NOPRINT
  print_perm(file_o, a, size, 1);
#endif
  perm_cnt++;

  /* L2. [Find j.] Set j <- n - 1. If aj >= a_{j+1}, decrease j by 1 repeatedly
   * until aj < a_{j+1}. Terminate the algorithm if j == 0. (At this point j is
   * the smallest subscript such that we have already visited all permutations
   * beginning with a1...aj. Therefore the lexicographically next permutation
   * will increase the value of aj.
   */
  n = size - 1;
  j = n - 1;
  while (a[j] >= a[j+1]) {
    j = j - 1;
  }
  if (j == 0) {
#ifndef NOMALLOC
    free(a);
#endif
    return;
  }

  /* L3. [Increase aj.] Set l <- n. If aj >= al, decrease l by 1 repeatedly
   * until aj < al. Then interchange aj <-> al. (Since a_{j+1} >= ... >= an,
   * element al is the smallest element greater than aj that can legitimately
   * follow a1...a_{j-1} in a permutation. Before the interchange we had
   * a_{j+1} >= ... >= a_{l-1} >= al > aj >= a_{l+1} >= ... >= an; after the
   * interchange, we have
   * a_{j+1} >= ... >= a_{l-1} >= aj > al >= a_{l+1} >= ... >= an.)
   */
  l = n;
  while (a[j] >= a[l]) {
    l = l - 1;
  }
  SWAP(a[j], a[l]);

  /* L4. [Reverse a_{j+1}...an.] Set k <- j + 1 and l <- n. Then, if k < l,
   * interchange ak <-> al, set k <- k + 1, l <- l - 1, and repeat until k >= l.
   * Return to L1.
   */
  k = j + 1;
  l = n;
  while (k < l) {
    SWAP(a[k], a[l]);
    k = k + 1;
    l = l - 1;
  }
  goto L1;
}

/* TAOCP_V4F2B_P:
 * Generate all permutations of a set using the plain changes method.
 * Given a sequence a1a2...an of n distinct elements, this algorithm generates
 * all of their permutations by repeatedly interchanging adjacent pairs. It
 * uses an auxiliary array c1c2...cn, which represents inversions as described
 * above, running through all sequences of integers such that
 *   0 <= cj < j    for 1 <= j <= n (1)
 * Another array o1o2...on governs the directions by which the entries cj
 * change.
 * The algorithm proceeds as follows:
 * P1. [Initialize.] Set cj <- 0 and oj <- 1 for 1 <= j <= n.
 * P2. [Visit.] Visit the permutation a1a2...an.
 * P3. [Prepare for change.] Set j <- n and s <- 0. (The following steps
 * determine the coordinate j for which cj is about to change, preserving
 * equation (1); variable s is the number of indices k > j such that
 * ck = k - 1.)
 * P4. [Ready to change?] Set q <- cj + oj. If q < 0, go to P7; if q = j, go
 * to P6.
 * P5. [Change.] Interchange a_{j-cj+s} <-> a_{j-q+s}. The set cj <- q and
 * return to P2.
 * P6. [Increase s.] Terminate if j = 1; otherwise set s <- s + 1.
 * P7. [Switch direction.] Set oj <- -oj, j <- j - 1, and go back to P4.
 * This procedure works for all n >= 1.
 *
 * Based on Donald E. Knuth, "The Art of Computer Programming, volume 4,
 * fascicle 2B: Algorithm P - Plain changes".
 */
#ifdef NOARRIF
void TAOCP_V4F2B_P(int elems)
#else
void TAOCP_V4F2B_P(int *setarr, int elems)
#endif
{
#ifdef NOMALLOC
  int a[DIM], c[DIM], o[DIM];
#else
  int *a, *c, *o;
#endif
  int i, j, n, q, s;
  int size;

  /* P0: Copy setarr to the perm working array. */
  size = elems;
  n = size-1;
  printf("n = %d\n", n);
#ifndef NOMALLOC
  a = malloc(size * sizeof(int));
#endif
  for (i = 0; i < size; i++) {
#ifdef NOARRIF
    a[i] = values[i];
#else
    a[i] = setarr[i];
#endif
  }

  /* P1. [Initialize.] Set cj <- 0 and oj <- 1 for 1 <= j <= n. */
#ifndef NOMALLOC
  c = malloc(size * sizeof(int));
  o = malloc(size * sizeof(int));
#endif
  for (j = 1; j <= size; j++) {
    c[j-1] = 0;
    o[j-1] = 1;
  }

  /* P2. [Visit.] Visit the permutation a1a2...an. */
P2:
#ifndef NOPRINT
  print_perm(file_o, a, size, 1);
#endif
  perm_cnt++;

  /* P3. [Prepare for change.] Set j <- n and s <- 0. (The following steps
   * determine the coordinate j for which cj is about to change, preserving
   * equation (1); variable s is the number of indices k > j such that
   * ck = k - 1.)
   */
  j = n;
  s = 0;

  /* P4. [Ready to change?] Set q <- cj + oj. If q < 0, go to P7; if q = j, go
   * to P6.
   */
P4:
  q = c[j] + o[j];
  if (q < 0) {
    goto P7;
  }
  if (q == j) {
    goto P6;
  }

  /* P5. [Change.] Interchange a_{j-cj+s} <-> a_{j-q+s}. The set cj <- q and
   * return to P2.
   */
  SWAP(a[j-c[j]+s], a[j-q+s]);
  c[j] = q;
  goto P2;

  /* P6. [Increase s.] Terminate if j = 1; otherwise set s <- s + 1. */
P6:
  if (j == 1) {
//    free(a);
//    free(c);
//    free(o);
    return;
  } else {
    s = s + 1;
  }

  /* P7. [Switch direction.] Set oj <- -oj, j <- j - 1, and go back to P4.
   * This procedure works for all n >= 1.
   */
P7:
  o[j] = -o[j];
  j = j - 1;
  goto P4;
}

/* TAOCP_V4F2B_C:
 * Generate all permutations of a set using the cyclic shifts method.
 * This algorithm visits all permutations a1...an of the distinct elements
 * {x1,...,xn}.
 * The algorithm proceeds as follows:
 * C1. [Initialize.] Set aj <- xj for 1 <= j <= n.
 * C2. [Visit.] Visit the permutation a1...an, and set k <- n.
 * C3. [Shift.] Replace a1a2...ak by the cyclic shift a2...aka1, and return
 * to C2 if ak != xk.
 * C4. [Decrease k.] Set k <- k - 1, and go back to C3 if k > 1.
 *
 * Based on Donald E. Knuth, "The Art of Computer Programming, volume 4,
 * fascicle 2B: Algorithm C - Permutation generation by cyclic shifts".
 */
#ifdef NOARRIF
void TAOCP_V4F2B_C(int elems)
#else
void TAOCP_V4F2B_C(int *setarr, int elems)
#endif
{
#ifdef NOMALLOC
  int a[DIM], x[DIM];
#else
  int *a, *x;
#endif
  int i, j, k, n, t;
  int size;

  /* C0. Copy setarr to x. */
  size = elems;
  n = size-1;
  printf("n = %d\n", n);
#ifndef NOMALLOC
  x = malloc(size * sizeof(int));
#endif
  for (i = 0; i < size; i++) {
#ifdef NOARRIF
    x[i] = values[i];
#else
    x[i] = setarr[i];
#endif
  }

  /* C1. [Initialize.] Set aj <- xj for 1 <= j <= n. */
#ifndef NOMALLOC
  a = malloc(size * sizeof(int));
#endif
  for (j = 0; j < size; j++) {
    a[j] = x[j];
  }

  /* C2. [Visit.] Visit the permutation a1...an, and set k <- n. */
C2:
#ifndef NOPRINT
  print_perm(file_o, a, size, 1);
#endif
  k = n;
  perm_cnt++;

  /* C3. [Shift.] Replace a1a2...ak by the cyclic shift a2...aka1, and return
   * to C2 if ak != xk.
   */
C3:
  t = a[1];
  for (i = 1; i < k; i++) {
    a[i] = a[i+1];
  }
  a[k] = t;
  if (a[k] != x[k]) {
    goto C2;
  }

  /* C4. [Decrease k.] Set k <- k - 1, and go back to C3 if k > 1. */
  k = k - 1;
  if (k > 1) {
    goto C3;
  }
}

/* TAOCP_V4F2B_E:
 * Generate all permutations of a set using Ehrlich's swap method.
 * This algorithm generates all permutations of the distinct elements a0..an-1
 * by using auxiliary tables b0..b_{n-1} and c1...c_{n}.
 * The algorithm proceeds as follows:
 * E1. [Initialize.] Set bj <- j and c_{j+1} <- 0 for 0 <= j < n.
 * E2. [Visit.] Visit the permutation a0...a_{n-1}.
 * E3. [Find k.] Set k <- 1. Then if ck = k, set ck <- 0, k <- k + 1, and
 * repeat until ck < j. Terminate if k = n, otherwise set ck <- ck + 1.
 * E4. [Swap.] Interchange a0 <-> a_{bk}.
 * E5. [Flip.] Set j <- 1, k <- k - 1. If j < k, interchange bj <-> bk, set
 * j <- j + 1, k <- k - 1, and repeat until j >= k. Return to E2.
 *
 * Based on Donald E. Knuth, "The Art of Computer Programming, volume 4,
 * fascicle 2B: Algorithm E - Ehrlich swaps".
 */
#ifdef NOARRIF
void TAOCP_V4F2B_E(int elems)
#else
void TAOCP_V4F2B_E(int *setarr, int elems)
#endif
{
#ifdef NOMALLOC
  int a[DIM], b[DIM], c[DIM+1];
#else
  int *a, *b, *c;
#endif
  int i, j, k, n;
  int size;

  /* E0: Copy setarr to the perm working array. */
  size = elems;
  n = size-1;
  printf("n = %d\n", n);
#ifndef NOMALLOC
  a = malloc(n * sizeof(int));
#endif
  for (i = 0; i < n; i++) {
#ifdef NOARRIF
    a[i] = values[i+1];
#else
    a[i] = setarr[i+1];
#endif
  }

  /* E1. [Initialize.] Set bj <- j and c_{j+1} <- 0 for 0 <= j < n. */
#ifndef NOMALLOC
  b = malloc(n * sizeof(int));
  c = malloc(size * sizeof(int));
#endif
  for (j = 0; j < n; j++) {
    b[j] = j;
  }
  c[0] = 0;
  for (j = 0; j < n; j++) {
    c[j+1] = 0;
  }

  /* E2. [Visit.] Visit the permutation a0...a_{n-1}. */
E2:
#ifndef NOPRINT
  print_perm(file_o, a, n, 0);
#endif
  perm_cnt++;

  /* E3. [Find k.] Set k <- 1. Then if ck = k, set ck <- 0, k <- k + 1, and
   * repeat until ck < j. Terminate if k = n, otherwise set ck <- ck + 1.
   */
  k = 1;
  if (c[k] == k) {
    do {
      c[k] = 0;
      k = k + 1;
    } while (c[k] >= k);
  }
  if (k == n) {
    return;
  } else {
    c[k] = c[k] + 1;
  }

  /* E4. [Swap.] Interchange a0 <-> a_{bk}. */
  SWAP(a[0], a[b[k]]);

  /* E5. [Flip.] Set j <- 1, k <- k - 1. If j < k, interchange bj <-> bk, set
   * j <- j + 1, k <- k - 1, and repeat until j >= k. Return to E2.
   */
  j = 1;
  k = k - 1;
  while (j < k) {
    SWAP(b[j], b[k]);
    j = j + 1;
    k = k - 1;
  }
  goto E2;
}

/* print_usage:
 * Print usage instructions for the "permgen" program.
 */
static void print_usage(void)
{
  printf("\n");
  printf("* Usage:\n");
  printf("* permgen [options]\n");
  printf("* Example: ./permgen -l -i test1.gap -o perms.txt\n");
  printf("* \n");
  printf("* Options:\n");
  printf("*   -h:           Print this help.\n");
  printf("*   -l:           Use algorithm L (lexicographic permutation generation).\n");
  printf("*                 This is the default generator.\n");
  printf("*   -p:           Use algorithm P (plain changes method).\n");
  printf("*   -c:           Use algorithm C (permutation generation by cyclic shifts).\n");
  printf("*   -e:           Use algorithm E (permutation generation by Ehrlich swaps).\n");
  printf("*   -i <file>:    Specify input file with set/multiset elements.\n");
  printf("*   -o <file>:    Specify output file.\n");
  printf("*   -n <num>:     Specify number of increasing elements in a set.\n");
  printf("*                 This option is used if no input file is specified.\n");
  printf("* \n");
  printf("* For further information, please refer to the website:\n");
  printf("* http://www.nkavvadias.com\n\n");
}

/* main:
 * Entry point for the "permgen" program.
 */
int main(int argc, char **argv)
{
  char *file_i_name, *file_o_name;
#ifndef NOMALLOC
  int *values = NULL;
#endif
  int nvals = 1;
  int copied_file_i_name=0, copied_file_o_name=0;
  int i, j;
  int t;

  /* Command-line argument passing. */
  if (argc < 2) {
    print_usage();
    return 0;
  }

  for (i = 1; i < argc; i++) {
    if (strcmp("-h", argv[i]) == 0) {
      print_usage();
      exit(1);
    } else if (strcmp("-l", argv[i]) == 0) {
      enable_algo_l = 1;
      enable_algo_p = 0;
      enable_algo_c = 0;
      enable_algo_e = 0;
    } else if (strcmp("-p", argv[i]) == 0) {
      enable_algo_l = 0;
      enable_algo_p = 1;
      enable_algo_c = 0;
      enable_algo_e = 0;
    } else if (strcmp("-c", argv[i]) == 0) {
      enable_algo_l = 0;
      enable_algo_p = 0;
      enable_algo_c = 1;
      enable_algo_e = 0;
    } else if (strcmp("-e", argv[i]) == 0) {
      enable_algo_l = 0;
      enable_algo_p = 0;
      enable_algo_c = 0;
      enable_algo_e = 1;
    } else if (strcmp("-n", argv[i]) == 0) {
      enable_incremental_set = 1;
      if ((i+1) < argc) {
        i++;
        nvals = atoi(argv[i]);
      }
#ifndef NOMALLOC
      values = malloc((nvals+1) * sizeof(int));
#endif
      for (j = 0; j <= nvals; j++) {
        values[j] = j;
        printf("values[%d] = %d\n", values[j], j);
      }
    } else if (strcmp("-i", argv[i]) == 0) {
      if ((i+1) < argc) {
        i++;
        file_i_name = malloc((strlen(argv[i])+1) * sizeof(char));
        strcpy(file_i_name, argv[i]);
        copied_file_i_name = 1;
      }
    } else if (strcmp("-o", argv[i]) == 0) {
      if ((i+1) < argc) {
        i++;
        file_o_name = malloc((strlen(argv[i])+1) * sizeof(char));
        strcpy(file_o_name, argv[i]);
        copied_file_o_name = 1;
      }
    }
    else
    {
      fprintf(stderr, "Warning: Unrecognized command-line option.\n");
    }
  }

  if (copied_file_o_name == 1) {
    if ((file_o = fopen(file_o_name, "w")) == NULL) {
      fprintf(stderr, "Error: Can't create the specified output file.\n");
      exit(1);
    }
  }

  if (copied_file_i_name == 1) {
    if ((file_i = fopen(file_i_name, "r")) == NULL) {
      fprintf(stderr, "Error: Can't read the specified input file.\n");
      exit(1);
    }
  }

  /* Read input file contents. */
  if (copied_file_i_name == 1) {
    i = 0;
    while (!feof(file_i)) {
      if (fscanf(file_i, "%d", &t)){}
      i = i + 1;
    }
    nvals = i;
    /**/
    rewind(file_i);
#ifndef NOMALLOC
    values = malloc((nvals+1) * sizeof(int));
#endif
    values[0] = 0;
    i = 1;
    while (!feof(file_i)) {
      if (fscanf(file_i, "%d", &t)){}
      values[i] = t;
      i = i + 1;
    }
  }

  /* Call the selected permutation generator. */
  if (enable_algo_l == 1) {
#ifdef NOARRIF
    TAOCP_V4F2B_L(nvals+1);
#else
    TAOCP_V4F2B_L(values, nvals+1);
#endif
  } else if (enable_algo_p == 1) {
#ifdef NOARRIF
    TAOCP_V4F2B_P(nvals+1);
#else
    TAOCP_V4F2B_P(values, nvals+1);
#endif
  } else if (enable_algo_c == 1) {
#ifdef NOARRIF
    TAOCP_V4F2B_C(nvals+1);
#else
    TAOCP_V4F2B_C(values, nvals+1);
#endif
  } else if (enable_algo_e == 1) {
#ifdef NOARRIF
    TAOCP_V4F2B_E(nvals+1);
#else
    TAOCP_V4F2B_E(values, nvals+1);
#endif
  }

  printf("Number of permutations: %d\n", perm_cnt);

  if (copied_file_i_name == 1) {
    fclose(file_i);
    free(file_i_name);
  }
  if (copied_file_o_name == 1) {
    fclose(file_o);
    free(file_o_name);
  }
#ifndef NOMALLOC
  free(values);
#endif

  /* Exit the program. */
  return (0);
}
