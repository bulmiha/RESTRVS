FROM ubuntu:latest
RUN bash -c "apt update && apt install libcpprest-dev libboost-program-options-dev cmake -y"
COPY . /app
RUN bash -c "cd /app/build && cmake .. && make"
CMD /app/build/REST
