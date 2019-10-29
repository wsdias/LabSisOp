#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

#cp $BASE_DIR/../custom-scripts/S42pythonServer $BASE_DIR/target/etc/init.d
#chmod +x $BASE_DIR/target/etc/init.d/S42pythonServer

#cp $BASE_DIR/../custom-scripts/simple_http_server.py $BASE_DIR/target/usr/bin

#Compile the syscall_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_test $BUILDROOT_DIR/custom-scripts/syscall_test.c

#Compile the syscall_sleep_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_sleep_test $BUILDROOT_DIR/custom-scripts/syscall_sleep_test.c

# Compile new driver
make -C $BASE_DIR/../modules/simple_driver/

make -C $BASE_DIR/../modules/simple_driver_modified/

# FTrace
cp $BASE_DIR/../custom-scripts/fstab $BASE_DIR/target/etc
