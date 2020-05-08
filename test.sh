# Execution Tests with Binary Files

function run_test() {
  test_dir="./tests/exec/$1"
  test_path="$test_dir/$1.bin"
  expected_path="$test_dir/expected.txt"
  
  echo "------------------------------------------------"
  echo "Building test binary $1..."
  echo
  make -C $test_dir
  echo
  echo "Running test with $1..."
  echo
  echo "ndisasm -b 32 $test_path"
  echo
  ndisasm -b 32 $test_path
  echo
  echo "./dax86 $test_path"
  echo
  output=$(./dax86 $test_path)
  echo "$output"
  echo
  if [[ $output == *"End of program :)"* ]]; then
    echo "[Run: SUCCESS]"
  else
    echo "[Run: FAILURE]"
    exit 1
  fi
  if [ -f "${expected_path}" ]
  then
    echo "[Assertion]"
    line_num=0
    while IFS= read -r line; do
      if [[ $output == *"$line"* ]]; then
        echo "Line ${line_num}: Pass"
      else
        echo "Line ${line_num}: Fail"
        exit 1
      fi
      line_num=$((line_num+1))
    done < "${expected_path}"
  else
    echo "expected.txt Not Found"
  fi
  echo
}

# Files below do not run after 0x7c00 offset:
#
# tests/mov_jmp/mov_jmp.bin
#
# Files below require input:
#
# tests/io/in.bin
# tests/io/in_display.bin
# tests/rep/rep.bin
function test_all() {
  for i in \
  "modrm"\
  "org_jmp"\
  "call"\
  "main_func"\
  "args"\
  "if"\
  "flags_set"\
  "flags_clear"\
  "if"\
  "jmp_c"\
  "io_out"\
  "seg"\
  "or"\
  "sub"\
  "and"\
  "xor"\
  "pusha"\
  "code_80"\
  "test"\
  "xchg"\
  "mov"\
  "pop"\
  "code_c0"\
  "loop"\
  "jmp_far"\
  "call_far"\
  "moffs"\
  "str_8"\
  "str_32"\
  "gdt"\
  "mul"\
  "div"\
  "paging"\
  "disk"\
  "movzx"\
  "lapic"
  do
    run_test $i
  done
}

function main() {
  if [ -z "$1" ]; then
    test_all
  else
    run_test $1
  fi
}

main $1
