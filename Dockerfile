########################################################################################################################
# Build Stage
########################################################################################################################
FROM alpine:latest as build

RUN apk update && \
    apk add --no-cache \
        build-base \
        cmake \
        boost-dev

WORKDIR /app

COPY . .

WORKDIR /app/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .

########################################################################################################################
# deploy stage
########################################################################################################################

FROM alpine:latest

RUN apk update && \
    apk add --no-cache \
    libstdc++

RUN addgroup -S appusers && adduser -S appuser -G appusers
USER appuser

COPY --chown=appuser:appusers \
    --from=build \
    ./app/build/main \
    ./run/

ENTRYPOINT [ "./run/main" ]