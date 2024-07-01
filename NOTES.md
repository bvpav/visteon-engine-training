# Git submodules setup (`.gitmodules`)

```console
$ mkdir src thirdparty
$ git submodule add https://github.com/glfw/glfw.git thirdparty/glfw
$ git submodule add https://github.com/nigels-com/glew.git thirdparty/glew
$ git submodule update --init --recursive
```