# This does not run after 0x7c00 offset.
# ./dax86 tests/mov_jmp/mov_jmp.bin

ORG_JMP="tests/org_jmp/org_jmp.bin"
MODRM="tests/modrm/modrm.bin"
CALL="tests/call/call.bin"
MAIN_FUNC="tests/main_func/main_func.bin"
ARGS="tests/args/args.bin"
IF_TEST="tests/if/if.bin"

for i in $ORG_JMP $MODRM $CALL $MAIN_FUNC $ARGS $IF_TEST
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