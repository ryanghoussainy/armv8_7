movz w0, #0x3f20, lsl #16

movz w1, #0x40
str w1, [w0]

movz w1, #0x1c
movz w2, #0x28

orr w1, w0, w1
orr w2, w0, w2

movz w3, #0
movz w4, #4

movz w5, #0x003c, lsl #16

off:
    str w3, [w1]
    str w4, [w2]
    sub w5, w5, #1
    tst w5, w5
    b.eq onResetTimer
    b off

on:
    str w3, [w2]
    str w4, [w1]
    sub w5, w5, #1
    tst w5, w5
    b.eq offResetTimer
    b on

onResetTimer:
    movz w5, #0x003c, lsl #16
    b on

offResetTimer:
    movz w5, #0x003c, lsl #16
    b off

