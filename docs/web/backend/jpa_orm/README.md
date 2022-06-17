[toc]

# JPA与ORM

**ORM(Object Relation Mapping)** 思想是建立实体类和数据库元数据的自动映射关系，减少数据库访问层的代码编写；

**JPA(Java Persistence API) 是Java EE5.0的平台标准的ORM规范，Hibernate兼容**。

- @Entity代表是一个实体，表示与数据库有映射关系；
- @Table表示与之对应的数据表，可以看到类属性与数据表的列是顺序对应的关系，属性上的注解表明了列的类型与状态；
- @Column表示是表中的某个字段；
- @Id表明是主键。当然也可以不使用注解，在xml中配置；

- @Transient表示成员不是数据库中的字段；