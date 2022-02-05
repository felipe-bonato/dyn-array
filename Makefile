C_CPP = clang++
C_FLAGS = -Wall -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
C_FLAGS_DEBUG = -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
C_LANG_VERSION = c++20

C_SRC_PATH = src/dyn_array.cpp
C_OUT_BIN_PATH = build/out

dev: $(C_SRC_PATH)
	$(C_CPP) $(C_SRC_PATH) -std=$(C_LANG_VERSION) -O1 $(C_FLAGS) -o $(C_OUT_BIN_PATH)_dev

debug: $(C_SRC_PATH)
	$(C_CPP) $(C_SRC_PATH) -std=$(C_LANG_VERSION) -O0 $(C_FLAGS) -o $(C_OUT_BIN_PATH)_debug $(C_FLAGS_DEBUG) 

release: $(C_SRC_PATH)
	$(C_CPP) $(C_SRC_PATH) -std=$(C_LANG_VERSION) -O3 $(C_FLAGS) -o $(C_OUT_BIN_PATH)_release -Werror 

clean:
	rm build/*