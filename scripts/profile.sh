#!/bin/bash

perf record --latency ./build/chess
perf report --latency