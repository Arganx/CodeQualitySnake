FROM alpine:3.20.1

# Enable testing
RUN echo -e "https://dl-cdn.alpinelinux.org/alpine/edge/main\nhttps://dl-cdn.alpinelinux.org/alpine/edge/community\n@testing https://dl-cdn.alpinelinux.org/alpine/edge/testing\n" \
    > /etc/apk/repositories

# Install required packages and clean cache
RUN apk update && apk add bash make g++ sfml-dev cmake bazel@testing\
    && apk cache clean

# Remove unnacasary files to reduce image size
RUN rm -rf /tmp/* /usr/share/doc/* /usr/share/info/* /var/tmp/*

# Create user
RUN adduser --disabled-password user \
    && mkdir /home/user/snake

WORKDIR /home/user/snake

# Copy snake source code
COPY ./.. /home/user/snake

# Allow the user to access build directory and SQL (otherwise database can't be compiled and option file can't be created)
RUN chmod -R 777 /home/user/snake/SQL-Lite \
    && chmod 777 /home/user/snake

# Update display to enable support for X11
RUN echo "export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0" >> /home/user/.bashrc

USER user