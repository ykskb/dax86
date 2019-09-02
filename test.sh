# This does not run after 0x7c00 offset.
# ./dax86 tests/mov_jmp/mov_jmp.bin

ORG_JMP="tests/org_jmp/org_jmp.bin"
MODRM="tests/modrm/modrm.bin"
CALL="tests/call/call.bin"
MAIN_FUNC="tests/main_func/main_func.bin"
ARGS="tests/args/args.bin"
IF_TEST="tests/if/if.bin"
LOOP_TEST="tests/loop/loop.bin"
OUT_TEST="tests/io/out.bin"
SET_FLAGS_TEST="tests/eflags/set_flags.bin"
CLEAR_FLAGS_TEST="tests/eflags/clear_flags.bin"

# Requires Input
IN_TEST="tests/io/in.bin"
IN_DISPLAY_TEST="tests/io/in_display.bin"

for i in $ORG_JMP $MODRM $CALL $MAIN_FUNC $ARGS $IF_TEST $LOOP_TEST $OUT_TEST $CLEAR_FLAGS_TEST $SET_FLAGS_TEST
do

echo
echo "Running test with $i..."
echo
echo "ndisasm -b 32 $i"
ndisasm -b 32 $i
echo
echo "./dax86 $i"
./dax86 $i

done