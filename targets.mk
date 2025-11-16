TARGETS = \
					$(TARGET_DIR)/libgshl.so \
					$(TARGET_DIR)/libgshl.a

DEPS :=
$(TARGET_DIR)/libgshl.so: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

DEPS :=
$(TARGET_DIR)/libgshl.a: $(DEPS:%.c=$(BUILD_DIR)/%.o)
	@mkdir --parents $(shell dirname $@)
	ar rcs $@ $^
