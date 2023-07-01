[toc]

# Apache APISix

> APISIX is built on top of Nginx and [ngx_lua](https://github.com/openresty/lua-nginx-module) leveraging the power offered by LuaJIT.

基于Etcd的watch机制，更新在毫秒级（推送的形式），而其他（如Kong）在5秒级别，且是拉取的形式。

<img src="pics/flow-software-architecture.png" alt="flow-software-architecture" style="zoom: 50%;" />

APISIX has two main parts:

1. APISIX core, Lua plugin, multi-language Plugin runtime, and the WASM plugin runtime.
2. Built-in Plugins that adds features for observability, security, traffic control, etc.

## Plugin

