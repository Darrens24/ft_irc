#!/bin/bash

pid=$(lsof -t -i:4242)
if [ -n "$pid" ]; then
  kill -9 $pid
fi
