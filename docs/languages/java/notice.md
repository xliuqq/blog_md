# 编程纪要

- **入参非空检查并赋值**

- - Objects.requireNonNull ；
  - Preconditions.checkNotNull； Guava
  - Optional.ofNullable(inputName).orElse("SOME_DEFAULT_NAME")；

- 使用**Optional简化多级if判空**(.map().map().ifPresent())；
- 使用Optional包装可能为空的返回值；

- lombok.NonNull、javax.annotation.Nonnull 和 javax.validation.constraints.NotNull的区别和使用场景：

- - **@NonNull**用在强制入参非空、属性非空的场景下，编译时会生成空指针检查的代码，如果不满足，则会抛出NullPointerException。
  - **@Nonnull**用在返回结果不为空的场景，主要由IDE识别并给出告警提示。
  - **@NotNull**主要用在Validator校验场景下，限制参数非空，如果不满足，则校验失败。

