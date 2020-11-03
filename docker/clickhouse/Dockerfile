FROM ubuntu:20.04

LABEL version="1.0"
LABEL description="Ubuntu 20.04 image for ClickHouse"

ARG TZ


# install sshd
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
    && apt-get update && apt-get install -y sudo openssh-server \
    && mkdir /var/run/sshd   \
    && echo 'root:nudata' | chpasswd  \
    && sed -i 's/#*PermitRootLogin prohibit-password/PermitRootLogin yes/g' /etc/ssh/sshd_config  \
    && sed -i 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' /etc/pam.d/sshd  \
    && echo "export VISIBLE=now" >> /etc/profile \
    && apt-get install -yq tree lsof net-tools \
    && apt-get install -yq vim git \
    && apt-get install -yq gdb gdbserver gcc-9 g++-9 ccache cmake ninja-build \
    && apt-get install -yq python

# ENV NOTVISIBLE "in users profile"

ENV TZ "Asia/Shanghai"
ENV CC "gcc-9"
ENV CXX "g++-9"

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
