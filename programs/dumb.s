movz w0, #0x3f20, lsl #16

movz w1, #0x1249
str w1, [w0]

movz w1, #0x1c
movz w2, #0x28

orr w0, w0, w1
orr w1, w0, w2

movz w4, #0xffff
movz w6, #0
movz w5, #0xffff, lsl #16

off:
    str w6, [w0]
    str w4, [w1]
    sub w5, w5, #0x1
    cmp w5, #0
    b.le offToOn
    b off

on:
    str w4, [w0]
    str w6, [w1]
    sub w5, w5, #0x1
    cmp w5, #0
    b.le onToOff
    b on

onToOff:
    movz w5, #0xffff, lsl #16
    b off

offToOn:
    movz w5, #0xffff, lsl #16
    b on

