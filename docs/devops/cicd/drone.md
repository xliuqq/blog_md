# [Drone](https://github.com/harness/drone)

> Drone by [Harness](https://harness.io/)™ is a modern Continuous Integration platform that empowers busy teams to automate their build, test and release workflows using a powerful, cloud native pipeline engine.



## 使用

### 示例Yaml

```yaml
name: default

kind: pipeline
type: docker

steps:
- name: backend
  image: golang
  commands:
    - go get
    - go build
    - go test

- name: frontend
  image: node:6
  commands:
    - npm install
    - npm test

- name: publish
  image: plugins/docker
  settings:
    repo: octocat/hello-world
    tags: [ 1, 1.1, latest ]
    registry: index.docker.io

- name: notify
  image: plugins/slack
  settings:
    channel: developers
    username: drone
```