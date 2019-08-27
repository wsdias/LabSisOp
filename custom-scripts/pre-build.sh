#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config
cp $BASE_DIR/../custom-scripts/simple_http_server.py $BASE_DIR/target/root
cp $BASE_DIR/../custom-scripts/pythonServer $BASE_DIR/target/etc/init.d

