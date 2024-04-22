#pragma once

#define _GIO_INLINE static inline __attribute__((always_inline))
#define greg_set(reg, mask) *reg |= mask
#define greg_clr(reg, mask) *reg &= ~mask
#define greg_write(reg, mask, val) (val) ? greg_set(reg, mask) : greg_clr(reg, mask)
#define greg_read(reg, mask) (bool)(*reg & mask)