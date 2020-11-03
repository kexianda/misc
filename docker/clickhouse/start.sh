
# when start container(docker run):
# 1. mapping local user into container
# 2. add home volume
# 3. start sshd in container

USER_ID=$(id -u ${USER})
GROUP_ID=$(id -g ${USER})
USER_NAME=`whoami`
TZ=`/bin/ls -l /etc/localtime|cut -d"/" -f8,9`
pub=`cat ~/.ssh/id_rsa.pub`

# local port for login to container
SSH_PORT=8322

# mapping host's user into the container and start sshd
dev_dir=~/.dev
if [[ ! -d "${dev_dir}" ]]; then
    mkdir -p ${dev_dir}
fi
START_SH=.ch_start.sh
cat > ${dev_dir}/${START_SH} <<EOF
# mapping local user into the container and set bash as default shell
if [ ${USER_ID:-0} -ne 0 ] && [ ${GROUP_ID:-0} -ne 0 ]; then
    if id -u ${USER_NAME} >/dev/null 2>&1; then userdel -f ${USER_NAME}; fi
    if getent group ${USER_NAME} ; then groupdel ${USER_NAME} ; fi
    groupadd -g ${GROUP_ID} ${USER_NAME}
    useradd --no-log-init -u ${USER_ID} -g ${USER_NAME} ${USER_NAME} -s /bin/bash
    install -d -m 0755 -o ${USER_NAME} -g ${USER_NAME} /home/${USER_NAME}
    sudo usermod -aG sudo ${USER_NAME} && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
    echo '${USER_NAME}:nudata' | chpasswd
fi

ulimit -c unlimited
sysctl -w kernel.core_pattern=/var/crash/core.%u.%e.%p

# start sshd
/usr/sbin/sshd -D
EOF

## start docker

home_dev_dir=/home/${USER_NAME}/.dev
USER_NAME=`whoami`
CONTAINER_NAME='clickhouse-dev'
docker stop ${CONTAINER_NAME} && docker rm ${CONTAINER_NAME}
docker run -d -p ${SSH_PORT}:22 \
    --name ${CONTAINER_NAME} -h ${CONTAINER_NAME} \
    --cap-add=SYS_PTRACE --security-opt seccomp=unconfined  \
    -v ~/../:/home/  \
    -t clickhouse-dev-img  /bin/bash ${home_dev_dir}/${START_SH}


# for login
if [ `uname` == 'Linux' ]; then
    sed -i '/0.0.0.0:8322/d' ~/.ssh/known_hosts
else
    sed -i.bak '/0.0.0.0:8322/d' ~/.ssh/known_hosts
fi