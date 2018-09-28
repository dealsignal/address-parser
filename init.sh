#!/bin/bash
PORT=3000

SPAWN_CGI=spawn-fcgi
PARSER=/usr/bin/address-parser-cgi
USER=root

CGI_ARGS=`$SPAWN_CGI -p $PORT -n $PARSER`
RETVAL=0

start() {
      echo -n "Starting Address Parsing Server: "
      start-stop-daemon --quiet --start --background --chuid "$USER" --exec /usr/bin/env -- $CGI_ARGS
      RETVAL=$?
}

stop() {
      echo -n "Stopping Address Parsing Server: "
      pkill -f $PARSER
      RETVAL=$?
}

case "$1" in
    start)
      start
  ;;
    stop)
      stop
  ;;
    restart)
      stop
      start
  ;;
    *)
      echo "Usage: address-parser  {start|stop|restart}"
      exit 1
  ;;
esac
exit $RETVAL