movz w0, #0x3f20, lsl #16

movz w1, #0x001001000
str w1, [w0]

movz w1, #0x1c
movz w2, #0x28

orr w0, w0, w1
orr w1, w0, w2

movz w2, #2

movz w4, #1
movz w5, #100000


on:
    str w4, [w0, w2]
    sub w5, w5, #1
    cmp w5, #0
    b.eq on_to_off
    b on

off:
    str w4, [w1, w2]
    sub w5, w5, #1
    cmp w5, #0
    b.eq off_to_on
    b off

on_to_off:
    movz w5, #100000
    b off

off_to_on:
    movz w5, #100000
    b on

and x0, x0, x0

