FROM alpine:latest AS build

RUN apk add --no-cache build-base make

WORKDIR /app
COPY . .

RUN make clean && make ARCH=linux

FROM alpine:latest
WORKDIR /app
COPY --from=build /app/bin/linux/metrics_daemon ./metrics_daemon

EXPOSE 8080

CMD ["./metrics_daemon"]