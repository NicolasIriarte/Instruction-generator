# Create a gcc debug compilation.
gcc-debug:
    #!/usr/bin/env bash
    set -e

    # Generate cmake build directory
    if [[ ! -d  build/gcc-debug ]]; then
        cmake --preset=gcc-debug
    fi

    # Generate correct compile_commands.json
    if [[ -f compile_commands.json || -L compile_commands.json ]]; then
        rm -rf compile_commands.json
    fi
    ln -s build/gcc-debug/compile_commands.json .

    # Compilation and test
    cd build/gcc-debug
    make -j$(expr $(nproc) - 1)
    ctest --output-junit gcc-junit.xml

# Create a clang debug compilation.
clang-debug:
    #!/usr/bin/env bash
    set -e
    # Generate cmake build directory
    if [[ ! -d  build/clang-debug ]]; then
        cmake --preset=clang-debug
    fi

    # Generate correct compile_commands.json
    if [[ -f compile_commands.json || -L compile_commands.json ]]; then
        rm -rf compile_commands.json
    fi
    ln -s build/clang-debug/compile_commands.json .

    # Compilation and test
    cd build/clang-debug
    make -j$(expr $(nproc) - 1)
    ctest --output-junit clang-junit.xml

# Create a gcc release compilation.
gcc-release:
    #!/usr/bin/env bash
    set -e
    # Generate cmake build directory
    if [[ ! -d  build/gcc-release ]]; then
        cmake --preset=gcc-release
    fi

    # Generate correct compile_commands.json
    if [[ -f compile_commands.json || -L compile_commands.json ]]; then
        rm -rf compile_commands.json
    fi
    ln -s build/gcc-release/compile_commands.json .

    # Compilation and test
    cd build/gcc-release
    make -j$(expr $(nproc) - 1)

# Create a clang release compilation.
clang-release:
    #!/usr/bin/env bash
    set -e
    # Generate cmake build directory
    if [[ ! -d  build/clang-release ]]; then
        cmake --preset=clang-release
    fi

    # Generate correct compile_commands.json
    if [[ -f compile_commands.json || -L compile_commands.json ]]; then
        rm -rf compile_commands.json
    fi
    ln -s build/clang-release/compile_commands.json .

    # Compilation and test
    cd build/clang-release
    make -j$(expr $(nproc) - 1)

# Install gcc release version.
gcc-install:
    #!/usr/bin/env bash
    set -e
    if [[ ! -d  build/gcc-release ]]; then
        cmake --preset=gcc-release
    fi

    cd build/gcc-release
    make -j$(expr $(nproc) - 1) install

# Install clang release version.
clang-install:
    #!/usr/bin/env bash
    set -e
    if [[ ! -d  build/clang-release ]]; then
        cmake --preset=clang-release
    fi

    cd build/clang-release
    make -j$(expr $(nproc) - 1) install

# Generate gcc release installable package.
gcc-installable:
    #!/usr/bin/env bash
    set -e
    if [[ ! -d  build/gcc-release ]]; then
        cmake --preset=gcc-release
    fi

    cd build/gcc-release
    make -j$(expr $(nproc) - 1) package

# Generate clang release installable package.
clang-installable:
    #!/usr/bin/env bash
    set -e
    if [[ ! -d  build/clang-release ]]; then
        cmake --preset=clang-release
    fi

    cd build/clang-release
    make -j$(expr $(nproc) - 1) package


# Generate coverage file.
coverage:
    #!/usr/bin/env bash
    set -e
    rm -rf build/*.xml build/*.html
    if [[ ! -d  build/gcc-debug ]]; then
        cmake --preset=gcc-debug
    fi

    # Generate correct compile_commands.json
    if [[ -f compile_commands.json || -L compile_commands.json ]]; then
        rm -rf compile_commands.json
    fi
    ln -s build/gcc-debug/compile_commands.json .

    # Compile and test.
    cd build/gcc-debug
    make -j$(expr $(nproc) - 1)
    ctest
    cd {{justfile_directory()}}
    cd build
    gcovr --xml-pretty --html index.html --exclude-unreachable-branches --print-summary -o coverage.xml --root {{justfile_directory()}}

# Generate documentation page.
pages:
    cmake --preset=doxy-doc
    cmake --build build/doxy-doc --target doxygen-docs
    mv build/doxy-doc/html public/

# Removes all generated directories, files and cache.
clean:
    echo "Removing previous compilations."
    rm -rf build
    echo "Removing cache."
    rm -rf .cache
    echo "Removing generated documentation."
    rm -rf public
    echo "Removing previous installs."
    rm -rf install
    echo "Removing 'compile_commands.json' file."
    rm -rf compile_commands.json
    echo "Done."
