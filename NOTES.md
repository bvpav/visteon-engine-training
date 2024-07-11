# Git submodules setup (`.gitmodules`)

```console
$ mkdir src thirdparty
$ git submodule add https://github.com/glfw/glfw.git thirdparty/glfw
$ git submodule add https://github.com/nigels-com/glew.git thirdparty/glew
$ git submodule update --init --recursive
```

## Videos

- [How do Video Game Graphics Work?](https://www.youtube.com/watch?v=C8YtdC8mxTU)
- [The Cherno's OpenGL Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)
- [An introduction to Shader Art Coding](https://www.youtube.com/watch?v=f4s1h2YETNY)
- [Graphics Pipeline Overview - Vulkan Game Engine Tutorial 02](https://www.youtube.com/watch?v=_riranMmtvI)

## VM Commands

### Fixing the VirtualBox install

The unattended VirtualBox install is a bit of a pain. Here are some problems we ran into:

1. The user `vboxuser` is not a member of the `sudo` group. This means that the user cannot run `sudo` commands.
2. A misconfigured `locale` is causing `gnome-terminal` to crash on launch.
3. The locale was in Ukrainian.

We first need to add the user to the `sudo` group:

```console
$ su
# usermod -aG sudo vboxuser
```

StackOverflow suggested that we remove the `gnome-terminal` package and reinstall it.

```console
# apt-get remove gnome-terminal
# apt-get install gnome-terminal
```

Next, we need to fix the locale.

```console
# locale-gen --purge
# dpkg-reconfigure locales
# reboot
```

At this point we discovered that the locale was in Ukrainian. We need to change it to English.

```console
$ sudoedit /etc/default/locale
$ sudo reboot
```

### Installing the SDK

1. Run the SDK installer

```console
$ cd Downloads/
$ ls
$ chmod +x ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin
$ ./ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin 
```

Click through the install steps. The install will take a while.

## Bash histories (ignore)

```console
$ exit
$ touch ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin 
$ file ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin
$ sha256sum ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin
$ cat /etc/default/locale 
$ su
$ sudo
$ sudo -l
$ adasd
$ sudoedit /etc/default/locale 
$ reboot
$ cd Downloads/
$ ls
$ sha256sum ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin 
$ rm ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin 
$ md5sum ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin
```

```console
# usermod -aG wheel vboxuser
# usermod -aG sudo vboxuser
# apt-get remove gnome-terminal
# apt-get install gnome-terminal
# locale-gen --purge
# dpkg-reconfigure locales
# reboot
```

## Building a container image with the SDK

```console
buildah bud -t ti-processor-sdk-linux:adas-j721s2-evm-09_02_00_05-Linux-x86 .
```
