movz w0, #0x3f20, lsl #16

movz w1, #0x001001000
str w1, [w0]

movz w1, #0x1c
movz w2, #0x28

orr w0, w0, w1
orr w1, w0, w2

movz w2, #2
movz w4, #0

str w4, [w0, w2]

and x0, x0, x0

