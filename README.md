# nanopack

The smallest [msgpack](http://msgpack.org/) encoding interface. This is
designed for embedded devices where memory constraints are tight and the output
format is usually static.

For reference, the resulting `.a` archive is ~1.9K when compiling with GCC
and `-Os`.

If you are not on a resource-constrained embedded device, I'd recommend instead
using the [mpack](https://github.com/ludocode/mpack) library.

```
#include "nanopack.h"

int main(void)
{
    size_t len;
    uint8_t buf[64];
    uint8_t *w = buf;

    np_map(&w, 2);
    np_str(&w, "key1");
    np_i32(&w, "value1");

    len = w - buf;

    // msgpack data stored from buf[0..len]
}
```

## Installation

The easiest method to use is to copy the `nanopack.c` and `nanopack.h` files
directly into your project.

It is advisable to run the test suite on your device before using to ensure
that it works as intended as there may be issues on more exotic architectures
(i.e. ones-complement devices).

## Requirements

 - A C89 compiler
 - `stdint.h` for `(u)int(8|16|32|64)_t` types
 - `stddef.h` for `size_t`

If either of the system header files are missing, these can be manually written
with relative ease (~15 lines of code for integer definitions).

## Questions

### Why no decoding?

Isn't required for my purposes. It is also unsuitable for embedded devices
which act as dumb beacons (don't receive external data).

### Buffer Overruns are possible!

This wrapper assumes that the caller knows the limits of the generated data and
will preallocate fixed buffers to be able to hold the full amount.

### How can I check errors?

The library assumes that you call it with the correct arguments and test it
before continuing. If called correctly, there should never be errors to report.

## Remarks

Minor inspiration taken from [mpack](https://github.com/ludocode/mpack)
and [libmpack](https://github.com/tarruda/libmpack) which are both licensed
under the MIT license.
