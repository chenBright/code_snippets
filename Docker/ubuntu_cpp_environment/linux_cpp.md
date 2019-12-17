我现在做的是 Linux C++ 开发，但手头上没有 Linux 设备。目前的常用的方法有两种：

1. 云主机；
2. 虚拟机。

但是使用云主机不方便，使用虚拟机对电脑有一定要求。还有第三种方法：Docker。

本文介绍我使用 Docker 搭的一个 Linux C++ 编译环境。以下是我的需求：

- 可远程调试的 C++ 编译环境；
- 远程连接工具：ssh；
- 版本控制工具：git；
- 编辑器：Vim；
- 好用的 shell：zsh 和插件。

## 系统

常用的 Linux 系统有：CentOS 和 Ubuntu，我选择了Ubuntu。

```dockerfile
From ubuntu:latest
```

## 换源

由于 Ubuntu 中包管理工具的源的服务器在国外，在国内使用 apt 安装软件，网速经常会很慢。先将源换成国内的源（阿里云）。

```dockerfile
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list && \
    apt clean && \
    apt update
```

## C++编译环境

`build-essential`包含了编译所需要的软件，不过`cmake`需要另外安装。另外，Clion 需要`rsync`来同步数据。

```dockerfile
# 编译工具
RUN apt install -y build-essential
RUN apt install -y cmake
RUN apt install -y gdb gdbserver
RUN apt install -y rsync # Clion 需要 rsync 同步数据
RUN apt install -y vim # vim
RUN apt install -y git # git
```

## ssh
参考[GitHub](https://github.com/rastasheep/ubuntu-sshd/blob/ed6fffcaf5a49eccdf821af31c1594e3c3061010/18.04/Dockerfile)。不同 Ubuntu 版本的配置有差异。该仓库中有其他版本的配置信息。

```dockerfile
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
```

## Zsh

参考[把zsh放到docker里](https://blog.phpgao.com/zsh_in_docker.html)先将 Bash 换成 Zsh。

```dockerfile
RUN apt install -y zsh
RUN git clone https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh \
    && cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc \
    && chsh -s /bin/zsh
```

还安装了一些插件：

```dockerfile
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
```

## Dockerfile

在我的[GitHub](https://github.com/chenBright/)上有[Dockerfile](https://github.com/chenBright/code_snippets/blob/master/Docker/ubuntu_cpp_environment/Dockerfile)文件。