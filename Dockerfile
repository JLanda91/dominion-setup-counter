FROM scratch
WORKDIR /run
COPY /cmake-build-releasestatic/dominion_setup_counter .
ENTRYPOINT [ "./dominion_setup_counter" ]