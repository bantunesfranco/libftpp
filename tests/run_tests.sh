#!/bin/bash

mkdir -p objs bin

if [ ! -z $1 ]; then
    TEST_FILES=("$1")
else
    TEST_FILES=($(find . -type f -name '*_test.cpp'))
fi

# Compile all test files to object files
for file in "${TEST_FILES[@]}"; do
    g++ -g -c "$file" -I../incs -o "objs/$(basename "${file%.cpp}.o")" || { echo "Build failed: $file"; exit 1; }
done

# Link each test object with all source objects and run
FAILED=0
for testobj in objs/*_test.o; do
    echo "Running test: $testobj"
    exe="bin/$(basename "${testobj%.o}")"
    g++ -g "$testobj" -lftpp -L.. -o "$exe" || { echo "Link failed: $exe"; exit 1; }
    "$exe" > /dev/null 2>&1
    if [[ $? != 0 ]]; then
        echo "Test failed: $exe"
		FAILED=$(($FAILED + 1))
    fi
done

if [[ $FAILED -eq 0 ]]; then
    rm -rf objs bin
	echo "All builds and tests passed."
else
	echo "$FAILED tests have failed"
fi
