# Code taken from https://github.com/adafruit/Adafruit_Learning_System_Guides/blob/master/CircuitPython_RGBMatrix/life.py
# Modified from original that has MIT license.

#MIT License
#
#Copyright (c) 2018 Adafruit Industries
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

import random
import time

import board
import displayio
import framebufferio
import rgbmatrix

displayio.release_displays()

# Conway's "Game of Life" is played on a grid with simple rules, based
# on the number of filled neighbors each cell has and whether the cell itself
# is filled.
#   * If the cell is filled, and 2 or 3 neighbors are filled, the cell stays
#     filled
#   * If the cell is empty, and exactly 3 neighbors are filled, a new cell
#     becomes filled
#   * Otherwise, the cell becomes or remains empty
#
# The complicated way that the "m1" (minus 1) and "p1" (plus one) offsets are
# calculated is due to the way the grid "wraps around", with the left and right
# sides being connected, as well as the top and bottom sides being connected.
#
# This function has been somewhat optimized, so that when it indexes the bitmap
# a single number [x + width * y] is used instead of indexing with [x, y].
# This makes the animation run faster with some loss of clarity. More
# optimizations are probably possible.

def apply_life_rule(old_board, new_board, pal):
    old = old_board[0]
    old_occupant = old_board[1]
    new = new_board[0]
    new_occupant = new_board[1]
    width = old.width
    height = old.height
    for y in range(height):
        yyy = y * width
        ym1 = ((y + height - 1) % height) * width
        yp1 = ((y + 1) % height) * width
        xm1 = width - 1
        for x in range(width):
            xp1 = (x + 1) % width
            neighbors = (
                old_occupant[xm1 + ym1] + old_occupant[xm1 + yyy] + old_occupant[xm1 + yp1] +
                old_occupant[x   + ym1] +                  old_occupant[x   + yp1] +
                old_occupant[xp1 + ym1] + old_occupant[xp1 + yyy] + old_occupant[xp1 + yp1])
            new_occupant[x+yyy] = neighbors == 3 or (neighbors == 2 and old_occupant[x+yyy])
            if new_occupant[x+yyy]:
                #new[x+yyy] = 1 + int(x / int(width / (len(pal) - 1)))
                #new[x+yyy] = random.choice(range(1, len(pal)))
                new[x+yyy] = old[x+yyy] + 1
                if new[x+yyy] >= len(pal):
                    new[x+yyy] = len(pal) - 1
            else:
                new[x+yyy] = 0
            xm1 = x

# Fill 'fraction' out of all the cells.
def randomize(output, fraction=0.33):
    for i in range(output[0].height * output[0].width):
        output[0][i] = random.random() < fraction
        output[1][i] = output[0][i] == 1


# Fill the grid with a tribute to John Conway
def conway(output):
    # based on xkcd's tribute to John Conway (1937-2020) https://xkcd.com/2293/
    conway_data = [
        b'  +++   ',
        b'  + +   ',
        b'  + +   ',
        b'   +    ',
        b'+ +++   ',
        b' + + +  ',
        b'   +  + ',
        b'  + +   ',
        b'  + +   ',
    ]
    for i in range(output[0].height * output[0].width):
        output[0][i] = 0
        output[1][i] = False
    for i, si in enumerate(conway_data):
        y = output[0].height - len(conway_data) - 2 + i
        for j, cj in enumerate(si):
            output[0][(output[0].width - 8)//2 + j, y] = cj & 1
            output[1][(output[0].width - 8)//2 + j + y * output[0].width] = cj & 1 == 1

# bit_depth=1 is used here because we only use primary colors, and it makes
# the animation run a bit faster because RGBMatrix isn't taking over the CPU
# as often.
matrix = rgbmatrix.RGBMatrix(
    width=64, height=32, bit_depth=4,
    rgb_pins=[board.MTX_R1,board.MTX_G1,board.MTX_B1,board.MTX_R2,board.MTX_G2,board.MTX_B2,],
    addr_pins=[board.MTX_ADDRA, board.MTX_ADDRB, board.MTX_ADDRC, board.MTX_ADDRD],
    clock_pin=board.MTX_CLK, latch_pin=board.MTX_LAT, output_enable_pin=board.MTX_OE)
display = framebufferio.FramebufferDisplay(matrix, auto_refresh=False)
SCALE = 1
palette = displayio.Palette(8)
b1 = displayio.Bitmap(display.width//SCALE, display.height//SCALE, len(palette))
b2 = displayio.Bitmap(display.width//SCALE, display.height//SCALE, len(palette))
tg1 = displayio.TileGrid(b1, pixel_shader=palette)
tg2 = displayio.TileGrid(b2, pixel_shader=palette)
g1 = displayio.Group(max_size=3, scale=SCALE)
g1.append(tg1)
display.show(g1)
g2 = displayio.Group(max_size=3, scale=SCALE)
g2.append(tg2)

board1 = [b1, []]
board2 = [b2, []]
for idx in range(b1.width * b1.height):
    board1[1].append(False)
    board2[1].append(False)

# First time, show the Conway tribute
for idx in range(1, len(palette)):
    palette[idx] = 0xffffff
conway(board1)
display.auto_refresh = True
time.sleep(3)
n = 5#40

while True:
    # run 2*n generations.
    # For the Conway tribute on 64x32, 80 frames is appropriate.  For random
    # values, 400 frames seems like a good number.  Working in this way, with
    # two bitmaps, reduces copying data and makes the animation a bit faster
    for _ in range(n):
        display.show(g1)
        apply_life_rule(board1, board2, palette)
        display.show(g2)
        apply_life_rule(board2, board1, palette)

    # After 2*n generations, fill the board with random values and
    # start over with a new color.
    randomize(board1)
    # Pick a random color out of 6 primary colors or white.
    colors = [  0xFF0000, 0x00FF00, 0x0000FF, 0xFF00FF, 0xFFFF00, 0x00FFFF, 0xFFFFFF,
                0x59981A, 0xDBA40E, 0x21B6A8, 0xA3EBB1, 0x18A558, 0xFF2768, 0xFB6090,
                0xFFC5D0, 0xAE388B, 0xF1C0B9 ]
    for idx in range(1, len(palette)):
        palette[idx] = random.choice(colors)
        colors.remove(palette[idx])
        #palette[idx] = ((random.choice(range(100, 250)) << 16) & 0xFF0000) | ((random.choice(range(100, 250)) << 8) & 0x00FF00) | (random.choice(range(100, 250)) & 0x0000FF)
        #(
        #    (0x0000ff if random.random() > .33 else 0) |
        #    (0x00ff00 if random.random() > .33 else 0) |
        #    (0xff0000 if random.random() > .33 else 0)) or 0xffffff
    n = 100