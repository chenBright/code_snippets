# Ubuntu C++ 编译环境
# 参考
# https://www.binss.me/blog/learn-docker-with-me-about-building-compiler-environment/
# https://blog.phpgao.com/zsh_in_docker.html

From ubuntu:latest
LABEL chenBright <1021774709@qq.com>

# 换源
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list && \
    apt clean && \
    apt update

# git
RUN apt install -y git

# zsh
RUN apt install -y zsh
RUN git clone https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh \
    && cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc \
    && chsh -s /bin/zsh
RUN echo $SHELL

# 参考
# https://segmentfault.com/a/1190000015283092
# https://juejin.im/post/5cf34558f265da1b80202d75
# 安装 autojump 插件
RUN apt install -y autojump
# 安装 zsh-syntax-highlighting 插件
RUN git clone https://github.com/zsh-users/zsh-syntax-highlighting.git \
    ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting
# 安装 zsh-autosuggestions 插件
RUN git clone https://github.com/zsh-users/zsh-autosuggestions \
    ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions
# 安装 git-open 插件
RUN git clone https://github.com/paulirish/git-open.git \
    ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/git-open
RUN sed -ri 's/^plugins=.*/plugins=(git autojump zsh-syntax-highlighting zsh-autosuggestions git-open)/' ~/.zshrc


# 编译工具
RUN apt install -y build-essential
RUN apt install -y cmake
RUN apt install -y gdb gdbserver
RUN apt install -y rsync # Clion 需要 rsync 同步数据
RUN apt install -y vim

# ssh服务器
# 参考 https://github.com/rastasheep/ubuntu-sshd/blob/ed6fffcaf5a49eccdf821af31c1594e3c3061010/18.04/Dockerfile
RUN apt install -y openssh-server
RUN mkdir /var/run/sshd && \
    mkdir /root/.ssh
# 修改 root 的密码为 123456
RUN echo 'root:123456' | chpasswd
RUN sed -ri 's/^#?PermitRootLogin\s+.*/PermitRootLogin yes/' /etc/ssh/sshd_config
# SSH login fix. Otherwise user is kicked off after login
RUN sed -ri 's/UsePAM yes/#UsePAM yes/g' /etc/ssh/sshd_config

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# 删除 apt update 产生的缓存文件
# 因为 docker 的文件系统是层文件系统，上一个层中缓存有apt-get update的结果，
# 那么下次 Dockerfile 运行时就会直接使用之前的缓存，
# 这样 docker 中的 apt 软件源就不是最新的软件列表了，将会带来缓存过期的问题。
# 并且这些缓存将占用不少空间，导致最终生成的image非常庞大，
# 而这些垃圾文件是我们最终的image中无需使用到的东西，我们应当在Docker构建过程中予以删除。
RUN apt clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]

