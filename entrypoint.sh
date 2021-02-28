#!/bin/sh
. /opt/er/build/config.sh
. /opt/go4/go4login
export LD_LIBRARY_PATH=/opt/accdaq/install/:$LD_LIBRARY_PATH
exec "$@"