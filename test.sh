# This does not run after 0x7c00 offset.
# ./dax86 tests/mov_jmp/mov_jmp.bin

ORG_JMP="tests/org_jmp/org_jmp.bin"
MODRM="tests/modrm/modrm.bin"

for i in $ORG_JMP $MODRM
do

echo
echo "Running test with $i..."
echo
echo "ndisasm $i"
ndisasm $i
echo
echo "./dax86 $i"
./dax86 $i

done