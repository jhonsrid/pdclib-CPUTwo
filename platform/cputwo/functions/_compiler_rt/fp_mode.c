/* Floating-point environment stubs for CPUTwo (no FP hardware).
 * Always returns round-to-nearest; inexact is a no-op.
 */

#include "fp_mode.h"

CRT_FE_ROUND_MODE __fe_getround(void) {
  return CRT_FE_TONEAREST;
}

int __fe_raise_inexact(void) {
  return 0;
}
