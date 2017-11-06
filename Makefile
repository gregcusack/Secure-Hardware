APP = arm-mb-comm

# Add any other object files to this list below
APP_OBJS = arm-mb-comm.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

