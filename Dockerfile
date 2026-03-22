FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++-multilib \
    gcc-multilib \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

CMD mkdir -p build_linux && cd build_linux && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/app && \
    cmake --build . --config Release -j$(nproc) && \
    cmake --install . && \
    echo "--- Done! Check your Release folder ---"