[toc]

# CAS

> CAS : Compare And Swap，乐观锁机制

Java的原子类AtomicXXX 都是使用CAS保证了线程安全。

- **ABA问题**：我们拿到期望值A，再用CAS写新值C，这两个操作并不能保证原子性。因此，就有可能出现实际值已经改为B，又改回A，这种情况。当执行CAS时，不会识别ABA问题，只要看实际值还是A，就执行了。
- 原子化的对象引用：对象引用都是封装类型，支持CAS操作。相关实现包括：AtomicReference、AtomicStampedReference和AtiomicMarkableReference。AtomicStampedReference和AtiomicMarkableReference解决了ABA问题。
- 原子化数组：AtomicIntegerArray、AtomicLongArray和AtomicReferenceArray；
- 原子化对象属性更新器：AtomicIntegerFieldUpdater、AtomicLongFiledUpdater和AtomicReferenceFieldUpdater
- 原子化的累加器：DoubleAccumulator、DoubleAdder、LongAccumulator和LongAdder

- 

