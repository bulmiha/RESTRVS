FROM ubuntu:latest
RUN bash -c "apt update && apt install libcpprest-dev libboost-program-options-dev cmake -y"
RUN bash -c "apt install libboost-test-dev -y"
COPY . /app
RUN bash -c "cd /app/build && cmake .. && make"
ENV DB_HOST redis
ENV DB_PORT 6379
CMD /app/build/REST
