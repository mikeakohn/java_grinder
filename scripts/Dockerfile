
FROM centos:7
RUN /usr/sbin/useradd makerpm
RUN yum -y install gcc-c++ git-all make readline-devel rpmdevtools wget
RUN mkdir -p /storage/git
ADD build.sh /home/makerpm/
RUN chmod 777 /storage/git

