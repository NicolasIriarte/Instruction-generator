## Dependencies

Note about install commands:
- for MacOS, we use [brew](https://brew.sh/).
- In case of an error in cmake, make sure that the dependencies are on the PATH.


### Too Long, Didn't Install

This is a really long list of dependencies, and it's easy to mess up. That's why:

#### Docker
We have a Docker image that's already set up for you. See the [Docker instructions](#docker-instructions).

#### Setup-cpp

We have [setup-cpp](https://github.com/aminya/setup-cpp) that is a cross-platform tool to install all the compilers and dependencies on the system.

Please check [the setup-cpp documentation](https://github.com/aminya/setup-cpp) for more information.

### Necessary Dependencies
1. A C++ compiler that supports C++17.
See [cppreference.com](https://en.cppreference.com/w/cpp/compiler_support)
to see which features are supported by each compiler.
The following compilers should work:

  * [gcc 7+](https://gcc.gnu.org/)
	<details>
	<summary>Install command</summary>

	- Debian/Ubuntu:

			sudo apt install build-essential

	- MacOS:

			brew install gcc
	</details>

  * [clang 6+](https://clang.llvm.org/)
	<details>
	<summary>Install command</summary>

	- Debian/Ubuntu:

			bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"


	- MacOS:

			brew install llvm
	</details>


2. [CMake 3.21+](https://cmake.org/)
	<details>
	<summary>Install Command</summary>

	- Debian/Ubuntu:

			sudo apt-get install cmake

	- MacOS:

			brew install cmake

	</details>

3. [LLVM](llvm.org/)
	<details>
	<summary>Install Command</summary>

	- Debian/Ubuntu:

			sudo apt-get install llvm

    - MacOS:

			brew install llvm

	</details>

### Optional Dependencies
#### C++ Tools
  * [Doxygen](http://doxygen.nl/)
	<details>
	<summary>Install Command</summary>

	- Debian/Ubuntu:

			sudo apt-get install doxygen
			sudo apt-get install graphviz

	- MacOS:

			brew install doxygen
	 		brew install graphviz

	</details>


  * [ccache](https://ccache.dev/)
	<details>
	<summary>Install Command</summary>

	- Debian/Ubuntu:

			sudo apt-get install ccache

	- MacOS:

			brew install ccache

	</details>


  * [Cppcheck](http://cppcheck.sourceforge.net/)
	<details>
	<summary>Install Command</summary>

	- Debian/Ubuntu:

			sudo apt-get install cppcheck

	- MacOS:

			brew install cppcheck

	</details>


  * [include-what-you-use](https://include-what-you-use.org/)
	<details>
	<summary>Install Command</summary>

	Follow instructions here:
	https://github.com/include-what-you-use/include-what-you-use#how-to-install
	</details>
