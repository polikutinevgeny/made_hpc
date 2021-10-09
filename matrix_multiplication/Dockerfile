FROM gcc:11.2

RUN apt-get update && apt-get -y install cmake && rm -rf /var/lib/apt/lists/*
COPY CMakeLists.txt main.cpp matrix.cpp matrix.hpp vector.cpp vector.hpp /
RUN cmake -DCMAKE_BUILD_TYPE=Release . && cmake --build .

CMD ["./matrix_multiplication"]
