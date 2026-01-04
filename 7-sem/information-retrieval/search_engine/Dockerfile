FROM ubuntu:22.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive
ENV VCPKG_ROOT=/opt/vcpkg
ENV VCPKG_DEFAULT_TRIPLET=x64-linux

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build git curl pkg-config ca-certificates python3 \
    zip unzip tar linux-libc-dev \
  && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg ${VCPKG_ROOT} \
  && ${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics

WORKDIR /src
COPY . .

RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
 && cmake --build build -j

# ---------------- runtime ----------------
FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ENV VCPKG_DEFAULT_TRIPLET=x64-linux

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /src/build/indexer /app/indexer
COPY --from=builder /src/build/search_server /app/search_server
COPY --from=builder /src/build/search_cli /app/search_cli

COPY --from=builder /src/build/vcpkg_installed /opt/vcpkg_installed
ENV LD_LIBRARY_PATH=/opt/vcpkg_installed/${VCPKG_DEFAULT_TRIPLET}/lib:/opt/vcpkg_installed/${VCPKG_DEFAULT_TRIPLET}/bin

COPY docker/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

EXPOSE 8080
ENTRYPOINT ["/entrypoint.sh"]
