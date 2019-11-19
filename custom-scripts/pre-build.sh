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

# Thread Runner
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/thread_runner $BUILDROOT_DIR/t3/thread_runner.c
chmod +x $BASE_DIR/target/bin/thread_runner

#KILLER1
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/killer1 $BUILDROOT_DIR/custom-scripts/killer1.c
chmod +x $BASE_DIR/target/bin/killer1

#KILLER2
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/killer2 $BUILDROOT_DIR/custom-scripts/killer2.c
chmod +x $BASE_DIR/target/bin/killer2

#KILLER3
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/killer3 $BUILDROOT_DIR/custom-scripts/killer3.c
chmod +x $BASE_DIR/target/bin/killer3

# Enderecos
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/enderecos $BUILDROOT_DIR/custom-scripts/enderecos.c
chmod +x $BASE_DIR/target/bin/enderecos

