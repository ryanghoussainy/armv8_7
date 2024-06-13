movz w0, #0x3f20, lsl #16

movz w1, #0x1249
str w1, [w0]

movz w1, #0x1c
movz w2, #0x28

orr w0, w0, w1
orr w1, w0, w2

movz w4, #0xffff
movz w6, #0
movz w5, #10000

on:
    str w6, [w1]
    str w4, [w0]
    sub w5, w5, #1
    cmp w5, #0
    b.eq onToOff
    b on

off:
    str w6, [w0]
    str w4, [w1]
    sub w5, w5, #1
    cmp w5, #0
    b.eq offToOn
    b off

onToOff:
    movz w5, #10000
    b off

offToOn:
    movz w5, #10000
    b on

