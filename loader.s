.globl start
.globl acpiAddress
.globl memMapAddress
.globl memMapSize
.globl memMapDescriptorSize

.text
start:
    ld.d $t0, $sp, -32
    la.local $t1, acpiAddress
    st.d $t0, $t1, 0

    ld.d $t0, $sp, -24
    la.local $t1, memMapAddress
    st.d $t0, $t1, 0

    ld.d $t0, $sp, -16
    la.local $t1, memMapSize
    st.d $t0, $t1, 0

    ld.d $t0, $sp, -8
    la.local $t1, memMapDescriptorSize
    st.d $t0, $t1, 0

    li.d $sp, 0x200000
    bl kernelMain
.data
acpiAddress:
.quad 0
memMapAddress:
.quad 0
memMapSize:
.quad 0
memMapDescriptorSize:
.quad 0
