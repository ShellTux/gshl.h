TARGETS = \
					$(TARGET_DIR)/gshl-test \
					$(TARGET_DIR)/examples/macros \
					$(TARGET_DIR)/examples/print \
					$(TARGET_DIR)/benches/print \
					$(TARGET_DIR)/benches/print.flamegraph.svg \
					$(TARGET_DIR)/libgshl.so \
					$(TARGET_DIR)/libgshl.a

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

DEPS := benches/print.c
$(TARGET_DIR)/benches/print: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -pg -o $@ $^ $(LDFLAGS) -lm

%.flamegraph.svg: %
	flamegraph --output $@ -- $< >/dev/null
