CFLAGS += -DGSHL_DEBUG
CFLAGS += -ggdb

CXXFLAGS += -DGSHL_DEBUG
CXXFLAGS += -ggdb

LDFLAGS += -fsanitize=address
LDFLAGS += -fsanitize=leak

BUILD_DIR = build/debug
TARGET_DIR = target/debug
