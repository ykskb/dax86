# Execution Tests with Binary Files

function echo_ndisasm() {
  echo "ndisasm -b 32 $1"
  echo
  ndisasm -b 32 $1
  echo
}

function echo_output() {
  echo "./dax86 $1"
  echo
  echo "$2"
  echo
}

function run_test() {
  test_dir="./tests/exec/$1"
  test_path="$test_dir/$1.bin"
  expected_path="$test_dir/expected.txt"
  verbose=false

  if [ "$2" == "-v" ]; then
    verbose=true
  fi
  # start of test
  echo "------------------------------------------------"
  echo "Building test binary $1..."
  if [ "$verbose" = true ]; then
    echo
    make -C $test_dir
    echo
  fi
  echo "Running test with $1..."
  output=$(./dax86 $test_path)
  if [[ $output == *"End of program :)"* ]]; then
    echo "[Run: SUCCESS]"
    if [ "$verbose" = true ]; then
      echo
      echo_ndisasm "$test_path"
      echo_output "$test_path" "$output"
    fi
  else
    echo "[Run: FAILURE]"
    echo
    echo_ndisasm "$test_path"
    echo_output "$test_path" "$output"
    exit 1
  fi
  if [ -f "${expected_path}" ]
  then
    echo "[Assertion]"
    line_num=0
    while IFS= read -r line; do
      if [[ $output == *"$line"* ]]; then
        if [ "$verbose" = true ]; then
          echo "Line ${line_num}: Pass"
        fi
      else
        echo "Line ${line_num}: Fail"
        echo
        echo_ndisasm "$test_path"
        echo_output "$test_path" "$output"
        exit 1
      fi
      line_num=$((line_num+1))
    done < "${expected_path}"
    echo "All check passed."
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
#   tests/io/in.bin
#   tests/io/in_display.bin
#   tests/rep/rep.bin
# Binary is more than 512 bytes (1 sector):
#   tests/mp/mp.bin

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
  "lapic"\
  "ioapic"
  do
    run_test $i
  done
}

function main() {
  if [ -z "$1" ]; then
    test_all
  else
    run_test $1 "-v"
  fi
}

main $1
