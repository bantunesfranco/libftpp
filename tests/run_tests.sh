#!/bin/bash

mkdir -p objs bin

# Compile all source files to object files
for file in ../srcs/*/*.cpp; do
    g++ -c "$file" -I../incs -o "objs/$(basename "${file%.cpp}.o")" || { echo "Build failed: $file"; exit 1; }
done

# Compile all test files to object files
for file in *.cpp; do
    g++ -c "$file" -I../incs -o "objs/$(basename "${file%.cpp}.o")" || { echo "Build failed: $file"; exit 1; }
done

# Link each test object with all source objects and run
FAILED=0
for testobj in objs/*_test.o; do
    exe="bin/$(basename "${testobj%.o}")"
    g++ "$testobj" objs/*.o -o "$exe" || { echo "Link failed: $exe"; exit 1; }
    if ! "$exe"; then
        echo "Test failed: $exe"
		FAILED=$(($FAILED + 1))
    fi
done

if [[ $FAILED -eq 0 ]]; then
	echo "All builds and tests passed."
else
	echo "$FAILED tests have failed"
fi