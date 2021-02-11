## Defenitions
* Depth - number of bits/bytes in each pixel(e.g. 8 bits for green, 8 for red and 8 for blue - depth is 24, so there are 3 bytes for each pixel)
* Pitch - amount of bytes in each row (row_width * bytes_per_pixel(depth))

## Formulas
* **Pitch:** `pitch = row_width * bytes_per_pixel(depth)`
* **Pixels per row:** `pixels_per_row = pitch / (depth / 8)`
* **Pixel offset(location) in a framebuffer:** `offset = pitch * y + (depth / 8) * x`
