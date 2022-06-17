# Context

上下文 `context.Context` Go 语言中用来**设置截止日期、同步信号，传递请求相关值**的结构体。

可能会创建多个 Goroutine 来处理一次请求，而 [`context.Context`](https://draveness.me/golang/tree/context.Context) 的作用是在不同 Goroutine 之间同步请求特定数据、取消信号以及处理请求的截止日期。

<img src="pics/golang-context-usage.png" alt="golang-context-usage" style="zoom:67%;" />

每一个 `context.Context` 都会从最顶层的 Goroutine 一层一层传递到最下层。`context.Context` 可以在上层 Goroutine 执行出现错误时，将信号及时同步给下层。

## 使用

```go
func main() {
	ctx, cancel := context.WithTimeout(context.Background(), 1*time.Second)
	defer cancel()

	go handle(ctx, 500*time.Millisecond)
	select {
	case <-ctx.Done():
		fmt.Println("main", ctx.Err())
	}
}

func handle(ctx context.Context, duration time.Duration) {
	select {
	case <-ctx.Done():
		fmt.Println("handle", ctx.Err())
	case <-time.After(duration):
        // 500ms < 1s, run here
		fmt.Println("process request with", duration)
	}
}
```

