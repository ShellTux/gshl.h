TARGETS = \
					$(TARGET_DIR)/gshl-test \
					$(TARGET_DIR)/examples/dynamic-array \
					$(TARGET_DIR)/examples/hash-table \
					$(TARGET_DIR)/examples/macros \
					$(TARGET_DIR)/examples/print \
					$(TARGET_DIR)/libgshl.so \
					$(TARGET_DIR)/libgshl.a

.PHONY: benchmark
benchmark: \
	$(TARGET_DIR)/benches/print.flamegraph.svg \
	$(TARGET_DIR)/benches/hash-table.flamegraph.svg

DEPS := $(SRCS)
$(TARGET_DIR)/libgshl.so: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

DEPS := $(SRCS)
$(TARGET_DIR)/libgshl.a: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	ar rcsv $@ $^

DEPS := test/all.c
$(TARGET_DIR)/gshl-test: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS) -lm

DEPS := examples/macros.c
$(TARGET_DIR)/examples/macros: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)

DEPS := examples/print.c
$(TARGET_DIR)/examples/print: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)

DEPS := examples/dynamic-array.c
$(TARGET_DIR)/examples/dynamic-array: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)

DEPS := examples/hash-table.c
$(TARGET_DIR)/examples/hash-table: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)

DEPS := benches/print.c
$(TARGET_DIR)/benches/print: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -pg -o $@ $^ $(LDFLAGS) -lm

DEPS := benches/hash-table.c
$(TARGET_DIR)/benches/hash-table: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -pg -o $@ $^ $(LDFLAGS) -lm

%.flamegraph.svg: %
	flamegraph --output $@ -- $< >/dev/null
