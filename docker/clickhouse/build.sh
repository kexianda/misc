docker rmi clickhouse-dev-img
docker build --build-arg TZ=${TZ} -t clickhouse-dev-img .