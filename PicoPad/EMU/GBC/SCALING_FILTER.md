# LCD Scaling Filter

The emulator scales the Game Boy's 160×144 image to the 240×240 display
using bilinear interpolation with lookup tables:

- **Horizontal**: three phases per pair of pixels. The fraction values
  repeat in the pattern `0,2,1` representing weights of 0, 2/3 and
  1/3 for the second source pixel.
- **Vertical**: five phases between lines. Fractions follow the pattern
  `0,3,1,4,2`, corresponding to weights of 0, 3/5, 1/5, 4/5 and 2/5 for
  the lower line.

Fractions (0–2 for X, 0–4 for Y) are packed together with source indices
in `lut_x` and `lut_y`. During rendering the precomputed weights are
combined using integer arithmetic (adds, multiplies and shifts) to
minimize CPU load on the RP2040.
