
# Modify this to point to the PJSIP location.
PJBASE=/home/mres/skola/pjproject-2.4.5

include $(PJBASE)/build.mak

CC      = $(APP_CC)
LDFLAGS = $(APP_LDFLAGS)
LDLIBS  = $(APP_LDLIBS)
#CFLAGS  = $(APP_CFLAGS)
CPPFLAGS= ${CFLAGS}

# If your application is in a file named myapp.cpp or myapp.c
# this is the line you will need to build the binary.
all: main

myapp: main.c $(CC) -o $@ $<  $(LDFLAGS) $(LDLIBS)

clean: rm -f main.o main

# Modify this to point to the PJSIP location.
# PJBASE=/home/mres/skola/pjproject-2.4.5

# include $(PJBASE)/build.mak

# CC=$(CROSS_COMPILE)$(CC_NAME)

# # Remove components that you don't need from the following definitions.
# LDFLAGS=-L${PJBASE}/pjlib/lib\
#     -L${PJBASE}/pjlib-util/lib\
#     -L${PJBASE}/pjmedia/lib\
#     -L${PJBASE}/pjsip/lib
#  LDLIBS=-lpjsua-${TARGET_NAME}\
#     -lpjsip-ua-${TARGET_NAME}\
#     -lpjsip-simple-${TARGET_NAME}\
#     -lpjsip-${TARGET_NAME}\
#     -lpjmedia-codec-${TARGET_NAME}\
#     -lpjmedia-${TARGET_NAME}\
#     -lpjmedia-codec-${TARGET_NAME}\
#     -lpjlib-util-${TARGET_NAME}\
#     -lpj-${TARGET_NAME}\
# 	-lm\
# 	-lpthread\
# 	-lasound\
# 	-lssl
# CFLAGS=-I${PJBASE}/pjlib/include\
#     -I${PJBASE}/pjlib-util/include\
#     -I${PJBASE}/pjmedia/include\
#     -I${PJBASE}/pjsip/include\
#     -DPJ_AUTOCONF=1
# CPPFLAGS=${CFLAGS}

# # If your application is in a file named myapp.cpp or myapp.c
# # this is the line you will need to build the binary.
# all: main

# myapp: main.c $(CC) -o $@ $<  $(LDFLAGS) $(LDLIBS)
# clean: rm -f main.o main