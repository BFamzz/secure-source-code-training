FROM staging
WORKDIR /injection-flaws
COPY . .
WORKDIR /injection-flaws/build
RUN cmake \
    -I/usr/local/include/ppxx \
    -L/usr/local/lib \
    -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql \
    -DPostgreSQL_INCLUDE_DIR=/usr/lib/x86_64-linux-gnu .. && \
    make
CMD [ "./injection-flaws" ]