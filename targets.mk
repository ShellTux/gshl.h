TARGETS = \
					$(TARGET_DIR)/libgshl.so \
					$(TARGET_DIR)/libgshl.a \
					$(TARGET_DIR)/gshl-test \
					$(TARGET_DIR)/examples/macros \

DEPS :=
$(TARGET_DIR)/libgshl.so: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

DEPS :=
$(TARGET_DIR)/libgshl.a: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	ar rcs $@ $^

DEPS := test/all.c
$(TARGET_DIR)/gshl-test: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)

DEPS := examples/macros.c
$(TARGET_DIR)/examples/macros: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -o $@ $^ $(LDFLAGS)
