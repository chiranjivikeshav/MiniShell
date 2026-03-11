FROM debian:11 AS builder

# install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake

# Set working directory
WORKDIR /build

# copy current host directory → container /build
COPY . .

# build mnsh
RUN cmake -S . -B build
RUN cmake --build build

# final stage (only binary)
FROM scratch

COPY --from=builder /build/build/mnsh /minishell_pkg/usr/bin/mnsh